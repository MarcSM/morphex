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

#include "instrument.h"
#include "synthesis.h"

#include "../helpers/helper_functions.h"
#include "../helpers/parameters.h"
#include "synth_sound.h"

namespace Core
{
struct Voice : public SynthesiserVoice
{
    enum StateADSR
    {
        Attack = 0,
        Decay,
        Sustain,
        Release
    };
    StateADSR adsr;

    Voice (Instrument& instrument, AudioProcessorValueTreeState& parameters) :
        m_parameters (parameters),
        m_instrument (instrument),
        m_synthesis (instrument)
    {
        reset();
    }

    void reset()
    {
        // Note playback
        adsr              = StateADSR::Attack;
        playing_note      = false;
        loop_mode         = true;
        hold_note         = false;
        track_velocity    = false; // NOTE - High CPU usage if true
        allow_pitch_wheel = true;

        // Default values
        f_pressed_midi_note = 0;
        f_current_midi_note = 0;
        f_current_velocity  = 0;
        f_last_midi_note    = 0;

        // Sounds
        max_loop_start = 0;
        min_loop_end   = 0;
        min_note_end   = 0;
    }

    bool canPlaySound (SynthesiserSound* synthSound) override
    {
        return dynamic_cast<SynthSound*> (synthSound) != nullptr;
    }

    void startNote (int midiNoteNumber, float velocity, SynthesiserSound*, int currentPitchWheelPosition) override
    {
        //        // Note playback
        //        adsr = StateADSR::Attack;
        playing_note = true;

        f_pressed_midi_note = (float) midiNoteNumber;
        f_current_midi_note = (float) midiNoteNumber;
        f_last_midi_note    = (float) midiNoteNumber;
        f_current_velocity  = velocity;

        updateMorphSounds (f_pressed_midi_note, f_current_velocity);

        updateMidiNoteWithPitchWheel (currentPitchWheelPosition, true);

        // Velocity affects the gain
        level               = velocity * 0.15;
        mAttackGainSmoothed = 0.0;
        mDecayGainSmoothed  = 1.0;
        tailOff             = 1.0;
    }

    void stopNote (float velocity, bool allowTailOff) override
    {
        if (allowTailOff)
        {
            if (tailOff == 1.0)
                tailOff *= 0.999;
        }
        else
        {
            clearAndResetCurrentNote();
            playing_note = false;
        }
    }

    void pitchWheelMoved (int newValue) override
    {
        updateMidiNoteWithPitchWheel (newValue);
    }

    void updateMidiNoteWithPitchWheel (int newValue, bool set_current = false)
    {
        // Map range 0-16383 to -12:12 semitones
        //        f_current_midi_pitch_wheel = jmap((float)newValue, 0.0f, 16383.0f,

        // TODO - Custom "semitones_range"
        //        float semitones_range = 12.0;

        if (allow_pitch_wheel)
        {
            float f_new_midi_pitch_wheel = jmap ((float) newValue, 0.0f, 16383.0f, -f_pitch_wheel_range_semitones, f_pitch_wheel_range_semitones);

            if (set_current)
            {
                f_current_midi_pitch_wheel.setCurrentAndTargetValue (f_new_midi_pitch_wheel);
            }
            else
            {
                f_current_midi_pitch_wheel.setTargetValue (f_new_midi_pitch_wheel);
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
        clearCurrentNote();
        m_synthesis.reset();
        reset();
    }

    void updateMorphSounds (float f_note, float f_velocity)
    {
        // Instrument::Mode::Morphing
        if (m_instrument.mode == Instrument::Mode::Morphing)
        {
            morph_sounds = m_instrument.getMorphSounds();
        }
        // Instrument::Mode::FullRange
        else
        {
            morph_sounds = m_instrument.getCloserSounds (f_note, f_velocity);
        }

        bool first_iter = true;

        for (int i = 0; i < morph_sounds.size(); i++)
        {
            if (morph_sounds[i] != nullptr)
            {
                if (first_iter)
                {
                    max_loop_start = morph_sounds[i]->loop.start;
                    min_loop_end   = morph_sounds[i]->loop.end;
                    min_note_end   = morph_sounds[i]->max_frames;

                    first_iter = false;
                }
                else
                {
                    // Compute common looping regions
                    max_loop_start = std::max (max_loop_start, morph_sounds[i]->loop.start);
                    min_loop_end   = std::min (min_loop_end, morph_sounds[i]->loop.end);

                    // Get the minimum length of both notes
                    min_note_end = std::min (min_note_end, morph_sounds[i]->max_frames);
                }
            }
        }

        f_last_midi_note = f_note;
    }

    void renderNextBlock (AudioBuffer<float>& outputBuffer, int startSample, int numSamples) override
    {
        if (playing_note)
        {
            if (allow_pitch_wheel)
                f_current_midi_note = f_pressed_midi_note + f_current_midi_pitch_wheel.getNextValue();

            std::vector<float> frame = getNextFrame (f_current_midi_note, f_current_velocity, numSamples);

            for (int i_sample = 0; i_sample < numSamples; i_sample++)
            {
                auto currentSample = (float) (frame[i_sample] * level * tailOff);
                //                auto currentSample = (float) (std::sin (currentAngle) * level * tailOff);

                for (auto i = outputBuffer.getNumChannels(); --i >= 0;)
                {
                    outputBuffer.addSample (i, startSample, currentSample);
                }

                ++startSample;

                if (tailOff < 1.0)
                {
                    tailOff *= 0.999;

                    if (tailOff <= 0.005)
                    {
                        playing_note = false;
                    }
                }
            }

            if (!playing_note)
            {
                clearAndResetCurrentNote();
            }
        }
    }

    std::vector<float> getNextFrame (float f_note, float f_velocity, int i_frame_length, float f_interpolation_factor = -1)
    {
        // Synthesis parameters shortcuts
        const int i_hop_size      = m_synthesis.parameters.hop_size;
        int*      i_current_frame = &m_synthesis.live_values.i_current_frame;

        // If "track_velocity" is enabled
        if (track_velocity)
        {
            // If current note has changed
            if (f_current_midi_note != f_last_midi_note)
            {
                updateMorphSounds (f_current_midi_note, f_current_velocity);
            }
        }

        Sound::Frame sound_frame;

        while (m_synthesis.live_values.i_samples_ready < i_frame_length)
        {
            // TODO - Implement ADSR

            //                float adsr_attack = *m_parameters.getRawParameterValue(SMTParameterID[kParameter_asdr_attack]);
            //                float adsr_decay = *m_parameters.getRawParameterValue(SMTParameterID[kParameter_asdr_decay]);
            //                float adsr_sustain = *m_parameters.getRawParameterValue(SMTParameterID[kParameter_asdr_sustain]);
            //                float adsr_release = *m_parameters.getRawParameterValue(SMTParameterID[kParameter_asdr_release]);
            //
            //                // Update ADSR parameters
            //                updateAdsrParams(adsr_attack, adsr_decay, adsr_sustain, adsr_release);

            //            int i_current_sample = *i_current_frame * i_hop_size;
            //
            //            // If ADSR is NOT on "Release" state
            //            if (adsr != StateADSR::Release)
            //            {
            //                if (adsr != StateADSR::Sustain and
            //                    ( i_current_sample >= max_loop_start ) )
            //                {
            //                    adsr = StateADSR::Sustain;
            //                }
            //
            //                // If loop mode is enabled
            //                if (loop_mode)
            //                {
            //                    // Keep the current_frame pointer inside the lowest "note.loop.end"
            //                    // and the highest "note.loop.start"
            //                    if ( i_current_sample >= min_loop_end )
            //                    {
            //                        *i_current_frame = int( max_loop_start / i_hop_size );
            //                    }
            //                }
            //                // If "loop_mode" is NOT activated
            //                else
            //                {
            //                    // If we are beyond the end loop point
            //                    if ( i_current_sample >= min_loop_end)
            //                    {
            //                        // Switch ADSR to "Relase" mode
            //                        adsr = StateADSR::Release;
            //                    }
            //                }
            //            }
            //
            //            // If ADSR is on "Release" state
            //            if (adsr == StateADSR::Release)
            //            {
            //                // TODO - JUMP CURRENT FRAME TO RELEASE SECTION, if defined of course,
            //                // if not, play the rest of the other morph_sounds or shorten the whole thing
            //                // to the shortest one and apply a fade out on hramonics_mags 4 frames before the end
            //
            //                if ( ( i_current_sample < min_loop_end ) or hold_note )
            //                {
            //                    *i_current_frame = int( min_loop_end / i_hop_size );
            //                }
            //            }

            // If we are on the last frame of the shortest note
            if (*i_current_frame == min_note_end - 1)
            {
                // End note playback
                m_synthesis.live_values.last_frame = true;
            }

            // Instrument::Mode::Morphing
            if (m_instrument.mode == Instrument::Mode::Morphing)
            {
                float freqs_interp_factor = *m_parameters.getRawParameterValue (Morphex::PARAMETERS<float>[Morphex::Parameters::FreqsInterpFactor].ID);
                float mags_interp_factor  = *m_parameters.getRawParameterValue (Morphex::PARAMETERS<float>[Morphex::Parameters::MagsInterpFactor].ID);

                // TODO - Apply fade out if *i_current_frame > min_note_end - 4 (4 = fade_out_frames)
                sound_frame = m_instrument.morphSoundFrames (f_current_midi_note, morph_sounds, *i_current_frame, i_hop_size, freqs_interp_factor, mags_interp_factor);
            }
            // Instrument::Mode::FullRange
            else
            {
                if (m_instrument.interpolation_mode == Instrument::Interpolation::None or morph_sounds[MorphLocation::UpLeft] == morph_sounds[MorphLocation::DownRight])
                {
                    Sound* selected_sound;

                    if (m_instrument.interpolation_mode == Instrument::Interpolation::None)
                    {
                        int left_note_distance  = std::abs (f_pressed_midi_note - morph_sounds[MorphLocation::UpLeft]->note);
                        int right_note_distance = std::abs (f_pressed_midi_note - morph_sounds[MorphLocation::DownRight]->note);

                        if (left_note_distance < right_note_distance)
                        {
                            selected_sound = morph_sounds[MorphLocation::UpLeft];
                        }
                        else
                        {
                            selected_sound = morph_sounds[MorphLocation::DownRight];
                        }
                    }
                    else
                    {
                        selected_sound = morph_sounds[MorphLocation::UpLeft];
                    }

                    sound_frame = selected_sound->getFrame (*i_current_frame, i_hop_size);

                    // Get target frequency
                    float f_target_frequency = Tools::Midi::toFreq (f_current_midi_note);
                    float f_note_frequency   = Tools::Midi::toFreq (selected_sound->note);

                    if (sound_frame.hasHarmonic())
                    {
                        // Recalculate the harmonics for the current midi note
                        for (int i = 0; i < sound_frame.harmonic.freqs.size(); i++)
                        {
                            sound_frame.harmonic.freqs[i] = (sound_frame.harmonic.freqs[i] / f_note_frequency) * f_target_frequency;

                            //                            if (sound_frame.hasPhases(sound_frame.harmonic))
                            //                            {
                            //                                // TODO TRANSPOSE PHASE (WIP)
                            //                                const int H = synthesis.parameters.hop_size;
                            //                                const int FS = synthesis.parameters.fs;
                            ////                                const int FRAME_SIZE = synthesis.parameters.fft_size;
                            //
                            //                                // NEW
                            //                                auto lastPhase = synthesis.live_values.harmonic.phases[i];
                            //                                auto lastFreq = synthesis.live_values.harmonic.last_freqs[i];
                            //                                auto phaseDelta = sound_frame.harmonic.phases[i] - lastPhase;
                            //                                synthesis.live_values.harmonic.phases[i] = sound_frame.harmonic.phases[i];
                            //
                            ////                                auto phaseDeltaPrime = (M_PI * (lastFreq + sound_frame.harmonic.freqs[i]) / FS) * H;
                            ////                                auto phaseDeltaPrime = (M_PI * (lastFreq + sound_frame.harmonic.freqs[i]) / FS) * H;
                            ////                                auto phaseDeltaPrime = phaseDelta - H * 2 * M_PI;
                            //                                auto phaseDeltaPrime = phaseDelta - H * 2 * M_PI;
                            //
                            ////                                auto phaseDeltaPrimeMod = std::fmod (phaseDeltaPrime + M_PI, (2.0 * M_PI) - M_PI);
                            //                                auto phaseDeltaPrimeMod = std::fmod (phaseDeltaPrime, (2.0 * M_PI));
                            //                                sound_frame.harmonic.phases[i] = phaseDeltaPrimeMod;
                            ////                                synthesis.live_values.harmonic.phases[i] = phaseDeltaPrimeMod; // test
                            //
                            //                                // LIVE
                            //
                            ////                                const int H = synthesis.parameters.hop_size;
                            ////                                const int MAX_HARMONICS = sound_frame.getMaxHarmonics();
                            ////                                std::vector<int> idx_harmonics = Tools::Generate::range (0, MAX_HARMONICS);
                            ////
                            ////                                sound_frame.harmonic.phases = Tools::Get::valuesByIndex (synthesis.live_values.harmonic.phases, idx_harmonics);
                            ////
                            ////                                // Save the current frequencies to be available fot the next iteration
                            ////                                synthesis.updateLastFreqs (synthesis.live_values.harmonic, sound_frame.harmonic.freqs, idx_harmonics);
                            ////
                            ////                                // Update phases for the next iteration
                            ////                                synthesis.updatePhases (synthesis.live_values.harmonic, sound_frame.harmonic.freqs, idx_harmonics, H);
                            //
                            //                                // OLD
                            //
                            ////                                synthesis.live_values.harmonic.last_freqs[i] = sound_frame.harmonic.freqs[i];
                            ////                                auto lastPhase = synthesis.live_values.harmonic.phases[i];
                            ////
                            ////                                sound_frame.harmonic.phases[i] = lastPhase + i_hop_size * sound_frame.harmonic.freqs[i];
                            ////
                            ////                                synthesis.live_values.harmonic.phases[i] = sound_frame.harmonic.phases[i];
                            ////                                //                            updatePhases (live_values.harmonic, sound_frame.harmonic.freqs, idx_harmonics, H);
                            ////                                // sound_frame.harmonic.phases[i] = (sound_frame.harmonic.phases[i] / f_note_frequency) * f_target_frequency;
                            //                            }

                            m_synthesis.live_values.harmonic.last_freqs[i] = sound_frame.harmonic.freqs[i];
                        }
                    }

                    if (sound_frame.hasSinusoidal())
                    {
                        // Recalculate the harmonics for the current midi note
                        for (int i = 0; i < sound_frame.sinusoidal.freqs.size(); i++)
                        {
                            sound_frame.sinusoidal.freqs[i] = (sound_frame.sinusoidal.freqs[i] / f_note_frequency) * f_target_frequency;
                        }
                    }

                    // TODO TRANSPOSE PHASE
                    //
                    //                    // Transpose left note frequencies to the target frequency
                    //                    Tools::Calculate::divideByScalar(sound_frame.harmonics_freqs,
                    //                                                     Tools::Midi::toFreq(morph_sounds[MorphLocation::Left]->note));
                    //                    Tools::Calculate::multiplyByScalar(sound_frame.harmonics_freqs, f_target_frequency);
                }
                else
                {
                    // TODO - Apply fade out if *i_current_frame > min_note_end - 4 (4 = fade_out_frames)
                    sound_frame = m_instrument.morphSoundFrames (f_current_midi_note, morph_sounds, *i_current_frame, i_hop_size);
                }
            }

            // NOTE - "frame" will have "i_hop_size" more samples ready to be played after each call
            // TODO - This function needs to be optimized
            m_synthesis.generateSoundFrame (sound_frame, i_frame_length);

            *i_current_frame += 1;

            if (*i_current_frame >= min_note_end)
            {
                // End note playback
                playing_note = false;
            }
        }

        // Selecting the processed samples
        std::vector<float> frame = m_synthesis.getBuffer (Synthesis::BufferSection::Play, Channel::Mono, i_frame_length);

        // Update play pointer position
        m_synthesis.updatePlayPointer (i_frame_length);

        // Update samples ready to be played
        m_synthesis.live_values.i_samples_ready -= i_frame_length;

        return frame;
    }

private:
    AudioProcessorValueTreeState& m_parameters;

    Instrument& m_instrument;
    Synthesis   m_synthesis;

    // Midi
    float                                             f_pressed_midi_note;
    float                                             f_current_midi_note;
    SmoothedValue<float, ValueSmoothingTypes::Linear> f_current_midi_pitch_wheel;
    float                                             f_current_velocity;
    float                                             f_last_midi_note;
    float                                             f_pitch_wheel_range_semitones = 3.0f;

    // Note playback
    bool playing_note;
    bool loop_mode;
    bool hold_note;
    bool track_velocity;
    bool allow_pitch_wheel;

    // Sounds
    MorphSounds morph_sounds;
    int         max_loop_start;
    int         min_loop_end;
    int         min_note_end;

    double level               = 0.0;
    double mAttackGainSmoothed = 0.0;
    double mDecayGainSmoothed  = 0.0;
    double tailOff             = 1.0;
};
}; // namespace Core
