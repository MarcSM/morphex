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

#include "reverb.h"
#include "../helpers/parameters.h"

namespace morphex
{
namespace dsp
{
    Reverb::Reverb (juce::AudioProcessorValueTreeState& parameters) :
        m_parameters (parameters)
    {
    }

    void Reverb::prepare (juce::dsp::ProcessSpec& spec)
    {
        m_reverb.prepare (spec);
    }

    void Reverb::process (juce::dsp::ProcessContextReplacing<float>& context)
    {
        updateParameters();

        m_reverb.process (context);
    }

    void Reverb::updateParameters()
    {
        m_reverbParameters.wetLevel = *m_parameters.getRawParameterValue (morphex::PARAMETERS<float>[morphex::Parameters::ReverbDryWet].ID);
        m_reverbParameters.dryLevel = 1 - m_reverbParameters.wetLevel;
        m_reverbParameters.roomSize = 0.8;
        m_reverbParameters.damping  = 0.1;
        m_reverb.setParameters (m_reverbParameters);
    }
}; // namespace dsp
}; // namespace morphex
