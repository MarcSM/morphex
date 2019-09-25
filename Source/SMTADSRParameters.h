/*
  ==============================================================================

    SMTADSRParameters.h
    Created: 17 Aug 2019 1:12:05pm
    Author:  Marc Sanchez Martinez

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"

#include "PluginProcessor.h"

#include "SMTParameterSlider.h"

class SMTADSRParameters
:   public Component
{
public:
    
    SMTADSRParameters(SpectralMorphingToolAudioProcessor* inProcessor);
    
    ~SMTADSRParameters();
    
    void paint(Graphics& g) override;
    
private:
    
    SpectralMorphingToolAudioProcessor* mProcessor;
    
    OwnedArray<SMTParameterSlider> mSliders;
    
    SMTParameterSlider* asdr_attack;
    SMTParameterSlider* asdr_sustain;
    SMTParameterSlider* asdr_decay;
    SMTParameterSlider* asdr_release;
};
