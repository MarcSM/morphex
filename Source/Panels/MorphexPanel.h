/* Copyright (C) 2020 Marc Sanchez Martinez
 *
 * https://github.com/MarcSM/morphex
 *
 * Morphex is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Morphex is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Morphex. If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include "MainPanel.h"
#include "TopPanel.h"
#include "BottomPanel.h"

#include "../Helpers/InterfaceDefines.h"

class MorphexPanel : public Component,
                     public DragAndDropContainer
{
public:
    
    MorphexPanel (SpectralMorphingToolAudioProcessor* inProcessor)
        : topPanel (inProcessor),
          mainPanel (inProcessor),
          bottomPanel (inProcessor)
    {
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

        FlexItem top (getWidth(), topPanelHeight, topPanel);
        FlexItem main (getWidth(), getHeight() - topPanelHeight - bottomPanelHeight, mainPanel);
        FlexItem bottom (getWidth(), bottomPanelHeight, bottomPanel);

        fb.items.addArray ({top, main, bottom});
        fb.performLayout (getLocalBounds().toFloat());
    }

private:
    
    struct RightSidePanel : public Component
    {
        RightSidePanel (Colour c) : backgroundColour (c)
        {
            for (int i = 0; i < 9; ++i)
            {
                addAndMakeVisible (buttons.add (new TextButton (String (i))));
            }
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

    struct LeftSidePanel : public Component
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
    
    TopPanel topPanel;
    MainPanel mainPanel;
    BottomPanel bottomPanel;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MorphexPanel)
};
