/*
 ==============================================================================
 
 Synthesis.h
 Created: 19 Jul 2019 7:42:42pm
 Author:  Marc Sanchez Martinez
 
 ==============================================================================
 */

#pragma once

#include "JuceHeader.h"

#include <vector>

class SynthesisEngine
{
public:
    
    dsp::FFT* fft;
    std::vector<float> window;
    
    SynthesisEngine();
    ~SynthesisEngine();
    
    std::vector<float> sineModelSynth(std::vector<std::vector<float>> frequencies,
                                      std::vector<std::vector<float>> magnitudes,
                                      std::vector<std::vector<float>> phases,
                                      int N, int H,
                                      std::vector<float> synth_window,
                                      int sample_rate);
    
    std::vector<float> stochasticModelSynth(std::vector<std::vector<float>> stocEnv, int N, int H);
    
private:
    
    std::vector<float> generateWindow();
    
    // FFT values
    int fftOrder;
    
    // Synthesis values
    std::vector<double> ow;
    std::vector<double> bh;
    std::vector<double> d_synth_window;
};
