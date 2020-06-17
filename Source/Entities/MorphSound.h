/*
  ==============================================================================

    MorphSound.h
    Created: 21 Jul 2019 9:19:56pm
    Author:  Marc Sanchez Martinez

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"

struct MorphSound
:   public SynthesiserSound
{
    MorphSound() {};
    
    bool appliesToNote (int /*midiNoteNumber*/) override    { return true; };
    bool appliesToChannel (int /*midiChannel*/) override    { return true; };
};
