/*
  ==============================================================================

    SMTPanelBase.h
    Created: 14 Jun 2019 7:38:37pm
    Author:  Marc Sanchez Martinez

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"
#include "PluginProcessor.h"
#include "SMTInterfaceDefines.h"

class SMTPanelBase
:   public Component
{
public:
    
    SMTPanelBase();
//    SMTPanelBase(SpectralMorphingToolAudioProcessor* inProcessor);
    ~SMTPanelBase();
    
    void paint(Graphics& g) override;
    
protected:
    
//    SpectralMorphingToolAudioProcessor* mProcessor;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SMTPanelBase);
};
