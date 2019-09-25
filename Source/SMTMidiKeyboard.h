/*
  ==============================================================================

    SMTMidiKeyboard.h
    Created: 20 Jul 2019 4:53:17pm
    Author:  Marc Sanchez Martinez

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"

#include "PluginProcessor.h"

class SMTMidiKeyboard
:   public MidiKeyboardComponent
{
public:

    SMTMidiKeyboard(MidiKeyboardState& midiState);
    ~SMTMidiKeyboard();

    void resized() override;
    
    void paint(Graphics& g) override;
    
private:

    SpectralMorphingToolAudioProcessor* mProcessor;
};
