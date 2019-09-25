/*
  ==============================================================================

    SMTAdditionalParameters.h
    Created: 11 Aug 2019 11:53:14am
    Author:  Sanchez Martinez, Marc

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"

#include "PluginProcessor.h"

#include "SMTParameterSlider.h"

class SMTAdditionalParameters:
    public Component
{
public:
    
    SMTAdditionalParameters(SpectralMorphingToolAudioProcessor* inProcessor);
    
    ~SMTAdditionalParameters();
    
    void paint(Graphics& g) override;
    
private:
    
    SpectralMorphingToolAudioProcessor* mProcessor;
    
    OwnedArray<SMTParameterSlider> mSliders;
    
    SMTParameterSlider* stocs_interp_factor;
    SMTParameterSlider* stocs_gain;
};
