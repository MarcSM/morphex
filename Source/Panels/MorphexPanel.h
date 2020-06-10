/*
 ==============================================================================
 
 MorphingPanel.h
 Created: 12 May 2020 14:37:23pm
 Author:  Marc Sanchez Martinez
 
 ==============================================================================
 */

#pragma once

#include "../InterfaceDefines.h"
#include "MorphexLookAndFeel.h"
#include "MainPanel.h"
#include "TopPanel.h"
#include "BottomPanel.h"

//==============================================================================
/*
*/
class MorphexPanel :    public Component,
                        public DragAndDropContainer
{
public:
    //==============================================================================
    MorphexPanel(SpectralMorphingToolAudioProcessor* inProcessor)
        : topPanel (inProcessor),
          mainPanel (inProcessor),
          bottomPanel (inProcessor)
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
        
        ColourGradient cg (GUI::Color::BackgroundGradientStart,
                           getWidth() / 2.0f, getHeight() / 4.0f,
                           GUI::Color::BackgroundGradientEnd,
                           getWidth() * 1.0f, getHeight() * 1.0f, true);
        
        g.setFillType (cg);
        g.fillRect (0, 0, getWidth(), getHeight());
    }

    void resized() override
    {
        FlexBox fb;
        fb.flexDirection = FlexBox::Direction::column;
        
        float topPanelHeight = getHeight() / 10.0f;
        float bottomPanelHeight = getHeight() / 8.0f;
        
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
    
    TopPanel topPanel;
    MainPanel mainPanel;
    BottomPanel bottomPanel;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MorphexPanel)
};
