/*
  ==============================================================================

    Reverb.h
    Created: 1 Jun 2020 11:10:16am
    Author:  Marc Sanchez Martinez

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

namespace DSP { class Reverb; }

class DSP::Reverb
{
public:
    
    Reverb (AudioProcessorValueTreeState* parameters)
    :   mParameters (parameters)
    {
//        float output_gain = *mParameters->getRawParameterValue (Morphex::PARAMETERS<float>[Morphex::Parameters::OutputGain].parameter_ID);
        
        updateParameters();
    }
    
    ~Reverb() {}
    
//    void process (float* buffer, int inNumSamplesToRender)
    void process (AudioSampleBuffer& buffer, int inNumSamplesToRender)
    {
        updateParameters();
        
        ScopedNoDenormals noDenormals;
        
        const int totalNumInputChannels  = 2;
        const int totalNumOutputChannels = 2;
        
//        const int totalNumInputChannels  = getTotalNumInputChannels();
//        const int totalNumOutputChannels = getTotalNumOutputChannels();
        
        for (int i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
            buffer.clear (i, 0, inNumSamplesToRender);
        
        // This is the place where you'd normally do the guts of your plugin's
        // audio processing...
        const auto numChannels = jmin (totalNumInputChannels, totalNumOutputChannels);
        
        if (numChannels == 1)
            theReverb.processMono (buffer.getWritePointer (0), inNumSamplesToRender);
        
        else if (numChannels == 2)
            theReverb.processStereo (buffer.getWritePointer (0), buffer.getWritePointer (1), inNumSamplesToRender);
        
//        float gainMapped = jmap(inGain, 0.0f, 1.0f, -24.0f, 24.0f);
//        gainMapped = Decibels::decibelsToGain(gainMapped, -24.0f);
//
//        for(int i = 0; i < inNumSamplesToRender; i++) {
//            buffer[i] = buffer[i] * gainMapped;
//        }
//
//        float absValue = fabs(buffer[0]);
//        mOutputSmoothed = kMeterSmoothingCoeff * (mOutputSmoothed - absValue) + absValue;
    }
    
    void updateParameters()
    {
        reverbParameters.wetLevel = *mParameters->getRawParameterValue (Morphex::PARAMETERS<float>[Morphex::Parameters::ReverbDryWet].ID);
        reverbParameters.dryLevel = 1 - reverbParameters.wetLevel;
        reverbParameters.roomSize = 0.8;
        reverbParameters.damping = 0.1;
        theReverb.setParameters (reverbParameters);
    }
    
private:
    
    AudioProcessorValueTreeState* mParameters;
    
    juce::Reverb theReverb;
    juce::Reverb::Parameters reverbParameters;
};
