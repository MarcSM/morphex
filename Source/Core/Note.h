/*
  ==============================================================================

    Note.h
    Created: 22 Apr 2020 12:08:29pm
    Author:  Marc Sanchez Martinez

  ==============================================================================
*/

#pragma once

#include "Velocity.h"

namespace Core
{
    class Note;
    
    const static int NUM_MIDI_VELOCITIES = 128;
    const static int MAX_MIDI_VELOCITY = NUM_MIDI_VELOCITIES - 1;
}
    
class Core::Note
{
public:

    // Midi velocity value
    int value;

    // Velocities
    std::vector<Velocity*> velocity;
    
    Note(int value)
    {
        // Midi velocity value
        this->value = value;
        
        // Velocities
        this->velocity = std::vector<Velocity*> (NUM_MIDI_VELOCITIES);
        for (int i = 0; i < this->velocity.size(); i++)
        {
            this->velocity[i] = new Velocity (value, i);
        }
    }
    
    void reset()
    {
        // Reset Velocities
        for (int i = 0; i < this->velocity.size(); i++)
        {
            this->velocity[i]->reset();
        }
    }
    
    std::vector<Velocity*> getLoadedVelocities()
    {
        std::vector<Velocity*> loaded_velocities;
        
        for (int i = 0; i < this->velocity.size(); i++)
        {
            Velocity* velocity = this->velocity[i];
            
            if (velocity->loaded)
            {
                loaded_velocities.push_back (velocity);
            }
        }
        
        return loaded_velocities;
    }
    
    bool hasAnyVelocity()
    {
        return this->getLoadedVelocities().size() > 0;
    }
    
private:
};
