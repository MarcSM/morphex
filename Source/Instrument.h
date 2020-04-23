/*
  ==============================================================================

    Instrument.h
    Created: 22 Apr 2020 11:40:34am
    Author:  Marc Sanchez Martinez

  ==============================================================================
*/

#pragma once

//#include "Tools.h"
#include "Note.h"
#include "Synthesis.h"

#include <vector>

int NUM_MIDI_NOTES = 128;

enum class FrameInterpolationMode
{
    Frequencies = 0,    // Interpolate Frequencies
    Magnitudes,         // Interpolate Magnitudes
    Residual,           // Interpolate Residual components
    Stochastic          // Interpolate Stochastic components
};

enum class StateADSR
{
    Attack = 0,
    Decay,
    Sustain,
    Release
};

class Instrument
{
public:
    
    // Notes
    std::vector<Note> note;
    
    // Data
    std::string name;
    std::string samples_dirpath;
        
    // Note playback
    bool playing_note;
    bool loop_mode;
    bool adsr;
        
    // Synthesis
    Synthesis synthesis;
    
    // Initializers
    Instrument();
    
private:
};
