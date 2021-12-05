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
#include "synth_sound.h"
#include "synthesis.h"
#include "parameters.h"

namespace morphex
{
class Voice : public juce::SynthesiserVoice
{
public:
    Voice (juce::AudioProcessorValueTreeState& parameters, Instrument& instrument);

    bool canPlaySound (juce::SynthesiserSound* sound) override;
    void startNote (int midiNoteNumber, float velocity, juce::SynthesiserSound*, int currentPitchWheelPosition) override;
    void stopNote (float velocity, bool allowTailOff) override;
    void pitchWheelMoved (int newPitchWheelValue) override;
    void controllerMoved (int controllerNumber, int newControllerValue) override;
    void renderNextBlock (juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples) override;
    void setCurrentPlaybackSampleRate (double playbackSampleRate) override;

    enum AdsrState
    {
        Attack = 0,
        Decay,
        Sustain,
        Release
    };
    AdsrState adsrState;

private:
    void reset();
    void clearAndResetCurrentNote();

    void updateMorphSounds (float f_note, float f_velocity);
    void updateMidiNoteWithPitchWheel (int newValue, bool set_current = false);
    void updateAdsrParams(float attack, float decay, float sustain, float release);

    std::vector<float> getNextFrame (float f_note, float f_velocity, int i_frame_length, float f_interpolation_factor = -1);

    juce::AudioProcessorValueTreeState& m_parameters;

    Instrument& m_instrument;
    Synthesis   m_synthesis;

    // Midi
    float                                                         f_pressed_midi_note;
    float                                                         f_current_midi_note;
    juce::SmoothedValue<float, juce::ValueSmoothingTypes::Linear> f_current_midi_pitch_wheel;
    float                                                         f_current_velocity;
    float                                                         f_last_midi_note;
    float                                                         f_pitch_wheel_range_semitones = 3.0f;

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
}; // namespace moprhex
