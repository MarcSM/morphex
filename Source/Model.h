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
    
    Model() : values(_values) {}
    
    bool hasHarmonic() { return this->_values.harmonic.freqs.size() > 0; };
    bool hasSinusoidal() { return this->_values.sinusoidal.freqs.size() > 0; };
    bool hasPhases(Values::FMP fmp_values) { return fmp_values.phases.size() > 0; };
    bool hasStochastic() { return this->_values.stochastic.size() > 0; };
    bool hasAttack() { return this->_values.attack.size() > 0; };
    bool hasResidual() { return this->_values.residual.size() > 0; };
    
    void setHarmonic (std::vector<std::vector<long long>> harmonic_freqs_int,
                      std::vector<std::vector<long long>> harmonic_mags_int,
                      bool decode = false )
    {
        if (decode)
        {
            Codec::decodeMatrixDiff (harmonic_freqs_int);
            Codec::decodeMatrixDiff (harmonic_mags_int);
        }
        
        std::vector<std::vector<float>> harmonic_freqs;
        std::vector<std::vector<float>> harmonic_mags;
        
        Tools::Cast::matrix (harmonic_freqs_int, harmonic_freqs);
        Tools::Cast::matrix (harmonic_mags_int, harmonic_mags);
        
        this->setHarmonicFrequencies (harmonic_freqs, decode);
        this->setHarmonicMagnitudes (harmonic_mags, decode);

//        this->_values.harmonic_freqs = harmonic_freqs;
//        this->_values.harmonic_mags = harmonic_mags;
//
//        // Matrices decoding
//        if (decode) Codec::decodeMatrix(this->_values.harmonic_freqs);
//        if (decode) Codec::decodeMatrix(this->_values.harmonic_mags, true);
    }
    
    void setHarmonicFrequencies (std::vector<std::vector<float>> harmonic_freqs, bool decode = false)
    {
        this->_values.harmonic.freqs = harmonic_freqs;
        
        // Matrix decoding
        if (decode) Codec::decodeMatrix(this->_values.harmonic.freqs, DEFAULT_HZ);
    }
    
    void setHarmonicMagnitudes (std::vector<std::vector<float>> harmonic_mags, bool decode = false)
    {
        this->_values.harmonic.mags = harmonic_mags;
        
        // Matrix decoding
        if (decode) Codec::decodeMatrix(this->_values.harmonic.mags, DEFAULT_DB, true);
    }
    
    void setHarmonicPhases (std::vector<std::vector<float>> harmonic_phases, bool decode = false)
    {
        this->_values.harmonic.phases = harmonic_phases;
        
        // Matrix decoding
        //if (decode) Codec::decodeMatrix(this->_values.harmonic.phases);
    }
    
    void setSinusoidal (std::vector<std::vector<long long>> sinusoidal_freqs_int,
                        std::vector<std::vector<long long>> sinusoidal_mags_int,
                        bool decode = false )
    {
        if (decode)
        {
            Codec::decodeMatrixDiff (sinusoidal_freqs_int);
            Codec::decodeMatrixDiff (sinusoidal_mags_int);
        }
        
        std::vector<std::vector<float>> sinusoidal_freqs;
        std::vector<std::vector<float>> sinusoidal_mags;
        
        Tools::Cast::matrix (sinusoidal_freqs_int, sinusoidal_freqs);
        Tools::Cast::matrix (sinusoidal_mags_int, sinusoidal_mags);
        
        this->setSinusoidalFrequencies (sinusoidal_freqs, decode);
        this->setSinusoidalMagnitudes (sinusoidal_mags, decode);
    }
    
    void setSinusoidalFrequencies (std::vector<std::vector<float>> sinusoidal_freqs, bool decode = false)
    {
        this->_values.sinusoidal.freqs = sinusoidal_freqs;
        
        // Matrix decoding
        if (decode) Codec::decodeMatrix(this->_values.sinusoidal.freqs, DEFAULT_HZ);
    }
    
    void setSinusoidalMagnitudes (std::vector<std::vector<float>> sinusoidal_mags, bool decode = false)
    {
        this->_values.sinusoidal.mags = sinusoidal_mags;
        
        // Matrix decoding
        if (decode) Codec::decodeMatrix(this->_values.sinusoidal.mags, DEFAULT_DB, true);
    }
    
    void setSinusoidalPhases (std::vector<std::vector<float>> sinusoidal_phases, bool decode = false)
    {
        //this->_values.sinusoidal.phases = sinusoidal_phases;
        
        // Matrix decoding
        //if (decode) Codec::decodeMatrix(this->_values.sinusoidal.phases);
    }
    
    void setStochastic (std::vector<std::vector<float>> stochastic, bool decode = false)
    {
        //this->_values.stochastic = stochastic;
        
        // Matrix decoding
        //if (decode) Codec::decodeMatrix(this->_values.stochastic, DEFAULT_HZ, true);
    }
    
    void setAttack (std::vector<float> attack, bool decode = false)
    {
        //this->_values.attack = attack;
        
        // Vector decoding
        //if (decode) Codec::decodeVector(this->_values.attack);
    }
    
    void setResidual (std::vector<float> residual, bool decode = false)
    {
        //this->_values.residual = residual;
        
        // Vector decoding
        //if (decode) Codec::decodeVector(this->_values.residual);
    }
    
private:
    
    Values _values;
};
