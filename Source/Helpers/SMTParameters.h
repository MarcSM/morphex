/*
  ==============================================================================

    SMTParameters.h
    Created: 14 Jun 2019 7:07:41pm
    Author:  Marc Sanchez Martinez

  ==============================================================================
*/

#pragma once

// TODO - Put everything in a namespace called "Morphex::Parameters"

namespace Morphex
{
    template<typename T>
    struct Parameter
    {
        String ID;
        String label;
        T min_value;
        T max_value;
        T default_value;
    };

    // Rename them and call them like: SMTParameter::FreqsInterpFactor
    enum Parameters
    {
        OutputGain = 0,
        ReverbDryWet,
        freqs_interp_factor,
        mags_interp_factor,
        stocs_interp_factor,
        stocs_gain,
        asdr_attack,
        asdr_decay,
        asdr_sustain,
        asdr_release,
        TotalNumParameters
    };

    //typedef std::map<int, Parameter> IndexedParameters;ç
    
//    params.push_back(std::make_unique<AudioParameterFloat>(things));
//    params.push_back(std::make_unique<AudioParameterBool>(things));

    template <typename T>
    using IndexedParameters = std::map<int, Parameter<T>>;

    //static std::map<int, Parameter> smt_parameters =
    template <typename T>
    static IndexedParameters<T> PARAMETERS =
    {
    //  index                   ID                      label                       min_value   max_value   default_value
        {OutputGain,            {"OutputGain",          "Master",                   0.0f,       1.0f,       0.5f}  },
        {ReverbDryWet,          {"ReverbDryWet",        "Dry/Wet",                  0.0f,       1.0f,       0.15f}  },
        {freqs_interp_factor,   {"FreqsInterpFactor",   "Harmonic Frequencies",     0.0f,       1.0f,       0.5f}   },
        {mags_interp_factor,    {"MagsInterpFactor",    "Harmonic Magnitudes",      0.0f,       1.0f,       0.5f}   },
        {stocs_interp_factor,   {"StocsInterpFactor",   "Stochastic Component",     0.0f,       1.0f,       0.5f}   },
        {stocs_gain,            {"StocsGain",           "Stochastic Gain",          0.0f,       1.0f,       0.0f}   },
        {asdr_attack,           {"ADSRAttack",          "Attack",                   0.1f,       5.0f,       0.1f}   },
        {asdr_decay,            {"ADSRDecay",           "Decay",                    0.1f,       2.0f,       0.8f}   },
        {asdr_sustain,          {"ADSRSustain",         "Sustain",                  0.1f,       5.0f,       0.8f}   },
        {asdr_release,          {"ADSRRelease",         "Release",                  0.1f,       5.0f,       0.1f}   }
    };

    template<typename T>
    inline Parameter<T> getParameterByID(String parameter_ID)
    {
        Parameter<T> found_parameter;
        
        auto result = std::find_if( PARAMETERS<T>.begin(), PARAMETERS<T>.end(), [parameter_ID](const auto& mo)
        {
//            return mo.second.parameter_ID == parameter_ID;
            return mo.second.ID == parameter_ID;
        });
        
        // Return variable if found
        if(result != PARAMETERS<T>.end())
            found_parameter = result->second;
        
        return found_parameter;
    }
} // namespace Morphex
