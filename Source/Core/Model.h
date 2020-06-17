/*
  ==============================================================================

    Model.h
    Created: 22 Apr 2020 1:29:05pm
    Author:  Marc Sanchez Martinez

  ==============================================================================
*/

#pragma once

#include "Tools.h"
#include "Codec.h"

namespace Core { class Model; }
    
class Core::Model
{
public:
    
    bool harmonic;
    bool sinusoidal;
    bool stochastic;
    bool attack;
    bool residual;
    
    /** Sound parameters of the actual ".had" file */
    struct Values
    {
        struct FMP
        {
//            enum Type
//            {
//                Harmonic = 0,
//                Sinusoidal
//            };
            
            std::vector<std::vector<float>> freqs;
            std::vector<std::vector<float>> mags;
            std::vector<std::vector<float>> phases;
        };
        
        FMP harmonic;
        FMP sinusoidal;
        std::vector<std::vector<float>> stochastic;
        std::vector<float> attack;
        std::vector<float> residual;
    };
    const Values &values;
    
    Model() : values (_values) {}

//    Model (const Model& obj) : values (obj.values) {};
    
//    Model (const Model& obj) : values (_values)
//    Model (const Model& obj) : values (obj._values)
    
//    Model (const Model& obj) : values (obj._values) {}
    
    //        this->_values = obj.values;
    
//    Model (const Model& obj) : values (_values) {};
//    {
//        this->_values = obj.values;
//    }
    
    void reset()
    {
        this->_values.attack.resize(0);
        this->_values.residual.resize(0);
        
        // Reset harmonic
        this->_values.harmonic.freqs.resize(0);
        this->_values.harmonic.mags.resize(0);
        this->_values.harmonic.phases.resize(0);

        // Reset sinusoidal
        this->_values.sinusoidal.freqs.resize(0);
        this->_values.sinusoidal.mags.resize(0);
        this->_values.sinusoidal.phases.resize(0);

        // Reset the rest of components
        this->_values.stochastic.resize(0);
        this->_values.attack.resize(0);
        this->_values.residual.resize(0);
    }
    
    bool hasHarmonic() { return this->_values.harmonic.freqs.size() > 0; };
    bool hasSinusoidal() { return this->_values.sinusoidal.freqs.size() > 0; };
    bool hasPhases (Values::FMP fmp_values) { return fmp_values.phases.size() > 0; };
    bool hasStochastic() { return this->_values.stochastic.size() > 0; };
    bool hasAttack() { return this->_values.attack.size() > 0; };
    bool hasResidual() { return this->_values.residual.size() > 0; };

    void setHarmonic (std::vector<std::vector<long long>> harmonic_freqs_int,
                      std::vector<std::vector<long long>> harmonic_mags_int,
                      bool decode = false )
    {
        if (decode) Codec::decodeMatrixDiff (harmonic_freqs_int);
        if (decode) Codec::decodeMatrixDiff (harmonic_mags_int);

        std::vector<std::vector<float>> harmonic_freqs;
        std::vector<std::vector<float>> harmonic_mags;

        Tools::Cast::matrix (harmonic_freqs_int, harmonic_freqs);
        Tools::Cast::matrix (harmonic_mags_int, harmonic_mags);

        if (decode) Codec::decodeMatrix (harmonic_freqs, false, false);
        if (decode) Codec::decodeMatrix (harmonic_mags, true, false);
        
        // TODO - For safety, and just in case, check if all elements in a magnitude frame are 0.0
        // if so, put them to DEFAULT_DB

        this->setHarmonicFrequencies (harmonic_freqs);
        this->setHarmonicMagnitudes (harmonic_mags);
    }

    void setHarmonicFrequencies (std::vector<std::vector<float>> harmonic_freqs, bool decode = false)
    {
        this->_values.harmonic.freqs = harmonic_freqs;

        // Matrix decoding
        if (decode) Codec::decodeMatrix (this->_values.harmonic.freqs);

        // TODO - Sanity check, don't trsut the "had", check if there are any anomalies and,
        // in such case, abort loading the sound, rise and exeption, and display a message to the user.
    }

    void setHarmonicMagnitudes (std::vector<std::vector<float>> harmonic_mags, bool decode = false)
    {
        this->_values.harmonic.mags = harmonic_mags;

        // Matrix decoding
        if (decode) Codec::decodeMatrix (this->_values.harmonic.mags, true);
    }

    void setHarmonicPhases (std::vector<std::vector<float>> harmonic_phases, bool decode = false)
    {
        this->_values.harmonic.phases = harmonic_phases;

        // Matrix decoding
        if (decode) Codec::decodeMatrix (this->_values.harmonic.phases);
    }

    void setSinusoidal (std::vector<std::vector<long long>> sinusoidal_freqs_int,
                        std::vector<std::vector<long long>> sinusoidal_mags_int,
                        bool decode = false )
    {
        if (decode) Codec::decodeMatrixDiff (sinusoidal_freqs_int);
        if (decode) Codec::decodeMatrixDiff (sinusoidal_mags_int);

        std::vector<std::vector<float>> sinusoidal_freqs;
        std::vector<std::vector<float>> sinusoidal_mags;

        Tools::Cast::matrix (sinusoidal_freqs_int, sinusoidal_freqs);
        Tools::Cast::matrix (sinusoidal_mags_int, sinusoidal_mags);

        if (decode) Codec::decodeMatrix (sinusoidal_freqs, false, false);
        if (decode) Codec::decodeMatrix (sinusoidal_mags, true, false);

        this->setSinusoidalFrequencies (sinusoidal_freqs);
        this->setSinusoidalMagnitudes (sinusoidal_mags);
    }

    void setSinusoidalFrequencies (std::vector<std::vector<float>> sinusoidal_freqs, bool decode = false)
    {
        // Matrix decoding
        if (decode) Codec::decodeMatrix (sinusoidal_freqs);

        this->_values.sinusoidal.freqs = sinusoidal_freqs;
    }

    void setSinusoidalMagnitudes (std::vector<std::vector<float>> sinusoidal_mags, bool decode = false)
    {
        // Matrix decoding
        if (decode) Codec::decodeMatrix (sinusoidal_mags, true);

        this->_values.sinusoidal.mags = sinusoidal_mags;
    }

    void setSinusoidalPhases (std::vector<std::vector<float>> sinusoidal_phases, bool decode = false)
    {
        // Matrix decoding
        if (decode) Codec::decodeMatrix (sinusoidal_phases);

        this->_values.sinusoidal.phases = sinusoidal_phases;
    }

    void setStochastic (std::vector<std::vector<float>> stochastic, bool decode = false)
    {
        // Matrix decoding
        if (decode) Codec::decodeMatrix (stochastic, true);

        this->_values.stochastic = stochastic;
    }

    void setAttack (std::vector<float> attack, bool decode = false)
    {
        // Vector decoding
        if (decode) Codec::decodeVector (attack);

        this->_values.attack = attack;
    }

    void setResidual (std::vector<float> residual, bool decode = false)
    {
        // Vector decoding
        if (decode) Codec::decodeVector (residual);

        this->_values.residual = residual;
    }
    
private:
    
    Values _values;
};
