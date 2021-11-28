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

namespace morphex
{
class Synth : public juce::Synthesiser
{
public:
    Synth (juce::AudioProcessorValueTreeState& parameters);
//    ~Synth();

    void renderNextBlock (juce::AudioBuffer<float>& outputAudio, const juce::MidiBuffer& inputMidi, int startSample, int numSamples);
    void reset();

    static constexpr auto MaxVoices = 10;

    // TODO - Make private
    Instrument m_instrument;
    
private:
    juce::AudioProcessorValueTreeState& m_parameters;

    dsp::Reverb m_reverb;
    dsp::Gain   m_outputGain;
};
}; // namespace moprhex
