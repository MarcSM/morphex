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

//==============================================================================
/*
*/
class MainPanel    : public Component
{
public:
    MainPanel() : morphingPanel ()
    {
        // In your constructor, you should add any child components, and
        // initialise any special settings that your component needs.
        
        addAndMakeVisible (morphingPanel);
    }

    ~MainPanel()
    {
    }

    void paint (Graphics& g) override
    {
        /* This demo code just fills the component's background and
           draws some placeholder text to get you started.

           You should replace everything in this method with your own
           drawing code..
        */

        /*
        g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));   // clear the background

        g.setColour (Colours::grey);
        g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

        g.setColour (Colours::white);
        g.setFont (14.0f);
        g.drawText ("MainPanel", getLocalBounds(),
                    Justification::centred, true);   // draw some placeholder text
        */
    }

    void resized() override
    {
        // This method is where you should set the bounds of any child
        // components that your component contains..
        
        FlexBox fb;

        FlexItem morphing  (getWidth(), getHeight(), morphingPanel);

        fb.items.addArray ( { morphing } );
        fb.performLayout (getLocalBounds().toFloat());
    }

private:

    MorphingPanel morphingPanel;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainPanel)
};
