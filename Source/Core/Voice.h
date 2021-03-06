/* Copyright (C) 2020 Marc Sanchez Martinez
 *
 * https://github.com/MarcSM/morphex
 *
 * Morphex is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Morphex is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Morphex. If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include "Instrument.h"
#include "Synthesis.h"

#include "../Entities/MorphSound.h"
#include "../Helpers/SMTAudioHelpers.h"
#include "../Helpers/SMTHelperFunctions.h"
#include "../Helpers/SMTParameters.h"

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
    };
    StateADSR adsr;
    
    Voice (Instrument* instrument, AudioProcessorValueTreeState* parameters)
    :   mParameters (parameters),
        instrument (instrument),
        synthesis (instrument)
    {
        this->reset();
    }
    
    void reset()
    {
        // Note playback
        this->adsr = StateADSR::Attack;
        this->playing_note = false;
        this->loop_mode = true;
        this->hold_note = false;
        this->track_velocity = false; // NOTE - High CPU usage if true
        this->allow_pitch_wheel = true;
        
        // Default values
        this->f_pressed_midi_note = 0;
        this->f_current_midi_note = 0;
        this->f_current_velocity = 0;
        this->f_last_midi_note = 0;
        
        // Sounds
        this->max_loop_start = 0;
        this->min_loop_end = 0;
        this->min_note_end = 0;
    }
    
    bool canPlaySound (SynthesiserSound* synthSound) override
    {
        return dynamic_cast<MorphSound*> (synthSound) != nullptr;
    }
    
    void startNote (int midiNoteNumber, float velocity, SynthesiserSound*, int currentPitchWheelPosition) override
    {
//        // Note playback
//        this->adsr = StateADSR::Attack;
        this->playing_note = true;
        
        this->f_pressed_midi_note = (float) midiNoteNumber;
        this->f_current_midi_note = (float) midiNoteNumber;
        this->f_last_midi_note = (float) midiNoteNumber;
        this->f_current_velocity = velocity;
        
        this->updateMorphSounds (this->f_pressed_midi_note, this->f_current_velocity);
        
        this->updateMidiNoteWithPitchWheel (currentPitchWheelPosition, true);
        
        // Velocity affects the gain
        this->level = velocity * 0.15;
        this->mAttackGainSmoothed = 0.0;
        this->mDecayGainSmoothed = 1.0;
        this->tailOff = 1.0;
    }
    
    void stopNote (float velocity, bool allowTailOff) override
    {
        if (allowTailOff)
        {
            if (this->tailOff == 1.0) this->tailOff *= 0.999;
        }
        else
        {
            clearAndResetCurrentNote();
            this->playing_note = false;
        }
    }
    
    void pitchWheelMoved (int newValue) override
    {
        this->updateMidiNoteWithPitchWheel (newValue);
    }
    
    void updateMidiNoteWithPitchWheel (int newValue, bool set_current = false)
    {
        // Map range 0-16383 to -12:12 semitones
        //        this->f_current_midi_pitch_wheel = jmap((float)newValue, 0.0f, 16383.0f,
        
        // TODO - Custom "semitones_range"
        //        float semitones_range = 12.0;
        
        if (this->allow_pitch_wheel)
        {
            float f_new_midi_pitch_wheel = jmap ((float) newValue, 0.0f, 16383.0f,
                                                 -this->f_pitch_wheel_range_semitones,
                                                 this->f_pitch_wheel_range_semitones);
            
            if (set_current)
            {
                this->f_current_midi_pitch_wheel.setCurrentAndTargetValue (f_new_midi_pitch_wheel);
            }
            else
            {
                this->f_current_midi_pitch_wheel.setTargetValue (f_new_midi_pitch_wheel);
            }
        }
    }
    
    void controllerMoved (int /*controllerNumber*/, int /*newValue*/) override {}
    
    // TODO
    void setADSRSampleRate (double sampleRate)
    {
//        adsr.setSampleRate(sampleRate);
    }
    
    // TODO
    void updateAdsrParams (float attack, float decay, float sustain, float release)
    {
//        adsrParams.attack = attack;
//        adsrParams.decay = decay;
//        adsrParams.sustain = sustain;
//        adsrParams.release = release;
//
//        // Update ADSR parameters
//        adsr.setParameters(adsrParams);
    }
    
    void clearAndResetCurrentNote()
    {
        this->clearCurrentNote();
        this->synthesis.reset();
        this->reset();
    }
    
    void updateMorphSounds (float f_note, float f_velocity)
    {
        // Instrument::Mode::Morphing
        if (this->instrument->mode == Instrument::Mode::Morphing)
        {
            this->morph_sounds = this->instrument->getMorphSounds();
        }
        // Instrument::Mode::FullRange
        else
        {
            this->morph_sounds = this->instrument->getCloserSounds (f_note, f_velocity);
        }
        
        bool first_iter = true;
        
        for (int i = 0; i < this->morph_sounds.size(); i++)
        {
            if (this->morph_sounds[i] != nullptr)
            {
                if (first_iter)
                {
                    this->max_loop_start = this->morph_sounds[i]->loop.start;
                    this->min_loop_end = this->morph_sounds[i]->loop.end;
                    this->min_note_end = this->morph_sounds[i]->max_frames;
                    
                    first_iter = false;
                }
                else
                {
                    // Compute common looping regions
                    this->max_loop_start = std::max (this->max_loop_start, this->morph_sounds[i]->loop.start);
                    this->min_loop_end = std::min (this->min_loop_end, this->morph_sounds[i]->loop.end);
                    
                    // Get the minimum length of both notes
                    this->min_note_end = std::min (this->min_note_end, this->morph_sounds[i]->max_frames);
                }
            }
        }
        
        this->f_last_midi_note = f_note;
    }
    
    void renderNextBlock (AudioBuffer<float>& outputBuffer, int startSample, int numSamples) override
    {
        if (this->playing_note)
        {
            if (this->allow_pitch_wheel) this->f_current_midi_note = this->f_pressed_midi_note + this->f_current_midi_pitch_wheel.getNextValue();
            
            std::vector<float> frame = getNextFrame (this->f_current_midi_note, this->f_current_velocity, numSamples);
            
            for (int i_sample = 0; i_sample < numSamples; i_sample++)
            {
                auto currentSample = (float) (frame[i_sample] * this->level * this->tailOff);
//                auto currentSample = (float) (std::sin (this->currentAngle) * this->level * this->tailOff);
                
                for (auto i = outputBuffer.getNumChannels(); --i >= 0;)
                {
                    outputBuffer.addSample (i, startSample, currentSample);
                }
                
                ++startSample;
                
                if (this->tailOff < 1.0)
                {
                    this->tailOff *= 0.999;
                    
                    if (this->tailOff <= 0.005)
                    {
                        this->playing_note = false;
                    }
                }
            }
            
            if (!this->playing_note)
            {
                clearAndResetCurrentNote();
            }
        }
    }
    
    std::vector<float> getNextFrame (float f_note, float f_velocity, int i_frame_length, float f_interpolation_factor = -1)
    {
        // Synthesis parameters shortcuts
        const int i_hop_size = this->synthesis.parameters.hop_size;
        int* i_current_frame = &this->synthesis.live_values.i_current_frame;
        
        // If "track_velocity" is enabled
        if (this->track_velocity)
        {
            // If current note has changed
            if (this->f_current_midi_note != this->f_last_midi_note)
            {
                this->updateMorphSounds (this->f_current_midi_note, this->f_current_velocity);
            }
        }
        
        Sound::Frame sound_frame;
        
        while (this->synthesis.live_values.i_samples_ready < i_frame_length)
        {
            // TODO - Implement ADSR
            
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
            
            // If we are on the last frame of the shortest note
            if (*i_current_frame == this->min_note_end-1)
            {
                // End note playback
                this->synthesis.live_values.last_frame = true;
            }
            
            // Instrument::Mode::Morphing
            if (this->instrument->mode == Instrument::Mode::Morphing)
            {
                float freqs_interp_factor = *mParameters->getRawParameterValue (Morphex::PARAMETERS<float>[Morphex::Parameters::freqs_interp_factor].ID);
                float mags_interp_factor = *mParameters->getRawParameterValue (Morphex::PARAMETERS<float>[Morphex::Parameters::mags_interp_factor].ID);
                
                // TODO - Apply fade out if *i_current_frame > this->min_note_end - 4 (4 = fade_out_frames)
                sound_frame = this->instrument->morphSoundFrames (this->f_current_midi_note, morph_sounds,
                                                                  *i_current_frame, i_hop_size,
                                                                  freqs_interp_factor, mags_interp_factor);
            }
            // Instrument::Mode::FullRange
            else
            {
                if (this->instrument->interpolation_mode == Instrument::Interpolation::None or
                    this->morph_sounds[MorphLocation::Left] == this->morph_sounds[MorphLocation::Right])
                {
                    Sound* selected_sound;
                    
                    if (this->instrument->interpolation_mode == Instrument::Interpolation::None)
                    {
                        int left_note_distance = std::abs (this->f_pressed_midi_note - this->morph_sounds[MorphLocation::Left]->note);
                        int right_note_distance = std::abs (this->f_pressed_midi_note - this->morph_sounds[MorphLocation::Right]->note);

                        if (left_note_distance < right_note_distance)
                        {
                            selected_sound = morph_sounds[MorphLocation::Left];
                        }
                        else
                        {
                            selected_sound = morph_sounds[MorphLocation::Right];
                        }
                    }
                    else
                    {
                        selected_sound = morph_sounds[MorphLocation::Left];
                    }   
                    
                    sound_frame = selected_sound->getFrame (*i_current_frame, i_hop_size);
                    
                    // Get target frequency
                    float f_target_frequency = Tools::Midi::toFreq (this->f_current_midi_note);
                    float f_note_frequency = Tools::Midi::toFreq (selected_sound->note);
                    
                    if (sound_frame.hasHarmonic())
                    {
                        // Recalculate the harmonics for the current midi note
                        for (int i=0; i<sound_frame.harmonic.freqs.size(); i++)
                        {
                            sound_frame.harmonic.freqs[i] = (sound_frame.harmonic.freqs[i] / f_note_frequency) * f_target_frequency;
                        }
                    }
                    
                    if (sound_frame.hasSinusoidal())
                    {
                        // Recalculate the harmonics for the current midi note
                        for (int i=0; i<sound_frame.sinusoidal.freqs.size(); i++)
                        {
                            sound_frame.sinusoidal.freqs[i] = (sound_frame.sinusoidal.freqs[i] / f_note_frequency) * f_target_frequency;
                        }
                    }
//
//                    // Transpose left note frequencies to the target frequency
//                    Tools::Calculate::divideByScalar(sound_frame.harmonics_freqs,
//                                                     Tools::Midi::toFreq(morph_sounds[MorphLocation::Left]->note));
//                    Tools::Calculate::multiplyByScalar(sound_frame.harmonics_freqs, f_target_frequency);
                }
                else
                {
                    // TODO - Apply fade out if *i_current_frame > this->min_note_end - 4 (4 = fade_out_frames)
                    sound_frame = this->instrument->morphSoundFrames (this->f_current_midi_note, morph_sounds, *i_current_frame, i_hop_size);
                }
            }

            // NOTE - "frame" will have "i_hop_size" more samples ready to be played after each call
            // TODO - This function needs to be optimized
            this->synthesis.generateSoundFrame (sound_frame, i_frame_length);

            *i_current_frame += 1;
            
            if ( *i_current_frame >= this->min_note_end)
            {
                // End note playback
                this->playing_note = false;
            }
        }
        
        // Selecting the processed samples
        std::vector<float> frame = this->synthesis.getBuffer (Synthesis::BufferSection::Play, Channel::Mono, i_frame_length);
        
        // Update play pointer position
        this->synthesis.updatePlayPointer (i_frame_length);

        // Update samples ready to be played
        this->synthesis.live_values.i_samples_ready -= i_frame_length;
        
        return frame;
    }
    
private:
    
    AudioProcessorValueTreeState* mParameters;
    
    Instrument* instrument;
    Synthesis synthesis;
        
    // Midi
    float f_pressed_midi_note;
    float f_current_midi_note;
    SmoothedValue<float, ValueSmoothingTypes::Linear> f_current_midi_pitch_wheel;
    float f_current_velocity;
    float f_last_midi_note;
    float f_pitch_wheel_range_semitones = 3.0f;
    
    // Note playback
    bool playing_note;
    bool loop_mode;
    bool hold_note;
    bool track_velocity;
    bool allow_pitch_wheel;

    // Sounds
    Core::MorphSounds morph_sounds;
    int max_loop_start;
    int min_loop_end;
    int min_note_end;
    
    double level = 0.0;
    double mAttackGainSmoothed = 0.0;
    double mDecayGainSmoothed = 0.0;
    double tailOff = 1.0;
};
