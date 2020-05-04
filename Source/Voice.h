/*
  ==============================================================================

    Voice.h
    Created: 28 Apr 2020 12:54:16pm
    Author:  Marc Sanchez Martinez

  ==============================================================================
*/

#pragma once

#include "Instrument.h"
#include "Synthesis.h"

#include "MorphSound.h"
#include "SMTAudioHelpers.h"
#include "SMTHelperFunctions.h"

#include "SynthesisEngine.h"

#include "SMTParameters.h"

using namespace Core;

struct Voice
:   public SynthesiserVoice
{
    enum StateADSR
    {
        Attack = 0,
        Decay,
        Sustain,
        Release
    } adsr;
    
    // Note playback
    bool playing_note;
    bool loop_mode;
    
//    Voice(Instrument instrument, AudioProcessorValueTreeState* parameters)
    Voice(Instrument& instrument, AudioProcessorValueTreeState* parameters)
    : instrument(instrument)
    {
//        // Add a reference to the parent instrument
//        this->instrument = &instrument;
        
        // Initialize the synthesis engine
        this->synthesis = Synthesis();
        
        // Initialize the synthesis engine
        mSynthesis = std::make_unique<SynthesisEngine>();
        
        // Default values
        this->f_current_midi_note = 0;
        this->f_current_velocity = 0;
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
    
    void startNote( int midiNoteNumber, float velocity,
                   SynthesiserSound*, int /*currentPitchWheelPosition*/) override
    {
        // Reset synthesis engine
        this->synthesis.reset();
        
        // Note playback
        this->playing_note = true;
        this->loop_mode = false;
//        this->loop_mode = true;
        this->adsr = StateADSR::Attack;
        
        this->f_current_midi_note = (float)midiNoteNumber;
        this->f_current_velocity = velocity;
        
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
    }
    
    void stopNote(float /*velocity*/, bool allowTailOff = true) override
    {
        // TODO - Check behaviour, this method is called when presing a note (reset?)
        if (this->playing_note)
        {
            this->adsr = StateADSR::Release;
            if (!allowTailOff) this->playing_note = false;
            
            // TODO - Test
            Tools::Audio::writeSoundFile(this->synthesis.generated.y);
//            Tools::Audio::writeSoundFile(this->synthesis.generated.harmonics_freqs, "/Users/Marc/Documents/Audio Plugins/Morphex/Tests/harmonics_freqs.wav");
//            Tools::Audio::writeSoundFile(this->synthesis.buffer.channels[Channel::Mono]);
        }
        
        
        //        // Start ADSR envelope
        //        adsr.noteOff();
        //
        //        // Disable sound playback
        //        play_sound = false;
        //        clearCurrentNote();
    }
    
    // TODO - Check if needed
    void setADSRSampleRate(double sampleRate)
    {
        bool trmv = false;
        //        adsr.setSampleRate(sampleRate);
    }
    
    //    // TODO - Check if needed
    void updateAdsrParams(float attack, float decay, float sustain, float release)
    {
        //        adsrParams.attack = attack;
        //        adsrParams.decay = decay;
        //        adsrParams.sustain = sustain;
        //        adsrParams.release = release;
        //
        //        // Update ADSR parameters
        //        adsr.setParameters(adsrParams);
    }
    
    //    void reset()
    //    {
    //        // Clean the buffers
    //        zeromem(mCircularBufferLeft, sizeof(float) * MORPH_CIRCULAR_BUFFER_LENGTH);
    //        zeromem(mCircularBufferRight, sizeof(float) * MORPH_CIRCULAR_BUFFER_LENGTH);
    //    }
    
    
    
    void pitchWheelMoved (int /*newValue*/) override
    {
        int semitones_range = 12;
        
        // TODO - Map range -12:12 semitones to 0:127
    }
    
    void controllerMoved (int /*controllerNumber*/, int /*newValue*/) override    {}
    
    void renderNextBlock (AudioBuffer<float>& outputBuffer, int startSample, int numSamples) override
    {
        // TODO - Manage f_current_midi_note on this class
        
        if (this->playing_note)
        {
//            float* sound_frame = getNextFrame(this->f_current_midi_note, this->f_current_velocity, numSamples);
//            float* sound_frame = getNextFrame(this->f_current_midi_note, this->f_current_velocity, numSamples);
            std::vector<float> sound_frame = getNextFrame(this->f_current_midi_note, this->f_current_velocity, numSamples);

//            float* sound_data = sound_frame.data();
//            float* sound_data[1] = {sound_frame.data()};

            for (int i_sample = 0; i_sample < numSamples; i_sample++)
            {
                float f_current_sample = sound_frame[i_sample];
                
//                auto current_sample = sound_data[i_sample];
                
                // Test
                this->synthesis.generated.y.push_back( f_current_sample );
                
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
                    
                    outputBuffer.addSample (i_channel, startSample, f_current_sample);
//                    outputBuffer.addSample (i_channel, startSample, current_sample);
                }
            }
            DBG("outputBuffer ready");
        }
    }
    
//    std::vector<float> 'getNextFrame'(float f_note, float f_velocity, int i_frame_length, float f_interpolation_factor = -1)
//    float* getNextFrame(float f_note, float f_velocity, int i_frame_length, float f_interpolation_factor = -1)
    std::vector<float> getNextFrame(float f_note, float f_velocity, int i_frame_length, float f_interpolation_factor = -1)
    {
        // If a note is being played
        if (this->playing_note)
        {
            // Synthesis parameters shortcuts
            const int i_fft_size = this->synthesis.parameters.fft_size;
            const int i_hop_size = this->synthesis.parameters.hop_size;
            int* i_current_frame = &this->synthesis.live_values.i_current_frame;
            
            // Output
            std::vector<float> frame;
            
//            // Test
////            int H = 128;
////            this->live_values.i_samples_ready += H;
////            this->synthesis->updateWritePointer(H);
//            
//            Core::MorphSounds morph_sounds = this->instrument.getCloserSounds( f_note, f_velocity );
//            
//            // Get the minimum length of both notes
//            int min_note_end = std::min(morph_sounds[MorphLocation::Left]->max_frames,
//                                        morph_sounds[MorphLocation::Right]->max_frames);
//            
//            // If we are on the last frame of the shortest note
//            if (*i_current_frame >= min_note_end)
//            {
//                // End note playback
//                this->playing_note = false;
//                this->synthesis.live_values.last_frame = true;
//            }
//            
//            Sound::Frame sound_frame = morph_sounds[MorphLocation::Left]->getFrame(*i_current_frame, i_hop_size);
//            
//            *i_current_frame += 1;
//            
////            frame = this->synthesis.generateSoundFrame(sound_frame, i_fft_size);
//            
//            // A list with the indexes of the harmonics we want to interpolate
//
////            return frame;

            
            
            
            
            
            while (this->synthesis.live_values.i_samples_ready < i_frame_length)
            {
                Core::MorphSounds morph_sounds = this->instrument.getCloserSounds( f_note, f_velocity );

                // If ADSR is NOT on "Release" state
                if (this->adsr != StateADSR::Release)
                {
                    // Compute common looping regions
                    int max_loop_start = std::max(morph_sounds[MorphLocation::Left]->loop.start,
                                                  morph_sounds[MorphLocation::Right]->loop.start);

                    int min_loop_end = std::min(morph_sounds[MorphLocation::Left]->loop.end,
                                                morph_sounds[MorphLocation::Right]->loop.end);

                    // If loop mode is enabled
//                    if (this->loop_mode)
                    if (this->loop_mode)
                    {
                        // Keep the current_frame pointer inside the lowest "note.loop.end"
                        // and the highest "note.loop.start"
                        if ( (*i_current_frame * i_hop_size) >= min_loop_end )
                        {
                            this->adsr = StateADSR::Sustain;
                            *i_current_frame = int( max_loop_start / i_hop_size );
                        }
                    }
                    // If "loop_mode" is NOT activated
                    else
                    {
                        // If we are beyond the end loop point
                        if (*i_current_frame >= min_loop_end)
                        {
                            // Switch ADSR to "Relase" mode
                            this->adsr = StateADSR::Release;
                        }
                    }
                }

                // If ADSR is on "Release" state
                if (this->adsr == StateADSR::Release)
                {
                    // TODO - JUMP CURRENT FRAME TO RELEASE SECTION, if defined of course,
                    // if not, play the rest of the other morph_sounds or shorten the whole thing
                    // to the shortest one and apply a fade out on hramonics_mags 4 frames before the end

                    // Get the minimum length of both notes
                    int min_note_end = std::min(morph_sounds[MorphLocation::Left]->max_frames,
                                                morph_sounds[MorphLocation::Right]->max_frames);

                    // If we are on the last frame of the shortest note
                    if (*i_current_frame >= min_note_end)
                    {
                        // End note playback
                        this->playing_note = false;
                        this->synthesis.live_values.last_frame = true;
                    }
                }

                Sound::Frame sound_frame;

//                if (morph_sounds[MorphLocation::Left] == morph_sounds[MorphLocation::Right])
                if (true)
                {
                    sound_frame = morph_sounds[MorphLocation::Left]->getFrame(*i_current_frame, i_hop_size);

//                    // Get target frequency
//                    float f_target_frequency = Tools::Midi::toFreq(f_note);
//
//                    // Transpose left note frequencies to the target frequency
//                    Tools::Calculate::divideByScalar(sound_frame.harmonics_freqs,
//                                                     Tools::Midi::toFreq(morph_sounds[MorphLocation::Left]->note));
//                    Tools::Calculate::multiplyByScalar(sound_frame.harmonics_freqs, f_target_frequency);
                }
                else
                {
                     sound_frame = this->instrument.morphSoundFrames(f_note, morph_sounds, *i_current_frame, i_frame_length);
                    //                    sound_frame = morphSoundFrames(f_note, morph_sounds, *i_current_frame, i_hop_size, f_interpolation_factor);
                }


                // TODO - TEST
//                frame = std::vector<float>(i_frame_length, 0.0);
//                this->synthesis.live_values.i_samples_ready += 128;
//                *i_current_frame += 1;
//                continue;

                // NOTE - "frame" will have "i_hop_size" more samples ready
                // to be played after each call
                frame = this->synthesis.generateSoundFrame(sound_frame, i_fft_size);

//                // TODO - TEST
//                this->synthesis.live_values.i_samples_ready += 128;
//                frame = std::vector<float>(i_frame_length, 0.0);

                // std::vector<float> generated_sound = this->synthesis.generateSoundFrame(sound_frame, i_frame_length);
                // generated_sound = self.synthesis.generateSoundFrame(sound, hop_size)

                *i_current_frame += 1;
            }

            // Update samples ready to be played
            this->synthesis.live_values.i_samples_ready -= frame.size();

//            DBG("\n* * * * FRAME DELIVERED * * * *\n");

            return frame;
        }
        // If note is NOT being played
        else
        {
//            return new float[512];
            return std::vector<float>(i_frame_length, 0.0);
        }
    }
    
private:
    
    AudioProcessorValueTreeState* mParameters;
    
    // Instrument
    Core::Instrument& instrument;
    
    // Synthesis
    Core::Synthesis synthesis;
    
    // Synthesis
    std::unique_ptr<SynthesisEngine> mSynthesis;
    
    // Midi
    float f_current_midi_note;
    int f_current_velocity;
};
