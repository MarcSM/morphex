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

#include "gain.h"
#include "../core/parameters.h"

namespace morphex
{
namespace dsp
{
    Gain::Gain (juce::AudioProcessorValueTreeState& parameters) :
        m_parameters (parameters),
        m_outputSmoothed (0)
    {
    }

    void Gain::prepare (juce::dsp::ProcessSpec& spec)
    {
        m_gain.prepare (spec);
    }

    void Gain::process (juce::dsp::ProcessContextReplacing<float>& context)
    {
        updateParameters();

        m_gain.process (context);

        auto absValue    = fabs (context.getOutputBlock().getSample (0, 0));
        m_outputSmoothed = Constants::MeterSmoothingCoeff * (m_outputSmoothed - absValue) + absValue;
    }

    float Gain::getMeterLevel()
    {
        return m_outputSmoothed;
    }

    void Gain::updateParameters()
    {
        auto outputGain = m_parameters.getRawParameterValue (PARAMETERS<float>[Parameters::OutputGain].ID)->load();
        m_gain.setGainLinear (outputGain);
    }
}; // namespace dsp
}; // namespace morphex
