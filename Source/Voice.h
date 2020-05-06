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
    
//    // Note playback
//    bool playing_note;
//    bool loop_mode;
    
//    Voice(Instrument instrument, AudioProcessorValueTreeState* parameters)
    Voice(Instrument& instrument, AudioProcessorValueTreeState* parameters, int voice_ID)
    : instrument(instrument), voice_ID(voice_ID)
    {
        DBG(String(voice_ID) + " - VOICE INIT");
        
        // Initialize the synthesis engine
        this->synthesis = Synthesis();
        
        // Default values
        this->adsr = StateADSR::Attack;
        this->f_current_midi_note = 0;
        this->f_current_velocity = 0;
        this->f_last_midi_note = 0;
        
        // Note playback
        this->playing_note = false;
        this->loop_mode = true;
        this->hold_note = false;
        this->track_velocity = false; // High CPU usage
        
        // Sounds
        this->max_loop_start = 0;
        this->min_loop_end = 0;
        this->min_note_end = 0;
        
        this->test_tone.active = false;
        
        this->frame.resize(0);
        
        // Sounds
        morph_sounds = this->instrument.getCloserSounds( this->f_current_midi_note, this->f_current_velocity );
        
        // Compute common looping regions
        this->max_loop_start = std::max(morph_sounds[MorphLocation::Left]->loop.start,
                                        morph_sounds[MorphLocation::Right]->loop.start);
        
        this->min_loop_end = std::min(morph_sounds[MorphLocation::Left]->loop.end,
                                      morph_sounds[MorphLocation::Right]->loop.end);
        
        // Get the minimum length of both notes
        this->min_note_end = std::min(morph_sounds[MorphLocation::Left]->max_frames,
                                      morph_sounds[MorphLocation::Right]->max_frames);
    }
    
    bool canPlaySound (SynthesiserSound* synthSound) override
    {
        DBG(String(voice_ID) + " - canPlaySound");
//        return true;
        return dynamic_cast<MorphSound*> (synthSound) != nullptr;
    }
    
    void startNote( int midiNoteNumber, float velocity,
                   SynthesiserSound*, int /*currentPitchWheelPosition*/) override
    {
        DBG(String(voice_ID) + " - startNote");
//        // Reset synthesis engine
//        this->synthesis.reset();
        
        // Note playback
        this->adsr = StateADSR::Attack;
        this->playing_note = true;
        
        this->f_current_midi_note = (float)midiNoteNumber;
        this->f_current_velocity = velocity;
        this->f_last_midi_note = this->f_current_midi_note;
        
//        // Sounds
//        morph_sounds = this->instrument.getCloserSounds( this->f_current_midi_note, this->f_current_velocity );
//
//        // Compute common looping regions
//        this->max_loop_start = std::max(morph_sounds[MorphLocation::Left]->loop.start,
//                                        morph_sounds[MorphLocation::Right]->loop.start);
//
//        this->min_loop_end = std::min(morph_sounds[MorphLocation::Left]->loop.end,
//                                      morph_sounds[MorphLocation::Right]->loop.end);
//
//        // Get the minimum length of both notes
//        this->min_note_end = std::min(morph_sounds[MorphLocation::Left]->max_frames,
//                                      morph_sounds[MorphLocation::Right]->max_frames);
        
        // Velocity affects the gain
        this->level = velocity * 0.15;
        this->mAttackGainSmoothed = 0.0;
        this->mDecayGainSmoothed = 1.0;
        this->tailOff = 1.0;

        if (this->test_tone.active)
        {
            this->test_tone.currentAngle = 0.0;
            this->test_tone.level = velocity * 0.15;
            this->test_tone.tailOff = 0.0;
            
            auto cyclesPerSecond = MidiMessage::getMidiNoteInHertz (midiNoteNumber);
            auto cyclesPerSample = cyclesPerSecond / getSampleRate();
            
            this->test_tone.angleDelta = cyclesPerSample * MathConstants<double>::twoPi;
        }
    }
    
    void stopNote(float velocity, bool allowTailOff) override
    {
        DBG(String(voice_ID) + " - stopNote");
        
        allowTailOff = false;
        
        if (allowTailOff)
        {
            // start a tail-off by setting this flag. The render callback will pick up on
            // this and do a fade out, calling clearCurrentNote() when it's finished.
            
            // Release ADSR envelope
//            this->adsr = StateADSR::Release;
            
//            if (tailOff == 0.0) // we only need to begin a tail-off if it's not already doing so - the
//                tailOff = 1.0;  // stopNote method could be called more than once.
            
            if (this->test_tone.active)
            {
                if (this->test_tone.tailOff == 0.0) // we only need to begin a tail-off if it's not already doing so - the
                    this->test_tone.tailOff = 1.0;  // stopNote method could be called more than once.
            }
            else
            {
                if (this->tailOff == 1.0) // we only need to begin a tail-off if it's not already doing so - the
                    this->tailOff *= 0.999;  // stopNote method could be called more than once.
            }
        }
        else
        {
            clearAndResetCurrentNote();
            
            if (this->test_tone.active)
            {
                this->test_tone.angleDelta = 0.0;
            }
            else
            {
                
//                this->synthesis.live_values.i_current_frame = 0;
//                this->synthesis.live_values.i_samples_ready = 0;
                this->playing_note = false;
            }
            
            // we're being told to stop playing immediately, so reset everything..
            
        }
        
//        if (velocity == 0)
//        {
//            this->synthesis.reset();
//            clearCurrentNote();
//        }
        
        // TODO TEST - Temporal override
//        allowTailOff = false;
        
        
//        if (!allowTailOff)
//        {
//            this->playing_note = false;
//            clearCurrentNote();
//        }
    }
    
    void setADSRSampleRate(double sampleRate)
    {
        //        adsr.setSampleRate(sampleRate);
    }
    
    void updateAdsrParams(float attack, float decay, float sustain, float release)
    {
        //        adsrParams.attack = attack;
        //        adsrParams.decay = decay;
        //        adsrParams.sustain = sustain;
        //        adsrParams.release = release;
        
        // Update ADSR parameters
        //        adsr.setParameters(adsrParams);
    }
    
    //    void reset()
    //    {
    //        // Clean the buffers
    //        zeromem(mCircularBufferLeft, sizeof(float) * MORPH_CIRCULAR_BUFFER_LENGTH);
    //        zeromem(mCircularBufferRight, sizeof(float) * MORPH_CIRCULAR_BUFFER_LENGTH);
    //    }
    
    void clearAndResetCurrentNote()
    {
//        SynthesiserVoice::clearCurrentNote();
        this->clearCurrentNote();
        this->frame.resize(0);
        this->synthesis.reset();
    }
    
    void pitchWheelMoved (int /*newValue*/) override
    {
        int semitones_range = 12;
        
        // TODO - Map range -12:12 semitones to 0:127
    }
    
    void controllerMoved (int /*controllerNumber*/, int /*newValue*/) override    {}
    
    void renderNextBlock (AudioBuffer<float>& outputBuffer, int startSample, int numSamples) override
    {
        if (this->playing_note)
        {
            if (numSamples != 512)
            {
                DBG("numSamples = " + String(numSamples));
                DBG("CHECK THIS!");
            }
            
            std::vector<float> sound_frame = getNextFrame(this->f_current_midi_note, this->f_current_velocity, numSamples);
            //                std::vector<float> sound_frame = getNextFrame(this->f_current_midi_note, this->f_current_velocity, 512);
            
            //                while (--numSamples >= 0)
            //            for (int i_sample = 0; i_sample < 512; i_sample++)
            for (int i_sample = 0; i_sample < numSamples; i_sample++)
            {
                auto currentSample = (float) (sound_frame[i_sample] * this->level * this->tailOff);
                //                    auto currentSample = (float) (std::sin (this->currentAngle) * this->level * this->tailOff);
                
                // TODO TEST
                this->synthesis.generated.y.push_back( currentSample );
                
                if (!this->test_tone.active)
                {
                    for (auto i = outputBuffer.getNumChannels(); --i >= 0;)
                        outputBuffer.addSample (i, startSample, currentSample);
                    
                    ++startSample;
                }
                
                if (this->tailOff < 1.0)
                {
                    this->tailOff *= 0.99999;
                    
                    if (this->tailOff <= 0.005)
                    {
                        this->playing_note = false;
                        break;
                    }
                }
            }
            
            if (!this->playing_note)
            {
                clearAndResetCurrentNote();
            }
        }
        
        if (this->test_tone.active)
        {
            if (this->test_tone.angleDelta != 0.0)
            {
                if (this->test_tone.tailOff > 0.0)
                {
                    while (--numSamples >= 0)
                    {
                        auto currentSample = (float) (std::sin (this->test_tone.currentAngle) * this->test_tone.level * this->test_tone.tailOff);

                        for (auto i = outputBuffer.getNumChannels(); --i >= 0;)
                            outputBuffer.addSample (i, startSample, currentSample);

                        this->test_tone.currentAngle += this->test_tone.angleDelta;
                        ++startSample;

                        this->test_tone.tailOff *= 0.99;

                        if (this->test_tone.tailOff <= 0.005)
                        {
                            clearCurrentNote();

                            this->test_tone.angleDelta = 0.0;
                            break;
                        }
                    }
                }
                else
                {
                    while (--numSamples >= 0)
                    {
                        auto currentSample = (float) (std::sin (this->test_tone.currentAngle) * this->test_tone.level);

                        for (auto i = outputBuffer.getNumChannels(); --i >= 0;)
                            outputBuffer.addSample (i, startSample, currentSample);

                        this->test_tone.currentAngle += this->test_tone.angleDelta;
                        ++startSample;
                    }
                }
            }
        }
        
        
        
//        // TODO - Lock 512 buffer size support
////        if (this->playing_note && numSamples == 512)
//        if (this->playing_note)
//        {
//            std::vector<float> sound_frame = getNextFrame(this->f_current_midi_note, this->f_current_velocity, 512);
////            std::vector<float> sound_frame = getNextFrame(this->f_current_midi_note, this->f_current_velocity, numSamples);
//
//            if (!this->test_tone.active)
//            {
//                for (int i_sample = 0; i_sample < numSamples; i_sample++)
//                {
//                    float f_current_sample = sound_frame[i_sample];
//
//                    mAttackGainSmoothed = mAttackGainSmoothed - 0.004 * (mAttackGainSmoothed - 1.0);
//
//                    //                // Test
//                    //                this->synthesis.generated.y.push_back( f_current_sample );
//
//                    for (auto i_channel = outputBuffer.getNumChannels(); --i_channel >= 0;)
//                    {
//                        //            // TODO - This operation can be done before
//                        //            int min_frame = int( std::min( mSound[1]->harmonic_frequencies.size(), mSound[2]->harmonic_frequencies.size() ) );
//                        //
//                        //            if ( (min_frame - mHarmonicsHead) < 12)
//                        //            {
//                        //                if (mDecayGainSmoothed <= 0.1) mDecayGainSmoothed = 0.0;
//                        //                else mDecayGainSmoothed = mDecayGainSmoothed - 0.1;
//                        //            }
//                        //
//
//                        auto current_sample = f_current_sample * mAttackGainSmoothed * mDecayGainSmoothed * level;
//                        //                    auto current_sample = f_current_sample * adsr.getNextSample() * mAttackGainSmoothed * mDecayGainSmoothed * level;
//
//                        outputBuffer.addSample (i_channel, startSample, current_sample);
//                    }
//
//                    ++startSample;
//                }
//            }
//
//            if (!this->playing_note)
//            {
//                clearCurrentNote();
////                break;
//            }
//        }
    }
    
    std::vector<float> getNextFrame(float f_note, float f_velocity, int i_frame_length, float f_interpolation_factor = -1)
    {
//        // If a note is being played
//        if (this->playing_note)
//        {
        // Synthesis parameters shortcuts
        const int i_fft_size = this->synthesis.parameters.fft_size;
        const int i_hop_size = this->synthesis.parameters.hop_size;
        int* i_current_frame = &this->synthesis.live_values.i_current_frame;
        
        // If "track_velocity" is enabled
        if (this->track_velocity)
        {
            // If current note has changed
            if (this->f_current_midi_note != this->f_last_midi_note)
            {
                // Update morph samples
                morph_sounds = this->instrument.getCloserSounds( f_current_midi_note, f_current_velocity );
                
                // Compute common looping regions
                this->max_loop_start = std::max(morph_sounds[MorphLocation::Left]->loop.start,
                                                morph_sounds[MorphLocation::Right]->loop.start);
                
                this->min_loop_end = std::min(morph_sounds[MorphLocation::Left]->loop.end,
                                              morph_sounds[MorphLocation::Right]->loop.end);
                
                // Get the minimum length of both notes
                this->min_note_end = std::min(morph_sounds[MorphLocation::Left]->max_frames,
                                              morph_sounds[MorphLocation::Right]->max_frames);
                
                this->f_last_midi_note = this->f_current_midi_note;
            }
        }
        
        while (this->synthesis.live_values.i_samples_ready < i_frame_length)
        {
//                float adsr_attack = *mParameters->getRawParameterValue(SMTParameterID[kParameter_asdr_attack]);
//                float adsr_decay = *mParameters->getRawParameterValue(SMTParameterID[kParameter_asdr_decay]);
//                float adsr_sustain = *mParameters->getRawParameterValue(SMTParameterID[kParameter_asdr_sustain]);
//                float adsr_release = *mParameters->getRawParameterValue(SMTParameterID[kParameter_asdr_release]);
//
//                // Update ADSR parameters
//                updateAdsrParams(adsr_attack, adsr_decay, adsr_sustain, adsr_release);
            
//            int i_current_sample = *i_current_frame * i_hop_size;
//
//            // If ADSR is NOT on "Release" state
//            if (this->adsr != StateADSR::Release)
//            {
//                if (this->adsr != StateADSR::Sustain and
//                    ( i_current_sample >= this->max_loop_start ) )
//                {
//                    this->adsr = StateADSR::Sustain;
//                }
//
//                // If loop mode is enabled
//                if (this->loop_mode)
//                {
//                    // Keep the current_frame pointer inside the lowest "note.loop.end"
//                    // and the highest "note.loop.start"
//                    if ( i_current_sample >= this->min_loop_end )
//                    {
//                        *i_current_frame = int( this->max_loop_start / i_hop_size );
//                    }
//                }
//                // If "loop_mode" is NOT activated
//                else
//                {
//                    // If we are beyond the end loop point
//                    if ( i_current_sample >= this->min_loop_end)
//                    {
//                        // Switch ADSR to "Relase" mode
//                        this->adsr = StateADSR::Release;
//                    }
//                }
//            }
//
//            // If ADSR is on "Release" state
//            if (this->adsr == StateADSR::Release)
//            {
//                // TODO - JUMP CURRENT FRAME TO RELEASE SECTION, if defined of course,
//                // if not, play the rest of the other morph_sounds or shorten the whole thing
//                // to the shortest one and apply a fade out on hramonics_mags 4 frames before the end
//
//                if ( ( i_current_sample < this->min_loop_end ) or this->hold_note )
//                {
//                    *i_current_frame = int( this->min_loop_end / i_hop_size );
//                }
//            }
//
            // If we are on the last frame of the shortest note
            if ( *i_current_frame >= this->min_note_end-1)
            {
                // End note playback
                this->playing_note = false;
                this->synthesis.live_values.last_frame = true;
            }

            Sound::Frame sound_frame;

            if (morph_sounds[MorphLocation::Left] == morph_sounds[MorphLocation::Right])
            {
//                    sound_frame = morph_sounds[MorphLocation::Left]->getFrame(*i_current_frame, i_hop_size);

                if ( *i_current_frame >= this->min_note_end-1)
                {
                    sound_frame.harmonics_freqs = std::vector<float>(0);
                    sound_frame.harmonics_mags = std::vector<float>(0);
                }
                else{
                    sound_frame.harmonics_freqs = morph_sounds[Core::MorphLocation::Left]->model->values.harmonics_freqs[*i_current_frame];
                    sound_frame.harmonics_mags = morph_sounds[Core::MorphLocation::Left]->model->values.harmonics_mags[*i_current_frame];
                }
                sound_frame.harmonics_phases = std::vector<float>(0);
                sound_frame.stochastic = std::vector<float>(0);
                sound_frame.residual = std::vector<float>(0);

                // Get target frequency
//                float f_target_frequency = 440.0;
                float f_target_frequency = Tools::Midi::toFreq(this->f_current_midi_note);

////                    float f_target_frequency = Tools::Midi::toFreq(f_note);
                
                // Recalculate the harmonics for the current midi note
                for (int i=0; i<sound_frame.harmonics_freqs.size(); i++)
                {
                    sound_frame.harmonics_freqs[i] = (sound_frame.harmonics_freqs[i] / Tools::Midi::toFreq(morph_sounds[MorphLocation::Left]->note)) * f_target_frequency;
                }
//
//                    // Transpose left note frequencies to the target frequency
//                    Tools::Calculate::divideByScalar(sound_frame.harmonics_freqs,
//                                                     Tools::Midi::toFreq(morph_sounds[MorphLocation::Left]->note));
//                    Tools::Calculate::multiplyByScalar(sound_frame.harmonics_freqs, f_target_frequency);
            }
            else
            {
//                    sound_frame = this->instrument.morphSoundFrames(this->f_current_midi_note, morph_sounds, *i_current_frame, i_frame_length);
//                    sound_frame = this->instrument.morphSoundFrames(f_note, morph_sounds, *i_current_frame, i_frame_length);
                //                    sound_frame = morphSoundFrames(f_note, morph_sounds, *i_current_frame, i_hop_size, f_interpolation_factor);
            }


            // TODO - TEST
//                frame = std::vector<float>(i_frame_length, 0.0);
//                this->synthesis.live_values.i_samples_ready += 128;
//                *i_current_frame += 1;
//                continue;

            // NOTE - "frame" will have "i_hop_size" more samples ready to be played after each call
            // TODO - This function needs to be optimized
            this->frame = this->synthesis.generateSoundFrame(sound_frame, i_fft_size);

            *i_current_frame += 1;
        }

        // Update samples ready to be played
        this->synthesis.live_values.i_samples_ready -= this->frame.size();

        return this->frame;
//        }
//        // If note is NOT being played
//        else
//        {
//            return std::vector<float>(i_frame_length, 0.0);
//        }
    }
    
private:
    
    // TODO TEST
    const int voice_ID;
    
    AudioProcessorValueTreeState* mParameters;
    Instrument& instrument;
    Synthesis synthesis;
    
    std::vector<float> frame;
    
    // Midi
    float f_current_midi_note;
    int f_current_velocity;
    float f_last_midi_note;
    
    // Note playback
    bool playing_note;
    bool loop_mode;
    bool hold_note;
    bool track_velocity;
    
    // Sounds
    Core::MorphSounds morph_sounds;
    int max_loop_start;
    int min_loop_end;
    int min_note_end;
    
    double level = 0.0;
    double mAttackGainSmoothed = 0.0;
    double mDecayGainSmoothed = 0.0;
    double tailOff = 1.0;
    
    // TODO TEST
    struct TestTone
    {
        bool active = false;
        double currentAngle = 0.0;
        double angleDelta = 0.0;
        double level = 0.0;
        double tailOff = 0.0;
    } test_tone;
};
