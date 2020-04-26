/*
  ==============================================================================

    Note.h
    Created: 22 Apr 2020 12:08:29pm
    Author:  Marc Sanchez Martinez

  ==============================================================================
*/

#pragma once

#include "Velocity.h"

namespace Core { class Note; }
    
class Core::Note
{
public:
    
    const static int NUM_MIDI_VELOCITIES = 128;
    
    // Midi velocity value
    int value;

    // Velocities
    std::vector<Velocity*> velocity;
    
    Note(int value)
    {
        // Midi velocity value
        this->value = value;
        
        // Velocities
        this->velocity.reserve( NUM_MIDI_VELOCITIES );
        for (int i = 0; i < NUM_MIDI_VELOCITIES; i++)
        {
            this->velocity[i] = new Velocity(i);
        }
    }
    
    std::vector<Velocity*> getLoadedVelocities()
    {
        std::vector<Velocity*> loaded_velocities;
        
        for (Velocity* & velocity : this->velocity)
        {
            loaded_velocities.push_back( velocity );
        }
        
        return loaded_velocities;
    }
    
    bool hasAnyVelocity()
    {
        return this->getLoadedVelocities().size() > 0;
    }
    
private:
};
