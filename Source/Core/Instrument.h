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

#include "note.h"
#include <vector>

enum MorphLocation
{
    // TODO - DownLeft, UpLeft and NUM_LEFT_MORPH_SOUNDS
    UpLeft = 0,
    // TODO - DownRight, UpRight and NUM_RIGHT_MORPH_SOUNDS
    DownRight,
    // TODO - NUM_MORPH_LOCATIONS = NUM_LEFT_MORPH_SOUNDS + NUM_RIGHT_MORPH_SOUNDS
    Total
};

using MorphNotes       = std::array<morphex::Note*, MorphLocation::Total>;
using MorphSounds      = std::array<morphex::Sound*, MorphLocation::Total>;
using MorphSoundFrames = std::array<morphex::Sound::Frame, MorphLocation::Total>;

namespace Constants
{
constexpr auto NUM_MIDI_NOTES = 128;
}

namespace morphex
{
class Instrument
{
public:
    enum class Mode
    {
        Morphing,
        FullRange
    };

    enum class Interpolation
    {
        None,
        Manual,
        FrequencyBased
    };

    enum FrameType
    {
        Frequencies, // Interpolate Frequencies
        Magnitudes,  // Interpolate Magnitudes
        Stochastic,  // Interpolate Stochastic
        Waveform,    // Interpolate Waveforms
    };

    struct Generate
    {
        bool harmonic   = true;
        bool sinusoidal = true;
        bool stochastic = true;
        bool attack     = true;
        bool residual   = false;
    };
    Generate generate;

    // Notes
    std::vector<Note*> note;

    // Data
    std::string name;
    std::string samples_dirpath;

    // Mode
    Mode mode = Mode::Morphing;

    // Interpolation
    Interpolation interpolation_mode = Interpolation::Manual;

    Instrument();
    ~Instrument();

    void init();
    void reset();

    void loadSound (std::string file_path, MorphLocation morph_location = MorphLocation::Total);
    void loadAllSoundsFromFolder (std::string folder_path);

    std::vector<Note*> getLoadedNotes();

    MorphNotes  getCloserNotes (float f_target_note);
    MorphSounds getCloserSounds (float f_target_note, float f_velocity);

    MorphNotes  getMorphNotes();
    void        setMorphNote (Note* note, MorphLocation morph_location, int midi_velocity = Constants::MAX_MIDI_VELOCITY);
    MorphSounds getMorphSounds();
    Sound*      getMorphSound (MorphLocation morph_location);
    void        setMorphSound (Sound* sound, MorphLocation morph_location);

    Sound*       getSound (float f_note, float f_velocity);
    Sound::Frame getSoundFrame (float f_note, float f_velocity, int i_current_frame, int i_frame_length, float f_freqs_interp_factor, float f_mags_interp_factor);

    Sound::Frame morphSoundFrames (float f_target_note, MorphSounds morph_sounds, int i_current_frame, int i_frame_length, float f_freqs_interp_factor = -1, float f_mags_interp_factor = -1);

    std::vector<float> getNextFrame (float f_note, float f_velocity, int i_frame_length, float f_freqs_interp_factor, float f_mags_interp_factor);

    std::vector<float> interpolateFrames (FrameType frame_type, float interp_factor, std::vector<float> frame_1, std::vector<float> frame_2, int i_frame_length, std::vector<int> idx_harmonics = std::vector<int>());

private:
    MorphNotes  morph_notes;
    MorphSounds morph_sounds {};
};
}; // namespace morphex
