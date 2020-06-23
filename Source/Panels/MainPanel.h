/*
  ==============================================================================

    MainPanel.h
    Created: 12 May 2020 11:14:36am
    Author:  Marc Sanchez Martinez

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include "MorphingPanel.h"

class MainPanel : public Component
{
public:
    
    MainPanel(SpectralMorphingToolAudioProcessor* inProcessor) : morphingPanel (inProcessor)
    {
        addAndMakeVisible (morphingPanel);
    }

    ~MainPanel() {}

    void paint (Graphics& g) override {}

    void resized() override
    {
        FlexBox fb;

        FlexItem morphing (getWidth(), getHeight(), morphingPanel);

        fb.items.addArray ({morphing});
        fb.performLayout (getLocalBounds().toFloat());
    }

private:

    MorphingPanel morphingPanel;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainPanel)
};
