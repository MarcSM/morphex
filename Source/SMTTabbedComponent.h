/*
  ==============================================================================

    SMTTabbedComponentComponent.h
    Created: 11 Aug 2019 11:09:13am
    Author:  Sanchez Martinez, Marc

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"

#include "PluginProcessor.h"

class SMTTabbedComponent
:   public TabbedComponent
{
public:
    
    SMTTabbedComponent(TabbedButtonBar::Orientation orientation);
    ~SMTTabbedComponent();
    
    void paint(Graphics& g) override;
    
private:
    
    SpectralMorphingToolAudioProcessor* mProcessor;
};
