/*
  ==============================================================================

    BottomPanel.h
    Created: 12 May 2020 11:19:10am
    Author:  Marc Sanchez Martinez

  ==============================================================================
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
