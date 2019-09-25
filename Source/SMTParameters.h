/*
  ==============================================================================

    SMTParameters.h
    Created: 14 Jun 2019 7:07:41pm
    Author:  Marc Sanchez Martinez

  ==============================================================================
*/

#pragma once

// TODO - Put everything in a namespace called "Morphex::Parameters"

struct ParameterStruct
{
    String parameter_ID;
    String parameter_label;
    float min_value;
    float max_value;
    float default_value;
};

// Rename them and call them like: SMTParameter::FreqsInterpFactor
enum SMTParameter
{
    kParameter_freqs_interp_factor = 0,
    kParameter_mags_interp_factor,
    kParameter_stocs_interp_factor,
    kParameter_stocs_gain,
    kParameter_asdr_attack,
    kParameter_asdr_decay,
    kParameter_asdr_sustain,
    kParameter_asdr_release,
    kParameter_TotalNumParameters
};

typedef std::map<int, ParameterStruct> IndexedParameters;

//static std::map<int, ParameterStruct> smt_parameters =
static IndexedParameters SMT_PARAMETERS =
{
//  index                               parameter_ID            parameter_label             min_value   max_value   default_value
    {kParameter_freqs_interp_factor,    {"FreqsInterpFactor",   "Harmonic Frequencies",     0.0f,       1.0f,       0.5f}    },
    {kParameter_mags_interp_factor,     {"MagsInterpFactor",    "Harmonic Magnitudes",      0.0f,       1.0f,       0.5f}    },
    {kParameter_stocs_interp_factor,    {"StocsInterpFactor",   "Stochastic Component",     0.0f,       1.0f,       0.5f}    },
    {kParameter_stocs_gain,             {"StocsGain",           "Stochastic Gain",          0.0f,       1.0f,       0.0f}    },
    {kParameter_asdr_attack,            {"ADSRAttack",          "Attack",                   0.1f,       5.0f,       0.1f}    },
    {kParameter_asdr_decay,             {"ADSRDecay",           "Decay",                    0.1f,       2.0f,       0.8f}    },
    {kParameter_asdr_sustain,           {"ADSRSustain",         "Sustain",                  0.1f,       5.0f,       0.8f}    },
    {kParameter_asdr_release,           {"ADSRRelease",         "Release",                  0.1f,       5.0f,       0.1f}    }
};

inline ParameterStruct getParameterByID(String parameter_ID)
{
    ParameterStruct found_parameter;
    
    auto result = std::find_if( SMT_PARAMETERS.begin(), SMT_PARAMETERS.end(), [parameter_ID](const auto& mo)
    {
        return mo.second.parameter_ID == parameter_ID;
    });
    
    // Return variable if found
    if(result != SMT_PARAMETERS.end())
        found_parameter = result->second;
    
    return found_parameter;
}

static String SMTParameterID [kParameter_TotalNumParameters] =
{
    "FreqsInterpFactor",
    "MagsInterpFactor",
    "StocsInterpFactor",
    "StocsGain",
    "ADSRAttack",
    "ADSRDecay",
    "ADSRSustain",
    "ADSRRelease",
};

static String SMTParameterLabel [kParameter_TotalNumParameters] =
{
    "Harmonic Frequencies",
    "Harmonic Magnitudes",
    "Stochastic Component",
    "Stochastic Gain",
    "Attack",
    "Decay",
    "Sustain",
    "Release"
};
