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

#include "sound.h"

namespace Core
{
class Velocity
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
        this->value           = value;
        this->midi_note_value = midi_note_value;

        // Initialize
        this->init();
    }

    void setSound (Sound sound)
    {
        // TODO - Find a better approach to do this (try to avoid using copy constructor)
        this->sound  = sound;
        this->loaded = true;
    }

    void loadSound (std::string file_path)
    {
        // Maybe turn "this->sound" into a pointer and create a new
        // method to load the sound in the proper note and velocity
        this->sound  = *new Sound (file_path, midi_note_value, value);
        this->loaded = true;
    }

private:
};
}; // namespace Core
