/*
  ==============================================================================

    Instrument.cpp
    Created: 22 Apr 2020 11:40:34am
    Author:  Marc Sanchez Martinez

  ==============================================================================
*/

#include "Instrument.h"

namespace Core
{
    enum MorphSoundsLocation
    {
        // TODO - Nested enum for Left Low, Left High and NUM_LEFT_MORPH_SOUNDS
        Left = 0,
        // TODO - Nested enum for Right Low, Right High and NUM_RIGHT_MORPH_SOUNDS
        Right,
        // TODO - NUM_MORPH_SOUNDS = NUM_LEFT_MORPH_SOUNDS + NUM_RIGHT_MORPH_SOUNDS
        NUM_MORPH_SOUNDS
    };
    
    typedef std::array<Sound*, MorphSoundsLocation::NUM_MORPH_SOUNDS> MorphSounds;
//    typedef std::array<std::unique_ptr<Sound>, MorphSoundsLocation::NUM_MORPH_SOUNDS> MorphSounds;

    Instrument::Instrument()
    {
        // Notes
        this->note.reserve( NUM_MIDI_NOTES );
        for (int i = 0; i < NUM_MIDI_NOTES; i++)
        {
            this->note[i] = new Note(i);
        }
        
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
    
    // TODO - Split getSound in two and add getSoundFrame
    Sound* getSound(float f_note, int i_velocity)
    {
        MorphSounds morph_sounds = getCloserSounds( f_note );
        
        if (morph_sounds[MorphSoundsLocation::Left] == morph_sounds[MorphSoundsLocation::Left])
        {
            Sound sound = *morph_sounds[MorphSoundsLocation::Left]; // Copy constructor is called here
            Sound::Frame sound_frame = sound.getFrame(i_current_frame, i_frame_length)
            sound.model.values.harmonics_freqs = sound_frame.harmonics_freqs
            sound.model.values.harmonics_mags = sound_frame.harmonics_mags
            sound.model.values.residual = sound_frame.residual
            sound.model.values.stochastic = sound_frame.stochastic
            return sound;
        }
        
        Sound morph_sound = morphSounds(f_note, morph_sounds, i_current_frame, i_frame_length, f_interpolation_factor)
        
        return morph_sound
    }
    
    Sound::Frame* getSoundFrame(float f_note, int i_velocity, int i_current_frame, int i_frame_length)
    {
        MorphSounds morph_sounds = getCloserSounds( f_note );
        
        if (morph_sounds[MorphSoundsLocation::Left] == morph_sounds[MorphSoundsLocation::Left])
        {
            return morph_sounds[MorphSoundsLocation::Left];
        }
        else
        {
            Sound morph_sound = morphSounds(f_note, morph_sounds, i_current_frame, i_frame_length, f_interpolation_factor)
        }
    }
    
    std::vector<float> Instrument::interpolateFrames(FrameType frame_type,
                                                     float interp_factor,
                                                     std::vector<float> frame_1,
                                                     std::vector<float> frame_2,
                                                     int max_harmonics,
                                                     std::vector<int> idx_harmonics) const override
    {
        // Output
        std::vector<float> interpolated_frame(max_harmonics, 0.0);
        
        int len_frame_1 = 0;
        int len_frame_2 = 0;

        // Magnitudes
        if (frame_type == FrameType.Magnitudes)
        {
            len_frame_1 = count_if(frame_1.begin(), frame_1.end(),
                                   [](float value) { return value > DEFAULT_DB; });
            
            len_frame_2 = count_if(frame_2.begin(), frame_2.end(),
                                   [](float value) { return value > DEFAULT_DB; });
            
            // Fill with -180.0
            std::fill(interpolated_frame.begin(), interpolated_frame.end(), DEFAULT_DB);
        }
        // Harmonics (and everything else)
        else
        {
            len_frame_1 = count_if(frame_1.begin(), frame_1.end(),
                                   [](float value) { return value > DEFAULT_HZ; });
            
            len_frame_2 = count_if(frame_2.begin(), frame_2.end(),
                                   [](float value) { return value > DEFAULT_HZ; });
            
            len_frame_1 = np.sum(frame_1 > DEFAULT_HZ)
            len_frame_2 = np.sum(frame_2 > DEFAULT_HZ)
            
            // Fill with 0.0
            std::fill(interpolated_frame.begin(),
                      interpolated_frame.end(),
                      DEFAULT_HZ);
        }
        
        if (len_frame_1 > len_frame_2)
        {
            master_frame = frame_1;
            second_frame = frame_2;
        }
        else
        {
            master_frame = frame_2;
            second_frame = frame_1;
        }
        
        // The master frame is always longer or equal than the second frame
        for (int i = 0; i < master_frame.size(); i++)
        {
            if (i < second_frame.size())
            {
                interpolated_frame[i] = (1-interp_factor) * master_frame[i] + interp_factor * second_frame[i];
            }
            else
            {
                interpolated_frame[i] = master_frame[i];
            }
        }
        
        return interpolated_frame
    }

} // namespace Core
