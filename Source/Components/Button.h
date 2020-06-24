/*
  ==============================================================================

    Button.h
    Created: 2 Jun 2020 1:18:59pm
    Author:  Marc Sanchez Martinez

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include "../Panels/MorphexLookAndFeel.h"

namespace Morphex { class Button; }

class Morphex::Button : public juce::TextButton
{
public:
    
    Button() : juce::TextButton (String()) {}

    ~Button() {}
    
    void paintButton (Graphics& g,
                      bool shouldDrawButtonAsHighlighted,
                      bool shouldDrawButtonAsDown)
    {
        auto& lf = getLookAndFeel();
        
        lf.drawButtonBackground (g, *this,
                                 findColour (getToggleState() ? buttonOnColourId : buttonColourId),
                                 shouldDrawButtonAsHighlighted, shouldDrawButtonAsDown);
        
        lf.drawButtonText (g, *this, shouldDrawButtonAsHighlighted, shouldDrawButtonAsDown);

        if (icon.isNotEmpty())
        {
            g.setColour (findColour (getToggleState() ? TextButton::textColourOnId
                                     : TextButton::textColourOffId)
                         .withMultipliedAlpha (isEnabled() ? 1.0f : 0.5f));
            const float icon_size = jmin (getWidth(), getHeight()) * 0.6f;
            FontAwesome::drawCenterd (g, icon, icon_size, Colours::white, getLocalBounds());
        }
    }
    
    void setIcon (Icon new_icon)
    {
        icon = new_icon;
    }

private:
    
    Icon icon;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Button)
};
