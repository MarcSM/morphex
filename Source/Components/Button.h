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

//==============================================================================
/*
*/

namespace Morphex { class Button; }

class Morphex::Button    : public juce::TextButton
{
public:
    
//    enum ColourIds
//    {
//        buttonColourId                  = 0x1000100,  /**< The colour used to fill the button shape (when the button is toggled
//                                                       'off'). The look-and-feel class might re-interpret this to add
//                                                       effects, etc. */
//        buttonOnColourId                = 0x1000101,  /**< The colour used to fill the button shape (when the button is toggled
//                                                       'on'). The look-and-feel class might re-interpret this to add
//                                                       effects, etc. */
//        textColourOffId                 = 0x1000102,  /**< The colour to use for the button's text when the button's toggle state is "off". */
//        textColourOnId                  = 0x1000103   /**< The colour to use for the button's text.when the button's toggle state is "on". */
//    };
    
//    Button (String buttonName)
//    :   juce::Button (buttonName)
    Button ()
    :   juce::TextButton (String())
    {
//        imageBounds.setSize (normalImage.getWidth(),
//                             normalImage.getHeight());
    }

    ~Button()
    {
        juce::TextButton::~TextButton();
    }
    
    void paintButton (Graphics& g,
                      bool shouldDrawButtonAsHighlighted,
                      bool shouldDrawButtonAsDown)
    {
        auto& lf = getLookAndFeel();
        
        lf.drawButtonBackground (g, *this,
                                 findColour (getToggleState() ? buttonOnColourId : buttonColourId),
                                 shouldDrawButtonAsHighlighted, shouldDrawButtonAsDown);
        
        lf.drawButtonText (g, *this, shouldDrawButtonAsHighlighted, shouldDrawButtonAsDown);
        
//        juce::Button::paintButton (g, shouldDrawButtonAsHighlighted, shouldDrawButtonAsDown);
//        LookAndFeel_V4::paintButton (g, shouldDrawButtonAsHighlighted, shouldDrawButtonAsDown);
//        MorphexLookAndFeel::paintButton (g, shouldDrawButtonAsHighlighted, shouldDrawButtonAsDown);

        if (icon.isNotEmpty())
        {
            g.setColour (findColour (getToggleState() ? TextButton::textColourOnId
                                     : TextButton::textColourOffId)
                         .withMultipliedAlpha (isEnabled() ? 1.0f : 0.5f));
            const float icon_size = jmin(getWidth(), getHeight()) * 0.6f;
            FontAwesome::drawCenterd(g, icon, icon_size, Colours::white, getLocalBounds());
        }
    }
    
    void setIcon (Icon new_icon)
    {
        icon = new_icon;
////        setButtonText (icon);
//        Icon test = FontAwesome_Save;
//        setButtonText (test);
//        setButtonText (FontAwesome_Save);
    }

private:
    
    Icon icon;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Button)
};
