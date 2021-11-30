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

namespace Constants
{
constexpr auto MeterSmoothingCoeff = 0.02;
} // namespace Constants

namespace morphex
{
namespace dsp
{
    class Gain
    {
    public:
        Gain (juce::AudioProcessorValueTreeState& parameters);

        void prepare (juce::dsp::ProcessSpec& spec);
        void process (juce::dsp::ProcessContextReplacing<float>& context);

        float getMeterLevel();

    private:
        void updateParameters();

        juce::AudioProcessorValueTreeState& m_parameters;

        juce::dsp::Gain<float> m_gain;

        float m_outputSmoothed;
    };
}; // namespace dsp
}; // namespace morphex
