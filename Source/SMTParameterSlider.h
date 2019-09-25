/*
  ==============================================================================

    SMTParameterSlider.h
    Created: 14 Jun 2019 7:08:38pm
    Author:  Marc Sanchez Martinez

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"

class SMTParameterSlider
:
    public Slider
{
public:
    
    SMTParameterSlider(AudioProcessorValueTreeState& stateToControl,
                       const String& parameterID,
                       const String& parameterLabel);
    ~SMTParameterSlider();
    
private:
    
    ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> mAttachment;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SMTParameterSlider);
};
