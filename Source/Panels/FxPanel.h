/*
  ==============================================================================

    FxPanel.h
    Created: 1 Jun 2020 12:24:39pm
    Author:  Marc Sanchez Martinez

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include "../Components/Slider.h"

//==============================================================================
/*
*/
class FxPanel : public Component
{
public:
    FxPanel (SpectralMorphingToolAudioProcessor* inProcessor)
    {
        // TODO - Filters FX
        // TODO - Distortion FX
        // TODO - Delay FX
        
        // Reverb FX
        FxRow* reverb_panel = new FxRow (inProcessor, "Reverb");
        addAndMakeVisible (mFxRows.add (reverb_panel));
        
        // Reverb FX - Dry / Wet
        Morphex::Slider* reverb_drywet_slider = new Morphex::Slider (inProcessor->parameters, Morphex::Parameters::ReverbDryWet);
        addAndMakeVisible (reverb_panel->mSliders.add (reverb_drywet_slider));
    }

    ~FxPanel()
    {
    }

    void paint (Graphics& g) override
    {
        g.fillAll (Colours::black);
        
        // Draw borders
        GUI::Paint::drawBorders (g, getLocalBounds());
    }

    void resized() override
    {
        auto bounds = getLocalBounds();
        auto row_height = bounds.getHeight() * 0.18;
        
        for (int i = 0; i < mFxRows.size(); ++i)
        {
            mFxRows[i]->setBounds (0, row_height * i, getWidth(), row_height);
        }
    }

private:
    
    struct FxRow : public Component
    {
        FxRow (SpectralMorphingToolAudioProcessor* inProcessor, String name)
        :   name (name)
        {
            mSliders.clear();
        }
        
        void paint (Graphics& g) override
        {
            g.fillAll (GUI::Color::BackgroundDark);
            g.setColour (Colours::white);
            g.setFont (14.0f);
            g.drawText (name, getLocalBounds(), Justification::topLeft, true);
            
            // Draw borders
            GUI::Paint::drawBorders(g, getLocalBounds());
        }
        
        void resized() override
        {
            FlexBox fb;
            fb.flexDirection = FlexBox::Direction::column;
            fb.flexWrap = FlexBox::Wrap::wrap;
            fb.justifyContent = FlexBox::JustifyContent::flexEnd;
            
            FlexItem drywet_knob (getWidth(), getHeight(), *mSliders[0]);
            
            fb.items.addArray ({drywet_knob});
            fb.performLayout (getLocalBounds().toFloat());
        }
        
        String name;
        
        OwnedArray<Slider> mSliders;
    };
    
    OwnedArray<FxRow> mFxRows;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FxPanel)
};
