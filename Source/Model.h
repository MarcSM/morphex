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
    bool phases;
    bool sinusoidal;
    bool stochastic;
    bool residual;
    
    /** Sound parameters of the actual ".had" file */
    struct Values
    {
        std::vector<std::vector<float>> harmonics_freqs;
        std::vector<std::vector<float>> harmonics_mags;
        std::vector<std::vector<float>> harmonics_phases;
        std::vector<std::vector<float>> sinusoidal;
        std::vector<std::vector<float>> stochastic;
        std::vector<float> residual;
    };
    const Values &values;
    
    Model() : values(_values) {}
    
    void setHarmonics(std::vector<std::vector<float>> harmonics_freqs,
                      std::vector<std::vector<float>> harmonics_mags,
                      bool decode = false )
    {
        this->setFrequencies(harmonics_freqs, decode);
        this->setMagnitudes(harmonics_mags, decode);

//        this->_values.harmonics_freqs = harmonics_freqs;
//        this->_values.harmonics_mags = harmonics_mags;
//
//        // Matrices decoding
//        if (decode) Codec::decodeMatrix(this->_values.harmonics_freqs);
//        if (decode) Codec::decodeMatrix(this->_values.harmonics_mags, true);
    }
    
    void setFrequencies(std::vector<std::vector<float>> harmonics_freqs, bool decode = false)
    {
        this->_values.harmonics_freqs = harmonics_freqs;
        
        // Matrix decoding
        if (decode) Codec::decodeMatrix(this->_values.harmonics_freqs);
    }
    
    void setMagnitudes(std::vector<std::vector<float>> harmonics_mags, bool decode = false)
    {
        this->_values.harmonics_mags = harmonics_mags;
        
        // Matrix decoding
        if (decode) Codec::decodeMatrix(this->_values.harmonics_mags, true);
    }
    
    void setPhases(std::vector<std::vector<float>> harmonics_phases, bool decode = false)
    {
        this->_values.harmonics_phases = harmonics_phases;
        
        // Matrix decoding
        if (decode) Codec::decodeMatrix(this->_values.harmonics_phases);
    }
    
    void setSinusoidal(std::vector<std::vector<float>> sinusoidal, bool decode = false)
    {
        this->_values.sinusoidal = sinusoidal;
        
        // Matrix decoding
        if (decode) Codec::decodeMatrix(this->_values.sinusoidal);
    }
    
    void setStochastic(std::vector<std::vector<float>> stochastic, bool decode = false)
    {
        this->_values.stochastic = stochastic;
        
        // Matrix decoding
        if (decode) Codec::decodeMatrix(this->_values.stochastic, true);
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
