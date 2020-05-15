/*
  ==============================================================================

    BottomPanel.h
    Created: 12 May 2020 11:19:10am
    Author:  Marc Sanchez Martinez

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class BottomPanel    : public Component
{
public:
    BottomPanel(SpectralMorphingToolAudioProcessor* inProcessor)
    :   mMidiKeyboard(inProcessor->getMidiState(), MidiKeyboardComponent::horizontalKeyboard)
    {
        // In your constructor, you should add any child components, and
        // initialise any special settings that your component needs.
        
        // Set available note range
        int i_octaves = 6;
        int i_num_keys = 12 * i_octaves;
        i_num_white_keys = 7 * i_octaves;
        
        int lowestNote = 24;
        int highestNote = lowestNote + i_num_keys;
        
        mMidiKeyboard.setAvailableRange (lowestNote, highestNote);
        
        // Disable scrolling
        mMidiKeyboard.setScrollButtonsVisible(false);
        
        addAndMakeVisible (leftPanel);
        addAndMakeVisible (rightPanel);
        addAndMakeVisible (mMidiKeyboard);
    }

    ~BottomPanel()
    {
    }

    void paint (Graphics& g) override
    {
        // Draw borders
        GUI::Paint::drawBorders(g, mMidiKeyboard.getLocalBounds());
    }

    void resized() override
    {
        FlexBox fb;
        
        float centerPanelWidth = getWidth() * 0.75f;
        float sidePanelWidth = ( getWidth() - centerPanelWidth ) / 2.0f;
        
        mMidiKeyboard.setKeyWidth( centerPanelWidth / float(i_num_white_keys) );
        
        FlexItem left  (sidePanelWidth, getHeight(), leftPanel);
        FlexItem center  (centerPanelWidth, getHeight(), mMidiKeyboard);
        FlexItem right (sidePanelWidth, getHeight(), rightPanel);
        
        fb.items.addArray ( { left, center, right } );
        fb.performLayout (getLocalBounds().toFloat());
        
//        FlexBox fb;
//
//        float side_margin = getWidth() / 8.0f;
//        float midi_keyboard_x = 0.0f + side_margin;
//        float midi_keyboard_y = 0.0f;
//        float midi_keyboard_width = getWidth() - (side_margin * 2.0f);
//        float midi_keyboard_height = getHeight();
//
////        mMidiKeyboard.setBounds(midi_keyboard_x, midi_keyboard_y,
////                                midi_keyboard_width, midi_keyboard_height);
////
//        mMidiKeyboard.setKeyWidth( midi_keyboard_width / float(i_num_white_keys) );
//
//        // mMidiKeyboard.setSize(midi_keyboard_width, midi_keyboard_height);
//
////        // Left side bounds
////        left_side_bounds.setBounds(0, 0, side_margin, getHeight());
////
////        // Right side bounds
////        right_side_bounds.setBounds(midi_keyboard_x + midi_keyboard_width, 0,
////                                    side_margin, getHeight());
//
//        FlexItem left (side_margin, getHeight(), leftPanel);
//        FlexItem center (midi_keyboard_width, getHeight(), mMidiKeyboard);
//        FlexItem right (side_margin, getHeight(), rightPanel);
//
//        fb.items.addArray ( { left, center, right } );
//        fb.performLayout (getLocalBounds().toFloat());
        
//        int w = (int)mMidiKeyboard.getKeyWidth() * (7 * 7 + 3), h = 50;
//        mMidiKeyboard.setSize(w, h);
        //mMidiKeyboard.setCentrePosition(getWidth() / 2, getHeight() / 2);
    }

private:
    
    struct LeftSidePanel    : public Component
    {
        LeftSidePanel ()
        {
            
        }
        
        void paint (Graphics& g) override
        {
            // Draw borders
            GUI::Paint::drawBorders(g, getLocalBounds());
        }
        
        void resized() override
        {
            
        }
    };
    
    struct RightSidePanel    : public Component
    {
        RightSidePanel ()
        {
            
        }
        
        void paint (Graphics& g) override
        {
            // Draw borders
            GUI::Paint::drawBorders(g, getLocalBounds());
        }
        
        void resized() override
        {
            
        }
    };
    
    MidiKeyboardComponent mMidiKeyboard;
    LeftSidePanel leftPanel;
    RightSidePanel rightPanel;
    int i_num_white_keys;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BottomPanel)
};
