/*
  ==============================================================================

    Model.h
    Created: 22 Apr 2020 1:29:05pm
    Author:  Marc Sanchez Martinez

  ==============================================================================
*/

#pragma once

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
    
    void setHarmonic(std::vector<std::vector<float>> harmonic_freqs,
                     std::vector<std::vector<float>> harmonic_mags,
                     bool decode = false )
    {
        this->setHarmonicFrequencies(harmonic_freqs, decode);
        this->setHarmonicMagnitudes(harmonic_mags, decode);

//        this->_values.harmonic_freqs = harmonic_freqs;
//        this->_values.harmonic_mags = harmonic_mags;
//
//        // Matrices decoding
//        if (decode) Codec::decodeMatrix(this->_values.harmonic_freqs);
//        if (decode) Codec::decodeMatrix(this->_values.harmonic_mags, true);
    }
    
    void setHarmonicFrequencies(std::vector<std::vector<float>> harmonic_freqs, bool decode = false)
    {
        this->_values.harmonic.freqs = harmonic_freqs;
        
        // Matrix decoding
        if (decode) Codec::decodeMatrix(this->_values.harmonic.freqs);
    }
    
    void setHarmonicMagnitudes(std::vector<std::vector<float>> harmonic_mags, bool decode = false)
    {
        this->_values.harmonic.mags = harmonic_mags;
        
        // Matrix decoding
        if (decode) Codec::decodeMatrix(this->_values.harmonic.mags, true);
    }
    
    void setHarmonicPhases(std::vector<std::vector<float>> harmonic_phases, bool decode = false)
    {
        this->_values.harmonic.phases = harmonic_phases;
        
        // Matrix decoding
        if (decode) Codec::decodeMatrix(this->_values.harmonic.phases);
    }
    
    void setSinusoidal(std::vector<std::vector<float>> sinusoidal_freqs,
                       std::vector<std::vector<float>> sinusoidal_mags,
                       bool decode = false )
    {
        this->setSinusoidalFrequencies(sinusoidal_freqs, decode);
        this->setSinusoidalMagnitudes(sinusoidal_mags, decode);
    }
    
    void setSinusoidalFrequencies(std::vector<std::vector<float>> sinusoidal_freqs, bool decode = false)
    {
        this->_values.sinusoidal.freqs = sinusoidal_freqs;
        
        // Matrix decoding
        if (decode) Codec::decodeMatrix(this->_values.sinusoidal.freqs);
    }
    
    void setSinusoidalMagnitudes(std::vector<std::vector<float>> sinusoidal_mags, bool decode = false)
    {
        this->_values.sinusoidal.mags = sinusoidal_mags;
        
        // Matrix decoding
        if (decode) Codec::decodeMatrix(this->_values.sinusoidal.mags, true);
    }
    
    void setSinusoidalPhases(std::vector<std::vector<float>> sinusoidal_phases, bool decode = false)
    {
        this->_values.sinusoidal.phases = sinusoidal_phases;
        
        // Matrix decoding
        if (decode) Codec::decodeMatrix(this->_values.sinusoidal.phases);
    }
    
    void setStochastic(std::vector<std::vector<float>> stochastic, bool decode = false)
    {
        this->_values.stochastic = stochastic;
        
        // Matrix decoding
        if (decode) Codec::decodeMatrix(this->_values.stochastic, true);
    }
    
    void setAttack(std::vector<float> attack, bool decode = false)
    {
        this->_values.attack = attack;
        
        // Vector decoding
        if (decode) Codec::decodeVector(this->_values.attack);
    }
    
    void setResidual(std::vector<float> residual, bool decode = false)
    {
        this->_values.residual = residual;
        
        // Vector decoding
        if (decode) Codec::decodeVector(this->_values.residual);
    }
    
private:
    
    Values _values;
};
