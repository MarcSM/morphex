/*
  ==============================================================================

    Instrument.cpp
    Created: 22 Apr 2020 11:40:34am
    Author:  Marc Sanchez Martinez

  ==============================================================================
*/

#include "Instrument.h"

Instrument::Instrument()
{
    // Notes
    this->note = np.ndarray(NUM_MIDI_NOTES, dtype=object)
    for i in range(NUM_MIDI_NOTES): self.note[i] = Note(i)

    // TODO
    // self.ID = Generate and ID with current datetime, save them on the
    // hid and had and compare the ID on loading the had files to see if it matches

    // TODO - Take velocity into account too
    // self.note = [None] * 128
    // self.note = np.empty((NUM_MIDI_NOTES, NUM_MIDI_VELOCITIES), dtype=object)

    // Data
    this->name = "";
    this->samples_dirpath = "";

    // Note playback
    this->playing_note = false;
    this->loop_mode = true;
    this->adsr = StateADSR::Attack;

    // Synthesis
    this->synthesis = Synthesis();
};
