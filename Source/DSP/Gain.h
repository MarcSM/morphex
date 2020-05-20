/*
  ==============================================================================

    Gain.h
    Created: 20 May 2020 12:44:29pm
    Author:  Marc Sanchez Martinez

  ==============================================================================
*/

#pragma once

#define kMeterSmoothingCoeff 0.02

namespace DSP { class Gain; }

class DSP::Gain
{
public:
    
    Gain()
    {
        mOutputSmoothed = 0;
    }
    
    ~Gain() {};
    
    void process(float* buffer, float inGain, int inNumSamplesToRender)
    {
        float gainMapped = jmap(inGain, 0.0f, 1.0f, -24.0f, 24.0f);
        gainMapped = Decibels::decibelsToGain(gainMapped, -24.0f);
        
        for(int i = 0; i < inNumSamplesToRender; i++) {
            buffer[i] = buffer[i] * gainMapped;
        }
        
        float absValue = fabs(buffer[0]);
        mOutputSmoothed = kMeterSmoothingCoeff * (mOutputSmoothed - absValue) + absValue;
    }
    
    float getMeterLevel()
    {
        return mOutputSmoothed;
    }
    
private:
    
    float mOutputSmoothed;
};
