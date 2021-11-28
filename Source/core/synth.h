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

#include <JuceHeader.h>

#include "instrument.h"
#include "sound.h"
#include "voice.h"

#include "../dsp/gain.h"
#include "../dsp/reverb.h"

namespace Core
{
class MorphexSynth : public Synthesiser
{
public:
    MorphexSynth (AudioProcessorValueTreeState& parameters);
    ~MorphexSynth();

    void setCurrentPlaybackSampleRate (double sampleRate) override;
    void renderNextBlock (AudioBuffer<float>& outputAudio, const MidiBuffer& inputMidi, int startSample, int numSamples);
    void reset();

    Instrument m_instrument;

protected:
    OwnedArray<Voice> m_voices;

private:
    AudioProcessorValueTreeState& m_parameters;

    morphex::dsp::Reverb m_reverb;
    morphex::dsp::Gain   m_outputGain;
};
}; // namespace Core
