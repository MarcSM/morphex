/*
  ==============================================================================

   This file is part of the JUCE tutorials.
   Copyright (c) 2017 - ROLI Ltd.

   The code included in this file is provided under the terms of the ISC license
   http://www.isc.org/downloads/software-support-policy/isc-license. Permission
   To use, copy, modify, and/or distribute this software for any purpose with or
   without fee is hereby granted provided that the above copyright notice and
   this permission notice appear in all copies.

   THE SOFTWARE IS PROVIDED "AS IS" WITHOUT ANY WARRANTY, AND ALL WARRANTIES,
   WHETHER EXPRESSED OR IMPLIED, INCLUDING MERCHANTABILITY AND FITNESS FOR
   PURPOSE, ARE DISCLAIMED.

  ==============================================================================
*/

/*******************************************************************************
 The block below describes the properties of this PIP. A PIP is a short snippet
 of code that can be read by the Projucer and used to generate a JUCE project.

 BEGIN_JUCE_PIP_METADATA

 name:             FlexBoxGridTutorial
 version:          1.0.0
 vendor:           JUCE
 website:          http://juce.com
 description:      Responsive layouts using FlexBox and Grid.

 dependencies:     juce_core, juce_data_structures, juce_events, juce_graphics,
                   juce_gui_basics
 exporters:        xcode_mac, vs2017, linux_make, xcode_iphone, androidstudio

 type:             Component
 mainClass:        MorphexPanel

 useLocalCopy:     1

 END_JUCE_PIP_METADATA

*******************************************************************************/


#pragma once

#include "../InterfaceDefines.h"
#include "MorphexLookAndFeel.h"
#include "MainPanel.h"
#include "TopPanel.h"
#include "BottomPanel.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MorphexPanel   : public Component
{
public:
    //==============================================================================
    MorphexPanel()
        : topPanel (),
          mainPanel (),
          bottomPanel  ()
    {
        setLookAndFeel (new MorphexLookAndFeel());
        
        addAndMakeVisible (topPanel);
        addAndMakeVisible (mainPanel);
        addAndMakeVisible (bottomPanel);

        setSize (MORPHEX_PANEL_WIDTH, MORPHEX_PANEL_HEIGHT);
    }

    void paint (Graphics& g) override
    {
        g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));   // clear the background
        
        //g.fillAll (Colour(34, 34, 34));
        
        ColourGradient cg (GUI::BackgroundColorGradientStart,
                           getWidth() / 2.0f, getHeight() / 4.0f,
                           GUI::BackgroundColorGradientEnd,
                           getWidth() * 1.0f, getHeight() * 1.0f, true);
    }

    void resized() override
    {
        FlexBox fb;
        fb.flexDirection = FlexBox::Direction::column;
        
        float topPanelHeight = getHeight() / 10.0f;
        float bottomPanelHeight = getHeight() / 6.0f;
        
        FlexItem top  (getWidth(), topPanelHeight, topPanel);
        FlexItem main  (getWidth(), getHeight() - topPanelHeight - bottomPanelHeight, mainPanel);
        FlexItem bottom (getWidth(), bottomPanelHeight, bottomPanel);
        
        /*
        FlexItem top  (getWidth(), TOP_PANEL_HEIGHT, topPanel);
        FlexItem main  (getWidth(), getHeight() - TOP_PANEL_HEIGHT - BOTTOM_PANEL_HEIGHT, mainPanel);
        FlexItem bottom (getWidth(), BOTTOM_PANEL_HEIGHT, bottomPanel);
        */
        
        fb.items.addArray ( { top, main, bottom } );
        fb.performLayout (getLocalBounds().toFloat());
        
        /*
        Grid grid;

        using Track = Grid::TrackInfo;

        grid.templateRows    = { Track (1_fr), Track (2_fr), Track (1_fr) };
        grid.templateColumns = { Track (1_fr) };
        
        GridItem topPanelGrid (topPanel);
        topPanelGrid.height = 50;

        grid.items = { topPanelGrid, GridItem (mainPanel), GridItem (bottomPanel) };

        grid.performLayout (getLocalBounds());
        */
    }

private:
    //==============================================================================
    struct RightSidePanel    : public Component
    {
        RightSidePanel (Colour c) : backgroundColour (c)
        {
            for (int i = 0; i < 9; ++i)
                addAndMakeVisible (buttons.add (new TextButton (String (i))));
        }

        void paint (Graphics& g) override
        {
            g.fillAll (backgroundColour);
        }

        void resized() override
        {
            auto bounds = getLocalBounds();
            auto buttonSize = bounds.getWidth() / 3;

            for (int i = 0; i < buttons.size(); ++i)
            {
                buttons[i]->setBounds (buttonSize * (i % 3),
                                       buttonSize * (i / 3) + bounds.getHeight() / 3,
                                       buttonSize,
                                       buttonSize);
            }
        }

        Colour backgroundColour;
        OwnedArray<TextButton> buttons;
    };

    struct LeftSidePanel    : public Component
    {
        LeftSidePanel (Colour c) : backgroundColour (c)
        {
            for (int i = 0; i < 6; ++i)
            {
                auto* slider = new Slider();
                slider->setSliderStyle (Slider::SliderStyle::Rotary);
                slider->setTextBoxStyle (Slider::NoTextBox, true, 0, 0);

                addAndMakeVisible (knobs.add (slider));
            }
        }

        void paint (Graphics& g) override
        {
            g.fillAll (backgroundColour);
        }

        void resized() override
        {
            auto bounds = getLocalBounds();
            auto knobSize = bounds.getWidth() / 3;

            for (int i = 0; i < knobs.size(); ++i)
            {
                knobs[i]->setBounds (knobSize * (i % 3),
                                     bounds.getHeight() / 2 * (i / 3),
                                     knobSize,
                                     knobSize);
            }
        }

        Colour backgroundColour;
        OwnedArray<Slider> knobs;
    };
    
    /*
    struct MainPanel    : public Component
    {
        MainPanel()
        {
            for (int i = 0; i < 5; ++i)
            {
                addAndMakeVisible (sliders.add (new Slider()));
                sliders.getLast()->setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
            }
        }

        void paint (Graphics& g) override
        {
            g.fillAll (Colours::hotpink);
        }

        void resized() override
        {
            auto bounds = getLocalBounds();

            auto sliderHeight = bounds.getHeight() / sliders.size();

            for (auto* slider : sliders)
            {
                slider->setSliderStyle (Slider::SliderStyle::LinearHorizontal);
                slider->setBounds (bounds.removeFromTop (sliderHeight));
            }
        }

        OwnedArray<Slider> sliders;
    };
    */

    //==============================================================================
    //RightSidePanel rightPanel;
    //LeftSidePanel leftPanel;
    TopPanel topPanel;
    MainPanel mainPanel;
    BottomPanel bottomPanel;
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MorphexPanel)
};
