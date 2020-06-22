/*
  ==============================================================================

    Velocity.h
    Created: 22 Apr 2020 12:08:37pm
    Author:  Marc Sanchez Martinez

  ==============================================================================
*/

#pragma once

#include "Sound.h"

namespace Core { class Velocity; }

class Core::Velocity
{
public:
    
    // Flags
    bool loaded;
//    bool loaded_metadata;
    
    Sound sound;

    // Midi values
    int value;
    int midi_note_value;
    
    void init()
    {
        // Flags
        this->loaded = false;
//        this->loaded_metadata = false;
    }
    
    void reset()
    {
        // Initialize
        this->init();
        
        // Reset the sound
        this->sound.reset();
    }
    
    Velocity (int midi_note_value, int value)
    {
        // Midi values
        this->value = value;
        this->midi_note_value = midi_note_value;
        
        // Initialize
        this->init();
    }
    
    void setSound (Sound sound)
    {
        // TODO - Find a better approach to do this (try to avoid using copy constructor)
        this->sound = sound;
        this->loaded = true;
    }
    
    void loadSound (std::string file_path)
    {
        // Maybe turn "this->sound" into a pointer and create a new
        // method to load the sound in the proper note and velocity
        this->sound = *new Sound(file_path, midi_note_value, value);
        this->loaded = true;
    }
    
private:
};
