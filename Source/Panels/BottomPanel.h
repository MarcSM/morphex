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

class BottomPanel : public Component
{
public:
    BottomPanel (SpectralMorphingToolAudioProcessor* inProcessor)
    :   mMidiKeyboard (inProcessor->getMidiState(), MidiKeyboardComponent::horizontalKeyboard)
    {
        // Set available note range
        int i_octaves = 6;
        int i_num_keys = 12 * i_octaves;
        i_num_white_keys = 7 * i_octaves;
        
        int lowestNote = 24;
        int highestNote = lowestNote + i_num_keys;
        
        mMidiKeyboard.setAvailableRange (lowestNote, highestNote);
        
        // Disable scrolling
        mMidiKeyboard.setScrollButtonsVisible (false);
        
        addAndMakeVisible (leftPanel);
        addAndMakeVisible (rightPanel);
        addAndMakeVisible (mMidiKeyboard);
    }

    ~BottomPanel() {}

    void paint (Graphics& g) override
    {
        // Draw borders
        GUI::Paint::drawBorders (g, mMidiKeyboard.getLocalBounds());
    }

    void resized() override
    {
        FlexBox fb;
        
        float centerPanelWidth = getWidth() * 0.75f;
        float sidePanelWidth = (getWidth() - centerPanelWidth) / 2.0f;
        
        mMidiKeyboard.setKeyWidth (centerPanelWidth / (float) i_num_white_keys);
        
        FlexItem left (sidePanelWidth, getHeight(), leftPanel);
        FlexItem center (centerPanelWidth, getHeight(), mMidiKeyboard);
        FlexItem right (sidePanelWidth, getHeight(), rightPanel);
        
        fb.items.addArray ({left, center, right});
        fb.performLayout (getLocalBounds().toFloat());
    }

private:
    
    struct LeftSidePanel : public Component
    {
        LeftSidePanel () {}
        
        void paint (Graphics& g) override
        {
            // Draw borders
            GUI::Paint::drawBorders(g, getLocalBounds());
        }
        
        void resized() override {}
    };
    
    struct RightSidePanel : public Component
    {
        RightSidePanel () {}
        
        void paint (Graphics& g) override
        {
            // Draw borders
            GUI::Paint::drawBorders(g, getLocalBounds());
        }
        
        void resized() override {}
    };
    
    MidiKeyboardComponent mMidiKeyboard;
    
    LeftSidePanel leftPanel;
    RightSidePanel rightPanel;
    
    int i_num_white_keys;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BottomPanel)
};
