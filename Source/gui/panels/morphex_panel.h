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

#include "bottom_panel.h"
#include "main_panel.h"
#include "top_panel.h"

#include "../../helpers/interface_defines.h"

class MorphexPanel : public juce::Component,
public juce::DragAndDropContainer
{
public:
    MorphexPanel (MorphexAudioProcessor& processor) :
        m_topPanel (processor),
        m_mainPanel (processor),
        m_bottomPanel (processor)
    {
        addAndMakeVisible (m_topPanel);
        addAndMakeVisible (m_mainPanel);
        addAndMakeVisible (m_bottomPanel);

        setSize (MORPHEX_PANEL_WIDTH, MORPHEX_PANEL_HEIGHT);
    }

    void paint (juce::Graphics& g) override
    {
        g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId)); // clear the background

        juce::ColourGradient cg (GUI::Color::BackgroundGradientStart,
                           getWidth() / 2.0f,
                           getHeight() / 4.0f,
                           GUI::Color::BackgroundGradientEnd,
                           getWidth() * 1.0f,
                           getHeight() * 1.0f,
                           true);

        g.setFillType (cg);
        g.fillRect (0, 0, getWidth(), getHeight());
    }

    void resized() override
    {
        juce::FlexBox fb;
        fb.flexDirection = juce::FlexBox::Direction::column;

        float m_topPanelHeight    = getHeight() / 10.0f;
        float m_bottomPanelHeight = getHeight() / 8.0f;

        juce::FlexItem top (getWidth(), m_topPanelHeight, m_topPanel);
        juce::FlexItem main (getWidth(), getHeight() - m_topPanelHeight - m_bottomPanelHeight, m_mainPanel);
        juce::FlexItem bottom (getWidth(), m_bottomPanelHeight, m_bottomPanel);

        fb.items.addArray ({ top, main, bottom });
        fb.performLayout (getLocalBounds().toFloat());
    }

private:
    struct RightSidePanel : public juce::Component
    {
        RightSidePanel (juce::Colour c) :
            m_backgroundColour (c)
        {
            for (int i = 0; i < 9; ++i)
            {
                addAndMakeVisible (m_buttons.add (new juce::TextButton (juce::String (i))));
            }
        }

        void paint (juce::Graphics& g) override
        {
            g.fillAll (m_backgroundColour);
        }

        void resized() override
        {
            auto bounds     = getLocalBounds();
            auto buttonSize = bounds.getWidth() / 3;

            for (int i = 0; i < m_buttons.size(); ++i)
            {
                m_buttons[i]->setBounds (buttonSize * (i % 3),
                                         buttonSize * (i / 3) + bounds.getHeight() / 3,
                                         buttonSize,
                                         buttonSize);
            }
        }

        juce::Colour                 m_backgroundColour;
        juce::OwnedArray<juce::TextButton> m_buttons;
    };

    struct LeftSidePanel : public juce::Component
    {
        LeftSidePanel (juce::Colour c) :
            m_backgroundColour (c)
        {
            for (int i = 0; i < 6; ++i)
            {
                auto* slider = new juce::Slider();
                slider->setSliderStyle (juce::Slider::SliderStyle::Rotary);
                slider->setTextBoxStyle (juce::Slider::NoTextBox, true, 0, 0);

                addAndMakeVisible (m_knobs.add (slider));
            }
        }

        void paint (juce::Graphics& g) override
        {
            g.fillAll (m_backgroundColour);
        }

        void resized() override
        {
            auto bounds   = getLocalBounds();
            auto knobSize = bounds.getWidth() / 3;

            for (int i = 0; i < m_knobs.size(); ++i)
            {
                m_knobs[i]->setBounds (knobSize * (i % 3),
                                       bounds.getHeight() / 2 * (i / 3),
                                       knobSize,
                                       knobSize);
            }
        }

        juce::Colour             m_backgroundColour;
        juce::OwnedArray<juce::Slider> m_knobs;
    };

    TopPanel    m_topPanel;
    MainPanel   m_mainPanel;
    BottomPanel m_bottomPanel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MorphexPanel)
};
