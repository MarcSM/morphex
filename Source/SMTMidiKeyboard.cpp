/*
  ==============================================================================

    SMTMidiKeyboard.cpp
    Created: 20 Jul 2019 4:53:17pm
    Author:  Marc Sanchez Martinez

  ==============================================================================
*/

#include "SMTMidiKeyboard.h"

SMTMidiKeyboard::SMTMidiKeyboard(MidiKeyboardState& midiState):
    MidiKeyboardComponent(midiState, MidiKeyboardComponent::horizontalKeyboard)
{
    // Set available note range
    int lowestNote = 24;
    int highestNote = 119;

    this->setAvailableRange (lowestNote, highestNote);
    
    // Disable scrolling
    this->setScrollButtonsVisible(false);
    
    repaint();
}

SMTMidiKeyboard::~SMTMidiKeyboard()
{
    
}

void SMTMidiKeyboard::paint (Graphics& g)
{
    MidiKeyboardComponent::paint(g);
    
    g.setColour(Colours::black);
    g.drawRoundedRectangle(0, 0, getWidth(), getHeight(), 3, 3);
}

void SMTMidiKeyboard::resized()
{
    
}
