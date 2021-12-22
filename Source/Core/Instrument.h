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

// #include "note.h"
#include "sound.h"
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

namespace Constants
{
constexpr auto MidiNotes      = 128;
constexpr auto MidiVelocities = 128;
} // namespace Constants

using Note             = std::array<std::unique_ptr<morphex::Sound>, Constants::MidiVelocities>;
using MorphNotes       = std::array<Note*, MorphLocation::Total>;
using MorphSounds      = std::array<morphex::Sound*, MorphLocation::Total>;
using MorphSoundFrames = std::array<std::shared_ptr<const morphex::Frame>, MorphLocation::Total>;

namespace morphex
{
class Instrument
{
public:
    enum class OperationMode
    {
        Morphing,
        FullRange
    };

    enum class InterpolationMode
    {
        None,
        Manual,
        FrequencyBased
    };
    
    enum class ModelType
    {
        Harmonic,
        Sinusoidal,
        Stochastic,
        Attack,
        Residual
    };

    struct ActiveModels
    {
        bool Harmonic   = true;
        bool Sinusoidal = true;
        bool Stochastic = true;
        bool Attack     = true;
        bool Residual   = false;
    };

    Instrument();

    void reset();
    void setOperationMode (OperationMode operationMode);
    void setModelActive (ModelType modelType, bool active);
    void loadSound (std::string filePath, MorphLocation morphLocation = MorphLocation::Total);
    void loadSoundsFromFolder (std::string folderPath);

    OperationMode     getOperationMode();
    InterpolationMode getInterpolationMode();
    MorphNotes        getClosestNotes (float targetMidiNote);
    MorphSounds       getClosestSounds (float targetMidiNote, float velocity);
    Sound*            getMorphSound (MorphLocation morph_location);
    MorphSounds       getMorphSounds();
//    std::shared_ptr<const Frame>      getSoundFrame (float targetMidiNote, float velocity, int currentFrameIndex, int frameLength, float freqsInterpFactor, float magsInterpFactor);
    Frame      getMorphedSoundFrame (float targetMidiNote, MorphSounds morphSounds, int currentFrameIndex, int frameLength, float freqsInterpFactor = -1, float magsInterpFactor = -1) const;

    bool isModelActive (ModelType modelType);

private:
    enum FrameType
    {
        Frequencies,
        Magnitudes,
        Stochastic,
        Waveform,
    };

    Sound* getClosestLoadedSound (const Note& note, const float velocity);

    std::vector<float> interpolateFrames (FrameType frameType, float interpolationFactor, std::vector<float> frameA, std::vector<float> frameB, size_t frameLength) const;
    bool               hasAnyLoadedSound (const Note& note) const;

    std::string       m_name;
    std::string       m_samplesDirectoryPath;
    OperationMode     m_operationMode;
    InterpolationMode m_interpolationMode;
    ActiveModels      m_activeModels;

    std::array<Note, Constants::MidiNotes> m_notes;

    MorphNotes  m_morphNotes;
    MorphSounds m_morphSounds;
    
    // TODO
    // bool regenerateHarmonicPhases
    // bool regenerateSinusoidalPhases
};
}; // namespace morphex
