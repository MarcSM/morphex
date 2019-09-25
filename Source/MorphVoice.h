/*
  ==============================================================================

    MorphVoice.h
    Created: 21 Jul 2019 9:22:59pm
    Author:  Marc Sanchez Martinez

  ==============================================================================
*/

#pragma once

#include "MorphSound.h"

#include "Sound.h"

#include "SynthesisEngine.h"

#include "SMTAudioHelpers.h"
#include "SMTHelperFunctions.h"

#include "SMTParameters.h"

enum class HarmonicTranspositionMode
{
    NoTransposition = 0,        // Keep the original harmonics
    RealFundamentalTracking,    // Real fundamental tracking
    PredominantFundamental      // Predominant fundamental normalization
};

enum class MagnitudeProcessingMode
{
    NoProcessing = 0,   // Keep the original magnitudes
    Normalize,          // Normalize the magnitudes across all sounds
    NormalizeToHighest  // Normalize to the highest magnitude of all sounds
};

struct MorphVoice
:   public SynthesiserVoice
{
    MorphVoice(SoundArray& sound, AudioProcessorValueTreeState* parameters)
    : mSound(sound)
    {
        play_sound = false;
        one_shot = true;
        
        // Default harmonic transposition mode
        harmonic_transp_mode = HarmonicTranspositionMode::PredominantFundamental;
        
        // Default magnitude processing mode
        magnitude_process_mode = MagnitudeProcessingMode::Normalize;
        
        // Keeping the parameters pointer within the class
        mParameters = parameters;
        
        number_of_steps = 4;
        mDAW_Buffer_Size = 512; // numSamples
        
        generated_sines_real = new float[mDAW_Buffer_Size];
        ifft_output = new juce::dsp::Complex<float>[mDAW_Buffer_Size];
        ifft_output_real = new float[mDAW_Buffer_Size];
        
        ifft_stocs_output = new juce::dsp::Complex<float>[mDAW_Buffer_Size];
        
        NS = 512; // size of fft used in synthesis - 512
        H = int(NS/4); // hop size (has to be 1/4 of NS) - 128
        NUMBER_OF_FFT_SYNTH_FRAMES = 4; // Number of FFT synth frames
        MORPH_CIRCULAR_BUFFER_LENGTH = NS * NUMBER_OF_FFT_SYNTH_FRAMES;
        
        // Circular Buffers
        mCircularBufferLeft = new float[MORPH_CIRCULAR_BUFFER_LENGTH];
        mCircularBufferRight = new float[MORPH_CIRCULAR_BUFFER_LENGTH];
        
        // Initialize the synthesis engine
        mSynthesis = std::make_unique<SynthesisEngine>();
        
        reset();
    }
    
    bool canPlaySound (SynthesiserSound* synthSound) override
    {
        if (mSound[1]->loaded && mSound[2]->loaded)
        {
            return dynamic_cast<MorphSound*> (synthSound) != nullptr;
        }
        else
        {
            return false;
        }
    }
    
    void setADSRSampleRate(double sampleRate)
    {
        adsr.setSampleRate(sampleRate);
    }
    
    void updateAdsrParams(float attack, float decay, float sustain, float release)
    {
        adsrParams.attack = attack;
        adsrParams.decay = decay;
        adsrParams.sustain = sustain;
        adsrParams.release = release;
        
        // Update ADSR parameters
        adsr.setParameters(adsrParams);
    }
    
    void reset()
    {
        // Initialize the harmonics pointer to 0
        mHarmonicsHead = 0; // min_harmonic_frame
        
        // Initialize the write and play heads to 0
        mCircularBufferWriteHead = 0;
        mCircularBufferPlayHead = 0;
        
        // Initialize all the pointers to 0
        mHeadWritePointer = 0;
        mTailWritePointer = 0;
        mHeadPlayPointer = 0;
        mTailPlayPointer = 0;
        mTailCleanPointer = 0;
        
        // Clean the buffers
        zeromem(mCircularBufferLeft, sizeof(float) * MORPH_CIRCULAR_BUFFER_LENGTH);
        zeromem(mCircularBufferRight, sizeof(float) * MORPH_CIRCULAR_BUFFER_LENGTH);
    }
    
    void startNote( int midiNoteNumber, float velocity,
                    SynthesiserSound*, int /*currentPitchWheelPosition*/) override
    {
        // Start ADSR envelope
        adsr.noteOn();
        
        // Enable sound playback
        play_sound = true;
        
        // Reset sample playback to the beginning
        mHarmonicsHead = 0;
        
        // Velocity affects the gain
        level = velocity * 0.15;
        mAttackGainSmoothed = 0.0;
        mDecayGainSmoothed = 1.0;
        
        last_freqs.resize(0);
        last_freqs = mSound[1]->harmonic_frequencies[0];
        phase_morph.resize(0);
        phase_morph.resize(mSound[1]->harmonic_frequencies[0].size(), 0.0);
        std::generate(phase_morph.begin(), phase_morph.end(), RandomGenerator(0.0, 0.0));
        
        for (int i=0; i<phase_morph.size(); i++) phase_morph[i] = 2 * M_PI * phase_morph[i];

        cycles_per_second = MidiMessage::getMidiNoteInHertz(midiNoteNumber);
        cycles_per_sample = cycles_per_second / getSampleRate();
    }
    
    void stopNote(float /*velocity*/, bool allowTailOff) override
    {
        // Start ADSR envelope
        adsr.noteOff();
        
        // Disable sound playback
        play_sound = false;
        clearCurrentNote();
    }
    
    void pitchWheelMoved (int /*newValue*/) override                              {}
    void controllerMoved (int /*controllerNumber*/, int /*newValue*/) override    {}
    
    void renderNextBlock (AudioBuffer<float>& outputBuffer, int startSample, int numSamples) override
    {
        if (play_sound && numSamples == 512 && mSound[1]->loaded && mSound[2]->loaded)
        {
            float adsr_attack = *mParameters->getRawParameterValue(SMTParameterID[kParameter_asdr_attack]);
            float adsr_decay = *mParameters->getRawParameterValue(SMTParameterID[kParameter_asdr_decay]);
            float adsr_sustain = *mParameters->getRawParameterValue(SMTParameterID[kParameter_asdr_sustain]);
            float adsr_release = *mParameters->getRawParameterValue(SMTParameterID[kParameter_asdr_release]);
            
            // Update ADSR parameters
            updateAdsrParams(adsr_attack, adsr_decay, adsr_sustain, adsr_release);
            
            // The two sounds must have the same "fs" at this point
            sample_rate = mSound[1]->file.fs;
            
            if (last_freqs.size() <= 0)
            {
                // Initialize last_freqs with the first harmonic frequencies frame
                last_freqs = mSound[1]->harmonic_frequencies[0];
            }
            
            if (phase_morph.size() <= 0)
            {
                // Initialize the phases randomly
                phase_morph.resize(mSound[1]->harmonic_frequencies[0].size(), 0.0);
                std::generate(phase_morph.begin(), phase_morph.end(), RandomGenerator(0.0, 1.0));
                for (int i=0; i<phase_morph.size(); i++) phase_morph[i] = 2 * M_PI * phase_morph[i];
            }
            
            float freqs_interp_factor = *mParameters->getRawParameterValue(SMTParameterID[kParameter_freqs_interp_factor]);
            float mags_interp_factor = *mParameters->getRawParameterValue(SMTParameterID[kParameter_mags_interp_factor]);
//            float stocs_interp_factor = *mParameters->getRawParameterValue(SMTParameterID[kParameter_stocs_interp_factor]);
//            float stocs_gain = *mParameters->getRawParameterValue(SMTParameterID[kParameter_stocs_gain]);
            float min_harmonic_frame = 0; // TODO add this as a parameter (slider?)
            float max_harmonic_frame = 40000; // TODO add this as a parameter (slider?)
            
            for (int i_buffer = 1; i_buffer <= number_of_steps; i_buffer++)
            {
                // Get the non-zero values of each frame
                non_zero_harmonic_frequencies_1 = getNonZeroIndices(mSound[1]->harmonic_frequencies[mHarmonicsHead]);
                non_zero_harmonic_frequencies_2 = getNonZeroIndices(mSound[2]->harmonic_frequencies[mHarmonicsHead]);
                
                // Interpolating the harmonics of the matching harmonics
                std::vector<int> harmonics(0);
                std::set_intersection(non_zero_harmonic_frequencies_1.begin(), non_zero_harmonic_frequencies_1.end(),
                                      non_zero_harmonic_frequencies_2.begin(), non_zero_harmonic_frequencies_2.end(),
                                      std::back_inserter(harmonics));
                
                // Initializing the sound harmonic frequencies
                std::vector<float> sound_harmonic_frequencies[3];
                
                // If harmonic transposition mode is predominant fundamental normalization
                if (harmonic_transp_mode == HarmonicTranspositionMode::PredominantFundamental)
                {
                    for (int i_sound=1; i_sound<=2; i_sound++)
                    {
                        // Get the harmonic frequencies for this sound
                        sound_harmonic_frequencies[i_sound] = mSound[i_sound]->harmonic_frequencies[mHarmonicsHead];
                        
                        // Check to avoid divide by zero
                        if (mSound[i_sound]->features.predominant_note > 0)
                        {
                            // Recalculate the harmonics for the current midi note
                            for (int i=0; i<sound_harmonic_frequencies[i_sound].size(); i++)
                            {
                                sound_harmonic_frequencies[i_sound][i] = (sound_harmonic_frequencies[i_sound][i] / mSound[i_sound]->features.predominant_note) * cycles_per_second;
                            }
                        }
                    }
                }
                else
                {
                    // No changes will be made to the frequencies
                    sound_harmonic_frequencies[1] = mSound[1]->harmonic_frequencies[mHarmonicsHead];
                    sound_harmonic_frequencies[2] = mSound[2]->harmonic_frequencies[mHarmonicsHead];
                }
                
                // Interpolating the frequencies of the matching harmonics
                std::vector<float> freqs_morph(0);
                freqs_morph = interpolateFrames(sound_harmonic_frequencies[1],
                                                sound_harmonic_frequencies[2],
                                                harmonics, freqs_interp_factor,
                                                FrameInterpolationMode::Harmonics);
                
                // Interpolating the magnitudes of the matching harmonics
                std::vector<float> mags_morph(0);
                mags_morph = interpolateFrames(mSound[1]->harmonic_magnitudes[mHarmonicsHead],
                                               mSound[2]->harmonic_magnitudes[mHarmonicsHead],
                                               harmonics, mags_interp_factor,
                                               FrameInterpolationMode::Magnitudes);
                
//                // Interpolating the stochastic component of the matching harmonics
//                std::vector<float> stocs_morph(0);
//                stocs_morph = interpolateFrames(mSound[1]->stochastic_residual[mHarmonicsHead],
//                                                mSound[2]->stochastic_residual[mHarmonicsHead],
//                                                harmonics, stocs_interp_factor,
//                                                FrameInterpolationMode::Stochastic);

                // If harmonic transposition mode is real fundamental tracking
                if (harmonic_transp_mode == HarmonicTranspositionMode::RealFundamentalTracking)
                {
                    // Fundamental note of this frame (in Hz)
                    float current_fundamental = freqs_morph[0];
                    
                    // Check to avoid divide by zero
                    if (current_fundamental > 0)
                    {
                        // Recalculate the harmonics for the current midi note
                        for (int i=0; i<freqs_morph.size(); i++)
                        {
                             freqs_morph[i] = (freqs_morph[i] / current_fundamental) * cycles_per_second;
                        }
                    }
                }
                
                // Generating the phases
                for (int i=0; i<last_freqs.size(); i++) phase_morph[i] += ( (M_PI * (last_freqs[i] + freqs_morph[i])) / sample_rate ) * H;
                
                // Keep phases inside 2 * pi
                for (int i=0; i<phase_morph.size(); i++) phase_morph[i] = std::fmod(phase_morph[i], (2*M_PI) );
                
                // Generate sines
                std::vector<std::complex<float>> generated_sines(0);
                generated_sines = genSpecSines(freqs_morph, mags_morph, phase_morph, NS, sample_rate);
                
                // Keep the frequencies for the next iteration
                last_freqs.resize(freqs_morph.size());
                for (int i=0; i<last_freqs.size(); i++) last_freqs[i] = freqs_morph[i];
                
                // Perform the IFFT (Harmonics)
                mSynthesis->fft->perform(generated_sines.data(), ifft_output, true);
                
                // Get the real part, we don't get the second half because it's empty
                for (int i = 0; i < numSamples; i++) ifft_output_real[i] = ifft_output[i].real();
                
                // Perform an FFT shift
                fftShift(ifft_output_real, numSamples);
                
                // TODO - Generate the stochastic component
//                std::vector<std::complex<float>> stoc_output = genStocComponent(stocs_morph, H, NS);
                // TODO - Perform the IFFT (Stochastic)
                // TODO - Apply stochastic gain
                
                // Keep the pointers inside the circular buffer
                mHeadWritePointer = CalculateModuloInt(mCircularBufferWriteHead, MORPH_CIRCULAR_BUFFER_LENGTH);
                mTailWritePointer = CalculateModuloInt((mCircularBufferWriteHead+NS), MORPH_CIRCULAR_BUFFER_LENGTH);
                mTailCleanPointer = CalculateModuloInt((mCircularBufferWriteHead+NS+H), MORPH_CIRCULAR_BUFFER_LENGTH);
                
                // Initialize the selected samples vector
                std::vector<int> buffer_samples_to_clean(0);
                
                // If the tail celan pointer is ahead the tail write pointer
                if ( (mTailCleanPointer-mTailWritePointer) < 0 )
                {
                    first_section_to_clean = getRangeInt(mTailWritePointer, MORPH_CIRCULAR_BUFFER_LENGTH);
                    buffer_samples_to_clean.insert(buffer_samples_to_clean.end(), first_section_to_clean.begin(), first_section_to_clean.end());
                    
                    second_section_to_clean = getRangeInt(0, mTailCleanPointer);
                    buffer_samples_to_clean.insert(buffer_samples_to_clean.end(), second_section_to_clean.begin(), second_section_to_clean.end());
                }
                else
                {
                    section_to_clean = getRangeInt(mTailWritePointer, mTailCleanPointer);
                    buffer_samples_to_clean.insert(buffer_samples_to_clean.end(), section_to_clean.begin(), section_to_clean.end());
                }
                
                // Clean the part of the buffer that we must overrid
                for (int i=0; i<buffer_samples_to_clean.size(); i++)
                    mCircularBufferLeft[buffer_samples_to_clean[i]] = 0;
                
                // Initialize the selected samples vector
                std::vector<int> selected_write_samples(0);
                
                // If the tail pointer is ahead the head pointer
                if ( (mTailWritePointer-mHeadWritePointer) < 0 )
                {
                    std::vector<int> first_section_to_write = getRangeInt(mHeadWritePointer, MORPH_CIRCULAR_BUFFER_LENGTH);
                    selected_write_samples.insert(selected_write_samples.end(), first_section_to_write.begin(), first_section_to_write.end());
                    
                    std::vector<int> second_section_to_write = getRangeInt(0, mTailWritePointer);
                    selected_write_samples.insert(selected_write_samples.end(), second_section_to_write.begin(), second_section_to_write.end());
                }
                else
                {
                    std::vector<int> section_to_write = getRangeInt(mHeadWritePointer, mTailWritePointer);
                    selected_write_samples.insert(selected_write_samples.end(), section_to_write.begin(), section_to_write.end());
                }
                
                // Applying the window and saving the result on the buffer
                for (int i=0; i<selected_write_samples.size(); i++)
                {
                    mCircularBufferLeft[selected_write_samples[i]] += mSynthesis->window[i] * ifft_output_real[i];
                }
                
                if (i_buffer % number_of_steps == 0)
                {
                    // Initialize the selected samples vector
                    std::vector<int> selected_play_samples(0);
                    
                    // Keep the pointers inside the circular buffer
                    mHeadPlayPointer = CalculateModuloInt(mCircularBufferPlayHead, MORPH_CIRCULAR_BUFFER_LENGTH);
                    mTailPlayPointer = CalculateModuloInt((mCircularBufferPlayHead+NS), MORPH_CIRCULAR_BUFFER_LENGTH);
                    
                    // If the tail pointer is ahead the head pointer
                    if ( (mTailPlayPointer-mHeadPlayPointer) < 0 )
                    {
                        std::vector<int> first_section_to_play = getRangeInt(mHeadPlayPointer, MORPH_CIRCULAR_BUFFER_LENGTH);
                        selected_play_samples.insert(selected_play_samples.end(), first_section_to_play.begin(), first_section_to_play.end());
                        
                        std::vector<int> second_section_to_play = getRangeInt(0, mTailPlayPointer);
                        selected_play_samples.insert(selected_play_samples.end(), second_section_to_play.begin(), second_section_to_play.end());
                    }
                    else
                    {
                        std::vector<int> section_to_play = getRangeInt(mHeadPlayPointer, mTailPlayPointer);
                        selected_play_samples.insert(selected_play_samples.end(), section_to_play.begin(), section_to_play.end());
                    }
                    
                    for (int i=0; i<numSamples; i++)
                    {
                        for (auto i_channel = outputBuffer.getNumChannels(); --i_channel >= 0;)
                        {
                            mAttackGainSmoothed = mAttackGainSmoothed - 0.004 * (mAttackGainSmoothed - 1.0);
                            
                            // TODO - This operation can be done before
                            int min_frame = int( std::min( mSound[1]->harmonic_frequencies.size(), mSound[2]->harmonic_frequencies.size() ) );
                            
                            if ( (min_frame - mHarmonicsHead) < 12)
                            {
                                if (mDecayGainSmoothed <= 0.1) mDecayGainSmoothed = 0.0;
                                else mDecayGainSmoothed = mDecayGainSmoothed - 0.1;
                            }

                            auto current_sample = mCircularBufferLeft[selected_play_samples[i]] * adsr.getNextSample() * mAttackGainSmoothed * mDecayGainSmoothed * level;
                            outputBuffer.addSample (i_channel, startSample, current_sample);
                        }
                        
                        ++startSample;
                    }
                }
                
                // Keep the pointers inside the circular buffer length
                if (mCircularBufferWriteHead >= MORPH_CIRCULAR_BUFFER_LENGTH) mCircularBufferWriteHead = 0;
                if (mCircularBufferPlayHead >= MORPH_CIRCULAR_BUFFER_LENGTH) mCircularBufferPlayHead = 0;
                
                // Incrementing the pointers
                mHarmonicsHead++;
                mCircularBufferWriteHead += H;
                mCircularBufferPlayHead = mCircularBufferWriteHead - NS;
                
                // If we have reached the end of the sounds
                if (mHarmonicsHead >= max_harmonic_frame ||
                    mHarmonicsHead >= mSound[1]->harmonic_frequencies.size() ||
                    mHarmonicsHead >= mSound[2]->harmonic_frequencies.size() )
                {
                    // Prepare the header for the next loop
                    mHarmonicsHead = min_harmonic_frame;
                    
                    // If one shot is ON
                    if (one_shot)
                    {
                        // Stop playing
                        play_sound = false;
                    }
                }
                
                // TODO Just in case (control this in the min max range)
                if (mHarmonicsHead >= mSound[1]->harmonic_frequencies.size() ||
                    mHarmonicsHead >= mSound[2]->harmonic_frequencies.size() )
                    mHarmonicsHead = 0;
            }
        }
    }
    
private:
    
    // Note playback
    bool play_sound;
    bool one_shot;
    
    // Note calculations
    float cycles_per_second;
    float cycles_per_sample;
    
    // Harmonic transposition mode
    // TODO - Change this parameter for a pointer from MorphSynth
    HarmonicTranspositionMode harmonic_transp_mode;
    
    // Magnitude processing mode
    // TODO - Change this parameter for a pointer from MorphSynth
    MagnitudeProcessingMode magnitude_process_mode;
    
    AudioProcessorValueTreeState* mParameters;
    
    int NS; // Size of fft used in synthesis - 512
    int H; // Hop size (it has to be 1/4 of NS) - 128
    int NUMBER_OF_FFT_SYNTH_FRAMES; // Number of FFT synth frames
    int MORPH_CIRCULAR_BUFFER_LENGTH;
    
    // TODO - Temp Var (UPDATE AND REMOVE)
    int mDAW_Buffer_Size;
    
    // Circular Buffers
    float* mCircularBufferLeft;
    float* mCircularBufferRight;
    
    // Harmonics frame pointer
    int mHarmonicsHead;
    
    // Circular buffer pointers
    int mCircularBufferWriteHead;
    int mCircularBufferPlayHead;
    
    // Write pointers
    int mHeadWritePointer;
    int mTailWritePointer;
    
    // Play pointers
    int mHeadPlayPointer;
    int mTailPlayPointer;
    
    // Clean pointer
    int mTailCleanPointer;
    
    // Synthesis
    std::unique_ptr<SynthesisEngine> mSynthesis;
    
    // Synthesis values
    std::vector<float> last_freqs;
    std::vector<float> phase_morph;
    
    int number_of_steps;
    int sample_rate;
    
    std::vector<int> non_zero_harmonic_frequencies_1;
    std::vector<int> non_zero_harmonic_frequencies_2;
    
    float* generated_sines_real;
    juce::dsp::Complex<float>* ifft_output;
    float* ifft_output_real;
    
    juce::dsp::Complex<float>* ifft_stocs_output;
    
    // Sample section to clean
    std::vector<int> first_section_to_clean;
    std::vector<int> second_section_to_clean;
    std::vector<int> section_to_clean;
    
    // Sample section to write
    std::vector<int> first_section_to_write;
    std::vector<int> second_section_to_write;
    std::vector<int> section_to_write;
    
    // Sample section to play
    std::vector<int> first_section_to_play;
    std::vector<int> second_section_to_play;
    std::vector<int> section_to_play;
    
    // Frame that will be added to the audio output
    std::vector<float> frame_to_play;
    
    float mTimeSmoothed;
    int mDelayIndex;
    
    double level = 0.0;
    double mAttackGainSmoothed = 0.0;
    double mDecayGainSmoothed = 0.0;
    
    ADSR adsr;
    ADSR::Parameters adsrParams;
    
    SoundArray& mSound;
};
