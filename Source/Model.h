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
                      std::vector<std::vector<float>> harmonics_mags)
    {
        this->_values.harmonics_freqs = harmonics_freqs;
        this->_values.harmonics_mags = harmonics_mags;
        
        // Matrices and lists decoding
        Codec::decodeMatrix(this->_values.harmonics_freqs, false);
        Codec::decodeMatrix(this->_values.harmonics_mags, true);
    }
    
    void setMagnitudes(std::vector<std::vector<float>> harmonics_mags)
    {
        this->_values.harmonics_mags = harmonics_mags;
    }
    
    void setPhases(std::vector<std::vector<float>> harmonics_phases)
    {
        this->_values.harmonics_phases = harmonics_phases;
    }
    
    void setSinusoidal(std::vector<std::vector<float>> sinusoidal)
    {
        this->_values.sinusoidal = sinusoidal;
    }
    
    void setStochastic(std::vector<std::vector<float>> stochastic)
    {
        this->_values.stochastic = stochastic;
    }
    
    void setResidual(std::vector<float> residual)
    {
        this->_values.residual = residual;
    }
    
private:
    
    Values _values;
};
