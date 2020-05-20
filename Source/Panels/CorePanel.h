/*
  ==============================================================================

    CorePanel.h
    Created: 14 May 2020 2:50:54pm
    Author:  Marc Sanchez Martinez

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include "../Components/PadXY.h"

//==============================================================================
/*
*/
class CorePanel    : public Component
{
public:
    CorePanel(SpectralMorphingToolAudioProcessor* inProcessor)
    {
        Morphex::Parameter freqs_interp_factor_parameter = Morphex::PARAMETERS<float>[Morphex::Parameters::freqs_interp_factor];
        Morphex::Parameter mags_interp_factor_parameter = Morphex::PARAMETERS<float>[Morphex::Parameters::mags_interp_factor];

        mPadXY = new PadXY(inProcessor, inProcessor->parameters,
                           freqs_interp_factor_parameter,
                           mags_interp_factor_parameter);
        
        addAndMakeVisible(mPadXY);
    }

    ~CorePanel()
    {
    }

    void paint (Graphics& g) override
    {
        
        g.fillAll (Colours::black);
//        g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

        // Background gradient
        ColourGradient cg (GUI::Color::Accent.withAlpha(0.5f),
                           getWidth() * 0.5f, getHeight() * 0.5f,
                           GUI::Color::Accent.withAlpha(0.0f),
                           getWidth() * 0.65f , getHeight() * 1.0f, true);
        
//        getWidth() * 0.75f , getHeight() * 0.75f, true);

        g.setFillType (cg);
        g.fillRect (0, 0, getWidth(), getHeight());
        
        // Circle glow
        PadXY::PadCircle* circle = mPadXY->getCircle();
        float glow_radius = circle->getWidth() * 2.0f;
        float glow_x = circle->getX() + ( circle->getWidth() / 2.0f ) + mPadXY->getX();
        float glow_y = circle->getY() + ( circle->getHeight() / 2.0f ) + mPadXY->getY();
        float glow_w = glow_radius * 2.0f;
        float glow_h = glow_radius * 2.0f;
        
        ColourGradient cg_glow (GUI::Color::Accent.withAlpha(0.3f),
                                glow_x, glow_y,
                                GUI::Color::Accent.withAlpha(0.0f),
                                glow_x + (glow_radius * 0.4f),
                                glow_y + (glow_radius * 0.4f), true);
        
        g.setFillType (cg_glow);
        g.fillRect (0, 0, getWidth(), getHeight());
//        g.fillRect (glow_x - glow_radius, glow_y - glow_radius, glow_w, glow_h);
    }

    void resized() override
    {
        // Pad XY
        const int pad_width = getWidth();
        const int pad_height = getWidth();
        
        const int pad_x = 0;
        const int pad_y = ( getHeight() - pad_height ) / 2;
        
        mPadXY->setBounds(pad_x, pad_y, pad_width, pad_height);
    }

private:
    
    PadXY* mPadXY;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CorePanel)
};
