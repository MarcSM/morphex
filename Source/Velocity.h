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
    bool loaded_metadata;
    
    // Initializing the harmonic analysis data structure for the .had file
    Sound* sound;
//    std::unique_ptr<Sound> sound;

    // Midi velocity value
    bool value;
    
    Velocity(int value)
    {
        // Flags
        this->loaded = false;
        this->loaded_metadata = false;
        
        // Initializing the harmonic analysis data structure for the .had file
//        this->sound = NULL;
        
        // Midi velocity value
        this->value = value;
    }
    
//    void loadSound(std::unique_ptr<Sound> sound)
    void loadSound(Sound sound)
    {
        this->sound = &sound;
        
        if (this->loaded_metadata)
        {
            this->sound->loop.start = this->loopstart;
            this->sound->loop.end = this->loopend;
            
            this->loaded =true;
        }
    }
    
    void loadSoundFromPath(std::string had_file_path)
    {
        this->sound = new Sound(had_file_path);
        this->sound->loadHadFile();
        this->loaded = true;
    }
    
private:
};
