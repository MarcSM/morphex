/* Copyright (C) 2020 Marc Sanchez Martinez
 *
 * https://github.com/MarcSM/morphex
 *
 * Morphex is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Morphex is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Morphex. If not, see <http://www.gnu.org/licenses/>.
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
