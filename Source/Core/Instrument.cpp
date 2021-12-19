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

#include "instrument.h"
#include "tools.h"

namespace Constants
{
constexpr auto DefaultInstrumentName       = "New Instrument";
constexpr auto DefaultSamplesDirectoryPath = "";
constexpr auto DefaultEmptyFrequencyHz     = 0.0f;
constexpr auto DefaultEmptyMagnitudeDb     = -200.0f;
} // namespace Constants

namespace morphex
{
Instrument::Instrument() :
    m_name (Constants::DefaultInstrumentName),
    m_samplesDirectoryPath (Constants::DefaultSamplesDirectoryPath),
    m_operationMode (OperationMode::Morphing),
    m_interpolationMode (InterpolationMode::Manual) {};

void Instrument::reset()
{
    m_name                 = Constants::DefaultInstrumentName;
    m_samplesDirectoryPath = Constants::DefaultSamplesDirectoryPath;

    for (auto& note : m_notes)
    {
        for (auto& sound : note)
        {
            sound = nullptr;
        }
    }
}

void Instrument::setOperationMode (OperationMode operationMode)
{
    m_operationMode = operationMode;

    if (operationMode == OperationMode::FullRange && m_interpolationMode == InterpolationMode::Manual)
    {
        m_interpolationMode = InterpolationMode::None;
    }
}

void Instrument::setModelActive (ModelType modelType, bool active)
{
    switch (modelType)
    {
        case ModelType::Harmonic:
            m_activeModels.Harmonic = active;
            break;
        case ModelType::Sinusoidal:
            m_activeModels.Sinusoidal = active;
            break;
        case ModelType::Stochastic:
            m_activeModels.Stochastic = active;
            break;
        case ModelType::Attack:
            m_activeModels.Attack = active;
            break;
        case ModelType::Residual:
            m_activeModels.Residual = active;
            break;
        default:
            jassert (false);
            break;
    }
}

void Instrument::loadSound (std::string filePath, MorphLocation morphLocation)
{
    auto sound = std::make_unique<Sound> (filePath);

    if (sound && sound->isLoaded())
    {
        // TODO - Improve this logic
        if (morphLocation < MorphLocation::Total)
        {
            MorphLocation otherSoundMorphLocation = MorphLocation::Total;

            if (morphLocation == MorphLocation::UpLeft)
            {
                otherSoundMorphLocation = MorphLocation::DownRight;
            }
            else if (morphLocation == MorphLocation::DownRight)
            {
                otherSoundMorphLocation = MorphLocation::UpLeft;
            }

            Sound* otherLoadedSound = getMorphSound (otherSoundMorphLocation);
            
            if (otherLoadedSound)
            {
                const auto soundInfo = sound->getInfo();
                const auto otherLoadedSoundInfo = otherLoadedSound->getInfo();
                
                if (soundInfo &&
                    otherLoadedSoundInfo &&
                    soundInfo->note == otherLoadedSoundInfo->note &&
                    soundInfo->velocity == otherLoadedSoundInfo->velocity)
                {
                    if (soundInfo->velocity == 0 || soundInfo->velocity == 1)
                    {
                        sound->setVelocity(Constants::MidiVelocities);
                    }
                    else
                    {
                        sound->setVelocity(1);
                    }
                }
            }
        }

        if (morphLocation < MorphLocation::Total)
        {
            //            setMorphNote (note[sound->note], morphLocation, sound.velocity);

            //            morph_notes[morph_location] = note;

            //            setMorphSound (&morph_notes[morph_location]->velocity[midi_velocity]->sound, morph_location);

            m_morphSounds[morphLocation] = sound.get();
        }
        
        const auto soundInfo = sound->getInfo();

        if (soundInfo)
        {
            m_notes[soundInfo->note][soundInfo->velocity] = std::move (sound);
        }
        else
        {
            jassertfalse;
        }
    }
    else
    {
        jassertfalse;
    }
}

void Instrument::loadSoundsFromFolder (std::string folderPath)
{
    int numOfSamplesLoaded = 0;

    juce::DirectoryIterator iter (juce::File (folderPath), true, "*.had");

    while (iter.next())
    {
        juce::File  soundFile (iter.getFile());
        std::string soundFilePath = soundFile.getFullPathName().toStdString();

        if (numOfSamplesLoaded < MorphLocation::Total)
        {
            loadSound (soundFilePath, static_cast<MorphLocation> (numOfSamplesLoaded));
        }
        else
        {
            loadSound (soundFilePath);
        }

        ++numOfSamplesLoaded;
    }
}

Instrument::OperationMode Instrument::getOperationMode()
{
    return m_operationMode;
}

Instrument::InterpolationMode Instrument::getInterpolationMode()
{
    return m_interpolationMode;
}

MorphNotes Instrument::getClosestNotes (float targetMidiNote)
{
    int lowerTargetMidiNote  = round (targetMidiNote);
    int higherTargetMidiNote = ceil (targetMidiNote);

    // Output (use "{}" to ensure "nullptr" initialization)
    MorphNotes closerNotes {};

    std::map<int, Note*> loadedNotes;

    for (int i = 0u; i < m_notes.size(); ++i)
    {
        Note& note = m_notes[i];

        if (hasAnyLoadedSound (note))
        {
            loadedNotes.insert ({ i, &note });
        }
    }

    // Instrument::Mode::Morphing
    if (m_operationMode == Instrument::OperationMode::Morphing)
    {
        auto numOfNotesToLoad = std::min (loadedNotes.size(), static_cast<size_t> (MorphLocation::Total));

        for (auto i = 0u; i < numOfNotesToLoad; ++i)
        {
            closerNotes[i] = loadedNotes[i];
        }
    }
    // Instrument::Mode::FullRange
    else
    {
        // For loadedNotes sorted from min to max
        for (const auto& [midiNote, note] : loadedNotes)
        {
            if (midiNote < lowerTargetMidiNote)
            {
                closerNotes[MorphLocation::UpLeft] = note;
            }
            else if (higherTargetMidiNote < midiNote)
            {
                closerNotes[MorphLocation::DownRight] = note;
                break;
            }
            else
            {
                closerNotes[MorphLocation::UpLeft]    = note;
                closerNotes[MorphLocation::DownRight] = note;

                if (higherTargetMidiNote <= midiNote)
                {
                    break;
                }
            }
        }
    }

    if (!closerNotes[MorphLocation::UpLeft])
    {
        closerNotes[MorphLocation::UpLeft] = closerNotes[MorphLocation::DownRight];
    }
    if (!closerNotes[MorphLocation::DownRight])
    {
        closerNotes[MorphLocation::DownRight] = closerNotes[MorphLocation::UpLeft];
    }

    return closerNotes;
}

MorphSounds Instrument::getClosestSounds (float targetMidiNote, float velocity)
{
    auto closestNotes = getClosestNotes (targetMidiNote);

    // Output
    MorphSounds closestSounds {};

    jassert (closestNotes.size() == closestSounds.size());

    // Getting closer sounds in velocity
    for (auto i = 0u; i < closestNotes.size(); ++i)
    {
        const auto& note = closestNotes[i];

        if (note)
        {
            closestSounds[i] = getClosestLoadedSound (*note, velocity);
        }
    }

    return closestSounds;
}

MorphSounds Instrument::getMorphSounds()
{
    return m_morphSounds;
}

Sound* Instrument::getMorphSound (MorphLocation morph_location)
{
    return m_morphSounds[morph_location];
}

Sound::Frame Instrument::getSoundFrame (float targetMidiNote, float velocity, int currentFrameIndex, int frameLength, float freqsInterpFactor, float magsInterpFactor)
{
    // TODO - Return empty sound frame if all "morphSounds" are nullptrs

    auto morphSounds = getClosestSounds (targetMidiNote, velocity);

    if (morphSounds[MorphLocation::UpLeft] == morphSounds[MorphLocation::DownRight])
    {
        return morphSounds[MorphLocation::UpLeft]->getFrame (currentFrameIndex, frameLength);
    }
    else
    {
        return getMorphedSoundFrame (targetMidiNote, morphSounds, currentFrameIndex, frameLength, freqsInterpFactor, magsInterpFactor);
    }
}

// NOTE: About looping, two modes, the first one is that each note loops
// on their own loop points and then do the morphing, it will cause misalignment.
// The second one is to have the maximum starting loop point and the minimum ending
// loop point, the loop section will be smaller but it will sound homogeneous
// among iterations.
Sound::Frame Instrument::getMorphedSoundFrame (float targetMidiNote, MorphSounds morphSounds, int currentFrameIndex, int frameLength, float freqsInterpFactor, float magsInterpFactor)
{
    // Output
    Sound::Frame morphedSoundFrame;

    // Get target frequency
    auto targetFrequency = Tools::Midi::toFreq (targetMidiNote);

    // Interpolation factor is calculated taking into account
    // how far is each note from the target frequency (normalized)
    if (m_interpolationMode == InterpolationMode::FrequencyBased)
    {
        if (morphSounds[MorphLocation::UpLeft]->note == morphSounds[MorphLocation::DownRight]->note)
        {
            freqsInterpFactor = 0.0;
        }
        else
        {
            freqsInterpFactor = (targetFrequency - Tools::Midi::toFreq (morphSounds[MorphLocation::UpLeft]->note)) / (Tools::Midi::toFreq (morphSounds[MorphLocation::DownRight]->note) - Tools::Midi::toFreq (morphSounds[MorphLocation::UpLeft]->note));
        }

        magsInterpFactor = freqsInterpFactor;
    }

    auto stocsInterpFactor    = freqsInterpFactor;
    auto attackInterpFactor   = freqsInterpFactor;
    auto residualInterpFactor = freqsInterpFactor;

    MorphSoundFrames morphSoundFrames;

    // Select current frame
    morphSoundFrames[MorphLocation::UpLeft]    = morphSounds[MorphLocation::UpLeft]->getFrame (currentFrameIndex, frameLength);
    morphSoundFrames[MorphLocation::DownRight] = morphSounds[MorphLocation::DownRight]->getFrame (currentFrameIndex, frameLength);

    // Get the maximum number of hamronics and sound length
    auto numOfMaxHarmonics = std::max (morphSoundFrames[MorphLocation::UpLeft].getMaxHarmonics(), morphSoundFrames[MorphLocation::DownRight].getMaxHarmonics());

    // NOTE - If it doesn't sound properly, try to always give more weight
    // to the higer samples (it is necessary to define a function that,
    // given two frequencies, calculate the weight that is given to each
    // one so that it contributes to the morphed sample).
    // Saparate frecuency and magnitude interpolation factors?

    // A list with the indexes of the harmonics we want to interpolate
    auto harmonicsIndices = Tools::Generate::range (0, numOfMaxHarmonics);

    // TOTEST - Live fundamental transposition
    //         def transposeFreqs(note_hfreq, f_note_freq, targetFrequency):
    // //             currrent_fundamental = instrument.available_notes[24][100].analysis.output.values.hfreq[0][0]
    // //             explicit_fundamental = i_note
    //             return (note_hfreq / f_note_freq) * targetFrequency

    if (morphSoundFrames[MorphLocation::UpLeft].hasHarmonic() or morphSoundFrames[MorphLocation::DownRight].hasHarmonic())
    {
        // TODO - Check parameter morphSounds[MorphLocation::Left]->parameters.transpose.harmonic
        // if false, do not transpose the frame of ote

        // Transpose left note frequencies to the target frequency
        Tools::Calculate::divideByScalar (morphSoundFrames[MorphLocation::UpLeft].harmonic.freqs, Tools::Midi::toFreq (morphSounds[MorphLocation::UpLeft]->note));
        Tools::Calculate::multiplyByScalar (morphSoundFrames[MorphLocation::UpLeft].harmonic.freqs, targetFrequency);

        // Transpose right note frequencies to the target frequency
        Tools::Calculate::divideByScalar (morphSoundFrames[MorphLocation::DownRight].harmonic.freqs, Tools::Midi::toFreq (morphSounds[MorphLocation::DownRight]->note));
        Tools::Calculate::multiplyByScalar (morphSoundFrames[MorphLocation::DownRight].harmonic.freqs, targetFrequency);

        // Interpolating the frequencies of the given harmonics
        morphedSoundFrame.harmonic.freqs = interpolateFrames (FrameType::Frequencies, freqsInterpFactor, morphSoundFrames[MorphLocation::UpLeft].harmonic.freqs, morphSoundFrames[MorphLocation::DownRight].harmonic.freqs, numOfMaxHarmonics);

        // Interpolating the magnitudes of the given harmonics
        morphedSoundFrame.harmonic.mags = interpolateFrames (FrameType::Magnitudes, magsInterpFactor, morphSoundFrames[MorphLocation::UpLeft].harmonic.mags, morphSoundFrames[MorphLocation::DownRight].harmonic.mags, numOfMaxHarmonics);
    }

    // TODO - Add sinusoidal component
    //        if (morphSoundFrames[MorphLocation::Left].hasSinusoidal() or
    //            morphSoundFrames[MorphLocation::Right].hasSinusoidal())
    //        {
    //            // Transpose left note frequencies to the target frequency
    //            Tools::Calculate::divideByScalar(morphSoundFrames[MorphLocation::Left].sinusoidal.freqs,
    //                                             Tools::Midi::toFreq(morphSounds[MorphLocation::Left]->note));
    //            Tools::Calculate::multiplyByScalar(morphSoundFrames[MorphLocation::Left].sinusoidal.freqs, targetFrequency);
    //
    //            // Transpose right note frequencies to the target frequency
    //            Tools::Calculate::divideByScalar(morphSoundFrames[MorphLocation::Right].sinusoidal.freqs,
    //                                             Tools::Midi::toFreq(morphSounds[MorphLocation::Right]->note));
    //            Tools::Calculate::multiplyByScalar(morphSoundFrames[MorphLocation::Right].sinusoidal.freqs, targetFrequency);
    //
    //            // Interpolating the frequencies of the given sinusoids
    //            morphedSoundFrame.sinusoidal.freqs =
    //            interpolateFrames(FrameType::Frequencies,
    //                              freqsInterpFactor,
    //                              morphSoundFrames[MorphLocation::Left].sinusoidal.freqs,
    //                              morphSoundFrames[MorphLocation::Right].sinusoidal.freqs,
    //                              numOfMaxHarmonics,
    //                              harmonicsIndices);
    //
    //            // Interpolating the magnitudes of the given sinusoids
    //            morphedSoundFrame.sinusoidal.mags =
    //            interpolateFrames(FrameType::Magnitudes,
    //                              magsInterpFactor,
    //                              morphSoundFrames[MorphLocation::Left].sinusoidal.mags,
    //                              morphSoundFrames[MorphLocation::Right].sinusoidal.mags,
    //                              numOfMaxHarmonics,
    //                              harmonicsIndices);
    //        }

    if (morphSoundFrames[MorphLocation::UpLeft].hasStochastic() or morphSoundFrames[MorphLocation::DownRight].hasStochastic())
    {
        // Interpolating the stochastic components of the given harmonics
        morphedSoundFrame.stochastic = interpolateFrames (FrameType::Stochastic, stocsInterpFactor, morphSoundFrames[MorphLocation::UpLeft].stochastic, morphSoundFrames[MorphLocation::DownRight].stochastic, numOfMaxHarmonics);
    }

    if (morphSoundFrames[MorphLocation::UpLeft].hasAttack() or morphSoundFrames[MorphLocation::DownRight].hasAttack())
    {
        // TODO - Check parameter morphSounds[MorphLocation::Left]->parameters.transpose.attack
        // if false, do not transpose the frame of ote

        // Interpolating the stochastic components of the given harmonics
        morphedSoundFrame.attack = interpolateFrames (FrameType::Waveform, attackInterpFactor, morphSoundFrames[MorphLocation::UpLeft].attack, morphSoundFrames[MorphLocation::DownRight].attack, frameLength);
    }

    if (morphSoundFrames[MorphLocation::UpLeft].hasResidual() or morphSoundFrames[MorphLocation::DownRight].hasResidual())
    {
        // TODO - Check parameter morphSounds[MorphLocation::Left]->parameters.transpose.residual
        // if false, do not transpose the frame of ote

        // Interpolating the stochastic components of the given harmonics
        morphedSoundFrame.residual = interpolateFrames (FrameType::Waveform, residualInterpFactor, morphSoundFrames[MorphLocation::UpLeft].residual, morphSoundFrames[MorphLocation::DownRight].residual, frameLength);
    }

    return morphedSoundFrame;
}

bool Instrument::isModelActive (const Model::Type modelType)
{
    bool isModelActive = false;

    switch (modelType)
    {
        case Model::Type::Harmonic:
            isModelActive = m_activeModels.Harmonic;
            break;
        case Model::Type::Sinusoidal:
            isModelActive = m_activeModels.Sinusoidal;
            break;
        case Model::Type::Stochastic:
            isModelActive = m_activeModels.Stochastic;
            break;
        case Model::Type::Attack:
            isModelActive = m_activeModels.Attack;
            break;
        case Model::Type::Residual:
            isModelActive = m_activeModels.Residual;
            break;
        default:
            jassert (false);
            break;
    }

    return isModelActive;
}

Sound* Instrument::getClosestLoadedSound (const Note& note, const float velocity)
{
    Sound* closestLoadedSound;

    // Range velocity from 0-1 to 0-127
    auto midiRangedVelocity       = juce::jmap (velocity, 0.0f, 1.0f, 1.0f, 127.0f);
    auto shortestVelocityDistance = Constants::MidiVelocities - 1.0f;

    for (auto& sound : note)
    {
        if (sound && sound->isLoaded())
        {
            auto velocityDistance = std::abs (sound->velocity - midiRangedVelocity);

            // Prefer lower velocities ("<=" for upper velocities)
            if (velocityDistance < shortestVelocityDistance)
            {
                closestLoadedSound       = sound.get();
                shortestVelocityDistance = velocityDistance;
            }
        }
    }

    return closestLoadedSound;
}

std::vector<float> Instrument::interpolateFrames (Instrument::FrameType frameType, float interpolationFactor, std::vector<float> frameA, std::vector<float> frameB, int frameLength)
{
    // TODO - Get rid of the for loops, use matrix multiplications instead

    auto defaultEmptyValue = (frameType == FrameType::Magnitudes) ? Constants::DefaultEmptyMagnitudeDb : Constants::DefaultEmptyFrequencyHz;

    // Output
    std::vector<float> interpolatedFrame (frameLength, defaultEmptyValue);

    if (frameLength > 0)
    {
        // TODO - Check if all elements in the magnitude frame are 0.0 if not, then load the sound

        // Aux values
        float auxValue1 = defaultEmptyValue;
        float auxValue2 = defaultEmptyValue;

        for (int i = 0; i < frameLength; i++)
        {
            // Not interpolate if any of the frames is empty
            if (frameA.size() == 0 || frameB.size() == 0)
            {
                interpolatedFrame[i] = defaultEmptyValue;
            }
            else
            {
                if (frameType == FrameType::Magnitudes)
                {
                    if (i < frameA.size() and abs (frameA[i]) != 0.0)
                        auxValue1 = frameA[i];
                    else
                        auxValue1 = defaultEmptyValue;

                    if (i < frameB.size() and abs (frameB[i]) != 0.0)
                        auxValue2 = frameB[i];
                    else
                        auxValue2 = defaultEmptyValue;
                }
                else
                {
                    if (i < frameA.size())
                        auxValue1 = frameA[i];
                    else
                        auxValue1 = defaultEmptyValue;

                    if (i < frameB.size())
                        auxValue2 = frameB[i];
                    else
                        auxValue2 = defaultEmptyValue;
                }

                interpolatedFrame[i] = interpolationFactor * auxValue2 + (1 - interpolationFactor) * auxValue1;
            }
        }
    }

    return interpolatedFrame;
}

bool Instrument::hasAnyLoadedSound (const Note& note) const
{
    for (const auto& sound : note)
    {
        if (sound && sound->isLoaded())
        {
            return true;
        }
    }

    return false;
}
} // namespace morphex
