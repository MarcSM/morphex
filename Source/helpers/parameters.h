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

// TODO - Put everything in a namespace called "Morphex::Parameters"

namespace Morphex
{
template <typename T>
struct Parameter
{
    juce::String ID;
    juce::String label;
    T      min_value;
    T      max_value;
    T      default_value;
};

// Rename them and call them like: SMTParameter::FreqsInterpFactor
enum Parameters
{
    OutputGain = 0,
    ReverbDryWet,
    FreqsInterpFactor,
    MagsInterpFactor,
    StocsInterpFactor,
    StocsGain,
    AsdrAttack,
    AsdrDecay,
    AsdrSustain,
    AsdrRelease,
    TotalNumParameters
};

//typedef std::map<int, Parameter> IndexedParameters;ç

//    params.push_back(std::make_unique<AudioParameterFloat>(things));
//    params.push_back(std::make_unique<AudioParameterBool>(things));

template <typename T>
using IndexedParameters = std::map<int, Parameter<T>>;

//static std::map<int, Parameter> smt_parameters =
template <typename T>
static IndexedParameters<T> PARAMETERS = {
    //  index                   ID                      label                       min_value   max_value   default_value
    { OutputGain, { "OutputGain", "Master", 0.0f, 1.0f, 0.5f } },
    { ReverbDryWet, { "ReverbDryWet", "Dry/Wet", 0.0f, 1.0f, 0.15f } },
    { FreqsInterpFactor, { "FreqsInterpFactor", "Harmonic Frequencies", 0.0f, 1.0f, 0.5f } },
    { MagsInterpFactor, { "MagsInterpFactor", "Harmonic Magnitudes", 0.0f, 1.0f, 0.5f } },
    { StocsInterpFactor, { "StocsInterpFactor", "Stochastic Component", 0.0f, 1.0f, 0.5f } },
    { StocsGain, { "StocsGain", "Stochastic Gain", 0.0f, 1.0f, 0.0f } },
    { AsdrAttack, { "ADSRAttack", "Attack", 0.1f, 5.0f, 0.1f } },
    { AsdrDecay, { "ADSRDecay", "Decay", 0.1f, 2.0f, 0.8f } },
    { AsdrSustain, { "ADSRSustain", "Sustain", 0.1f, 5.0f, 0.8f } },
    { AsdrRelease, { "ADSRRelease", "Release", 0.1f, 5.0f, 0.1f } }
};

template <typename T>
inline Parameter<T> getParameterByID (juce::String parameter_ID)
{
    Parameter<T> found_parameter;

    auto result = std::find_if (PARAMETERS<T>.begin(), PARAMETERS<T>.end(), [parameter_ID] (const auto& mo) {
        //            return mo.second.parameter_ID == parameter_ID;
        return mo.second.ID == parameter_ID;
    });

    // Return variable if found
    if (result != PARAMETERS<T>.end())
        found_parameter = result->second;

    return found_parameter;
}
} // namespace Morphex
