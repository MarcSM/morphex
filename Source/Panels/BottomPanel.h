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
        
        addAndMakeVisible(mMidiKeyboard);
    }

    ~BottomPanel()
    {
    }

    void paint (Graphics& g) override
    {
        /* This demo code just fills the component's background and
           draws some placeholder text to get you started.

           You should replace everything in this method with your own
           drawing code..
        */

//        g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));   // clear the background
        
        /*
        g.setColour (Colours::grey);
        g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

        g.setColour (Colours::white);
        g.setFont (14.0f);
        g.drawText ("BottomPanel", getLocalBounds(),
                    Justification::centred, true);   // draw some placeholder text
         */
    }

    void resized() override
    {
        // This method is where you should set the bounds of any child
        // components that your component contains..
        
        int side_margin = getWidth() / 8.0f;
        int midi_keyboard_x = 0 + side_margin;
        int midi_keyboard_y = 0;
        int midi_keyboard_width = getWidth() - (side_margin * 2);
        int midi_keyboard_height = getHeight();
        
        mMidiKeyboard.setBounds(midi_keyboard_x, midi_keyboard_y,
                                midi_keyboard_width, midi_keyboard_height);
        
        mMidiKeyboard.setKeyWidth( midi_keyboard_width / float(i_num_white_keys) );
        
        mMidiKeyboard.setSize(midi_keyboard_width, midi_keyboard_height);
        
//        int w = (int)mMidiKeyboard.getKeyWidth() * (7 * 7 + 3), h = 50;
//        mMidiKeyboard.setSize(w, h);
        //mMidiKeyboard.setCentrePosition(getWidth() / 2, getHeight() / 2);
    }

private:
    
    MidiKeyboardComponent mMidiKeyboard;
    int i_num_white_keys;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BottomPanel)
};
