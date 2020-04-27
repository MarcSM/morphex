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

//enum class HarmonicTranspositionMode
//{
//    NoTransposition = 0,        // Keep the original harmonics
//    RealFundamentalTracking,    // Real fundamental tracking
//    PredominantFundamental      // Predominant fundamental normalization
//};
//
//enum class MagnitudeProcessingMode
//{
//    NoProcessing = 0,   // Keep the original magnitudes
//    Normalize,          // Normalize the magnitudes across all sounds
//    NormalizeToHighest  // Normalize to the highest magnitude of all sounds
//};
//
//// Default harmonic transposition mode
//harmonic_transp_mode = HarmonicTranspositionMode::PredominantFundamental;
//
//// Default magnitude processing mode
//magnitude_process_mode = MagnitudeProcessingMode::Normalize;

struct MorphVoice
:   public SynthesiserVoice
{
    MorphVoice(AudioProcessorValueTreeState* parameters)
    : mParameters(parameters)
    {
        // Create a new instrument
    }
    
    bool canPlaySound (SynthesiserSound* synthSound) override
    {
        return true;
//        if (mSound[1]->loaded && mSound[2]->loaded)
//        {
//            return dynamic_cast<MorphSound*> (synthSound) != nullptr;
//        }
//        else
//        {
//            return false;
//        }
    }
    
    // TODO - Check if needed
//    void setADSRSampleRate(double sampleRate)
//    {
//        adsr.setSampleRate(sampleRate);
//    }
    
//    // TODO - Check if needed
//    void updateAdsrParams(float attack, float decay, float sustain, float release)
//    {
//        adsrParams.attack = attack;
//        adsrParams.decay = decay;
//        adsrParams.sustain = sustain;
//        adsrParams.release = release;
//
//        // Update ADSR parameters
//        adsr.setParameters(adsrParams);
//    }
    
//    void reset()
//    {
//        // Clean the buffers
//        zeromem(mCircularBufferLeft, sizeof(float) * MORPH_CIRCULAR_BUFFER_LENGTH);
//        zeromem(mCircularBufferRight, sizeof(float) * MORPH_CIRCULAR_BUFFER_LENGTH);
//    }
    
//    void startNote( int midiNoteNumber, float velocity,
//                    SynthesiserSound*, int /*currentPitchWheelPosition*/) override
//    {
//        // Start ADSR envelope
//        adsr.noteOn();
//
//        // Enable sound playback
//        play_sound = true;
//
//        // Reset sample playback to the beginning
//        mHarmonicsHead = 0;
//
//        // Velocity affects the gain
//        level = velocity * 0.15;
//        mAttackGainSmoothed = 0.0;
//        mDecayGainSmoothed = 1.0;
//
//        last_freqs.resize(0);
//        last_freqs = mSound[1]->harmonic_frequencies[0];
//        phase_morph.resize(0);
//        phase_morph.resize(mSound[1]->harmonic_frequencies[0].size(), 0.0);
//        std::generate(phase_morph.begin(), phase_morph.end(), RandomGenerator(0.0, 0.0));
//
//        for (int i=0; i<phase_morph.size(); i++) phase_morph[i] = 2 * M_PI * phase_morph[i];
//
//        cycles_per_second = MidiMessage::getMidiNoteInHertz(midiNoteNumber);
//        cycles_per_sample = cycles_per_second / getSampleRate();
//    }
//
//    void stopNote(float /*velocity*/, bool allowTailOff) override
//    {
//        // Start ADSR envelope
//        adsr.noteOff();
//
//        // Disable sound playback
//        play_sound = false;
//        clearCurrentNote();
//    }
    
//    void pitchWheelMoved (int /*newValue*/) override                              {}
//    void controllerMoved (int /*controllerNumber*/, int /*newValue*/) override    {}
    
    void renderNextBlock (AudioBuffer<float>& outputBuffer, int startSample, int numSamples) override
    {
        // TODO - Manage current_midi_note on this class
        
        for (auto i_channel = outputBuffer.getNumChannels(); --i_channel >= 0;)
        {
//            mAttackGainSmoothed = mAttackGainSmoothed - 0.004 * (mAttackGainSmoothed - 1.0);
//
//            // TODO - This operation can be done before
//            int min_frame = int( std::min( mSound[1]->harmonic_frequencies.size(), mSound[2]->harmonic_frequencies.size() ) );
//
//            if ( (min_frame - mHarmonicsHead) < 12)
//            {
//                if (mDecayGainSmoothed <= 0.1) mDecayGainSmoothed = 0.0;
//                else mDecayGainSmoothed = mDecayGainSmoothed - 0.1;
//            }
//
//            auto current_sample = mCircularBufferLeft[selected_play_samples[i]] * adsr.getNextSample() * mAttackGainSmoothed * mDecayGainSmoothed * level;
            outputBuffer.addSample (i_channel, startSample, 0.0);
        }
    }
    
private:
    
    AudioProcessorValueTreeState* mParameters;
    
    // Circular Buffers
    float* mCircularBufferLeft;
    float* mCircularBufferRight;
        
    ADSR adsr;
    ADSR::Parameters adsrParams;
};
