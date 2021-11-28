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

#include <JuceHeader.h>

class BottomPanel : public juce::Component
{
public:
    BottomPanel (MorphexAudioProcessor& processor) :
        m_midiKeyboard (processor.getMidiState(), juce::MidiKeyboardComponent::Orientation::horizontalKeyboard)
    {
        // Set available note range
        int i_octaves  = 6;
        int i_num_keys = 12 * i_octaves;
        m_numWhiteKeys = 7 * i_octaves;

        int lowestNote  = 24;
        int highestNote = lowestNote + i_num_keys;

        m_midiKeyboard.setAvailableRange (lowestNote, highestNote);

        // Disable scrolling
        m_midiKeyboard.setScrollButtonsVisible (false);

        addAndMakeVisible (m_leftPanel);
        addAndMakeVisible (m_rightPanel);
        addAndMakeVisible (m_midiKeyboard);
    }

    ~BottomPanel() {}

    void paint (juce::Graphics& g) override
    {
        // Draw borders
        GUI::Paint::drawBorders (g, m_midiKeyboard.getLocalBounds());
    }

    void resized() override
    {
        juce::FlexBox fb;

        float centerPanelWidth = getWidth() * 0.75f;
        float sidePanelWidth   = (getWidth() - centerPanelWidth) / 2.0f;

        m_midiKeyboard.setKeyWidth (centerPanelWidth / (float) m_numWhiteKeys);

        juce::FlexItem left (sidePanelWidth, getHeight(), m_leftPanel);
        juce::FlexItem center (centerPanelWidth, getHeight(), m_midiKeyboard);
        juce::FlexItem right (sidePanelWidth, getHeight(), m_rightPanel);

        fb.items.addArray ({ left, center, right });
        fb.performLayout (getLocalBounds().toFloat());
    }

private:
    struct LeftSidePanel : public Component
    {
        LeftSidePanel() {}

        void paint (juce::Graphics& g) override
        {
            // Draw borders
            GUI::Paint::drawBorders (g, getLocalBounds());
        }

        void resized() override {}
    };

    struct RightSidePanel : public Component
    {
        RightSidePanel() {}

        void paint (juce::Graphics& g) override
        {
            // Draw borders
            GUI::Paint::drawBorders (g, getLocalBounds());
        }

        void resized() override {}
    };

    juce::MidiKeyboardComponent m_midiKeyboard;

    LeftSidePanel  m_leftPanel;
    RightSidePanel m_rightPanel;

    int m_numWhiteKeys;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BottomPanel)
};
