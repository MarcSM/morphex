/*
  ==============================================================================

    Instrument.cpp
    Created: 22 Apr 2020 11:40:34am
    Author:  Marc Sanchez Martinez

  ==============================================================================
*/

#include "Instrument.h"
#include "Tools.h"

namespace Core
{
    Instrument::Instrument()
    {
        // Notes
        this->note = std::vector<Note*>(NUM_MIDI_NOTES);
//        this->note.resize( NUM_MIDI_NOTES, nullptr );
        for (int i = 0; i < this->note.size(); i++)
        {
            this->note[i] = new Note(i);
        }
        
        // Mode
        if (this->mode == Mode::FullRange) this->interpolation_mode = Interpolation::None;
//        if (this->mode == Mode::FullRange) this->interpolation_mode = Interpolation::FrequencyBased;

//        // Voices
//        this->voice.reserve( i_max_voices );
//        for (int i = 0; i < i_max_voices; i++)
//        {
//            this->voice[i] = new Voice(i);
//        }
        
//        // TODO - Put synthesis inside voice
//        // Synthesis
//        this->synthesis = Synthesis();
        
        // TODO
        // self.ID = Generate and ID with current datetime, save them on the
        // hid and had and compare the ID on loading the had files to see if it matches

        // TODO - Take velocity into account too
        // self.note = [None] * 128
        // self.note = np.empty((NUM_MIDI_NOTES, NUM_MIDI_VELOCITIES), dtype=object)

        // Data
        this->name = "";
        this->samples_dirpath = "";
    };
    
    Instrument::~Instrument() {}
    
    std::vector<Note*> Instrument::getLoadedNotes()
    {
        // Output
        std::vector<Note*> loaded_notes;
        
        for (int i = 0; i < this->note.size(); i++)
        {
            Note* note = this->note[i];
            
            if ( note->hasAnyVelocity() )
            {
                loaded_notes.push_back( note );
            }
        }
        
        return loaded_notes;
    }
    
    MorphNotes Instrument::getCloserNotes(float f_target_note)
    {
        int l_i_target_note = round(f_target_note);
        int h_i_target_note = ceil(f_target_note);
        
        MorphNotes closer_notes{};
        
//        closer_notes[MorphLocation::Left] = this->note[59];
//        closer_notes[MorphLocation::Right] = closer_notes[MorphLocation::Left];
////        closer_notes[MorphLocation::Right] = this->note[64];
//
//        return closer_notes;
        
        std::vector<Note*> loaded_notes = getLoadedNotes();

        // Output (use "{}" to ensure "nullptr" initialization)
//        MorphNotes closer_notes{};

//        closer_notes[MorphLocation::Left] = loaded_notes[0];
//        closer_notes[MorphLocation::Right] = closer_notes[MorphLocation::Left];
//
//        return closer_notes;

        // Instrument::Mode::Morphing
        if (this->mode == Instrument::Mode::Morphing)
        {
            int i_notes_to_load = std::min( (int)loaded_notes.size(), (int)MorphLocation::NUM_MORPH_LOCATIONS );
            
            for (int i = 0; i < i_notes_to_load; i++)
            {
                closer_notes[i] = loaded_notes[i];
            }
        }
        // Instrument::Mode::FullRange
        else
        {
            // For loaded_notes sorted from min to max
            for (int i = 0; i < loaded_notes.size(); i++)
            {
                Note* note = loaded_notes[i];

                if (note->value < l_i_target_note)
                {
                    closer_notes[MorphLocation::Left] = note;
                }
                else if (h_i_target_note < note->value)
                {
                    closer_notes[MorphLocation::Right] = note;
                    break;
                }
                else
                {
                    closer_notes[MorphLocation::Left] = note;
                    closer_notes[MorphLocation::Right] = note;
                    if (h_i_target_note <= note->value) break;
                }
            }
        }

        if (closer_notes[MorphLocation::Left] == nullptr) closer_notes[MorphLocation::Left] = closer_notes[MorphLocation::Right];
        if (closer_notes[MorphLocation::Right] == nullptr) closer_notes[MorphLocation::Right] = closer_notes[MorphLocation::Left];

        return closer_notes;
    }
    
    MorphSounds Instrument::getCloserSounds(float f_target_note, float f_velocity)
    {
        MorphNotes closer_notes = getCloserNotes(f_target_note);
        
        // TODO - Select the closest velocity for range
        // NOTE: or interpolate velocities, if so, target also current
        // frame to not interpolate all everything on "analysis.output.values.hfreq"
        // left_note_and_velocity = left_note[ i_velocity ]
        // right_note_and_velocity = right_note[ i_velocity ]
        
        // Output
        MorphSounds closer_sounds;
        
//        closer_sounds[MorphLocation::Left] = &this->note[59]->velocity[1]->sound;
//        closer_sounds[MorphLocation::Right] = closer_sounds[MorphLocation::Left];
////        closer_sounds[MorphLocation::Right] = &this->note[59]->velocity[1]->sound;
//
//        return closer_sounds;
        
//        if (f_velocity != 0.0 and f_velocity != 1.0)
//        {
//            DBG("NOW GET");
//        }
        
        // Transform velocity range from 0-1 to 0-127
        float f_velocity_midi_range = jmap(f_velocity, 0.0f, 1.0f, 1.0f, 127.0f);
        
        // Getting closer velocities
        for (int i = 0; i < closer_notes.size(); i++)
        {
            std::vector<Velocity*> loaded_velocities = closer_notes[i]->getLoadedVelocities();
            
            int i_closer_velocity = 0;
            float f_shortest_velocity_distance = Note::NUM_MIDI_VELOCITIES;

            for (int j = 0; j < loaded_velocities.size(); j++)
            {
                float f_velocity_distance = std::abs( loaded_velocities[j]->value - f_velocity_midi_range );
                
                // Priority to lower velocities ("<=" for upper velocities)
                if (f_velocity_distance < f_shortest_velocity_distance)
                {
                    i_closer_velocity = j;
                    f_shortest_velocity_distance = f_velocity_distance;
                }
            }
            
            closer_sounds[i] = &loaded_velocities[i_closer_velocity]->sound;
        }
        
//        // TODO - Remove this velocity bypass
//        int i_velocity = 0;
//        closer_sounds[MorphLocation::Left] = &closer_notes[MorphLocation::Left]->getLoadedVelocities()[i_velocity]->sound;
//        closer_sounds[MorphLocation::Right] = &closer_notes[MorphLocation::Right]->getLoadedVelocities()[i_velocity]->sound;
//
        return closer_sounds;
    }
    
    MorphSounds Instrument::getMorphSounds()
    {
        return morph_sounds;
    }
    
    void Instrument::setMorphSound(Sound* sound, MorphLocation morph_location)
    {
        morph_sounds[morph_location] = sound;
        
//        // Output
//        MorphSounds morph_sounds;
//
//        std::vector<Note*> loaded_notes = getLoadedNotes();
//
//        int i_notes_to_load = std::min( (int)loaded_notes.size(), (int)MorphLocation::NUM_MORPH_LOCATIONS );
//
//        for (int i = 0; i < i_notes_to_load; i++)
//        {
//            std::vector<Velocity*> loaded_velocities = loaded_notes[i]->getLoadedVelocities();
//
//            morph_sounds[i] = &loaded_velocities[0]->sound;
//        }
//
//        return morph_sounds;
    }
    
//    Sound* Instrument::getSound(float f_note, int i_velocity)
//    {
//        MorphSounds morph_sounds = getCloserSounds( f_note, i_velocity );
//
//        if (&morph_sounds[MorphLocation::Left] == &morph_sounds[MorphLocation::Left])
//        {
//            return morph_sounds[MorphLocation::Left];
//        }
//
//        Sound* morph_sound = morphSounds(f_note, morph_sounds);
//
//        return morph_sound;
//    }
    
    Sound::Frame Instrument::getSoundFrame(float f_note, float f_velocity, int i_current_frame, int i_frame_length, float f_freqs_interp_factor, float f_mags_interp_factor)
    {
        MorphSounds morph_sounds = getCloserSounds( f_note, f_velocity );
        
        if (morph_sounds[MorphLocation::Left] == morph_sounds[MorphLocation::Right])
        {
            return morph_sounds[MorphLocation::Left]->getFrame(i_current_frame, i_frame_length);
        }
        else
        {
            return morphSoundFrames(f_note, morph_sounds, i_current_frame, i_frame_length, f_freqs_interp_factor, f_mags_interp_factor);
        }
    }
    
    // NOTE: About looping, two modes, the first one is that each note loops
    // on their own loop points and then do the morphing, this will cause unalignment.
    // The second one is two have the maximum starting loop point and the minimum ending
    // loop point, the loop section will be smaller but it will sound homogeneous
    // among iterations.
    Sound::Frame Instrument::morphSoundFrames(float f_target_note, MorphSounds morph_sounds, int i_current_frame, int i_frame_length, float f_freqs_interp_factor, float f_mags_interp_factor)
    {
//        if (i_current_frame >= 88)
//        {
//            DBG("Bug is coming");
//        }
        
        // Output
        Sound::Frame morphed_sound_frame;

//        // Get the maximum number of hamronics and sound length
//        int i_max_harmonics = std::max( morph_sounds[MorphLocation::Left]->max_harmonics, morph_sounds[MorphLocation::Right]->max_harmonics );
//
        // Get target frequency
        float f_target_frequency = Tools::Midi::toFreq(f_target_note);
//        float f_target_frequency = MidiMessage::getMidiNoteInHertz(int i_note);
        
        // Interpolation factor is calculated taking into account
        // how far is each note from the target frequency (normalized)
        if (this->interpolation_mode == Interpolation::FrequencyBased)
        {
//            if (f_freqs_interp_factor == -1)
            if (morph_sounds[MorphLocation::Left]->note == morph_sounds[MorphLocation::Right]->note)
            {
                f_freqs_interp_factor = 0.0;
            }
            else
            {
                f_freqs_interp_factor =
                ( f_target_frequency - Tools::Midi::toFreq(morph_sounds[MorphLocation::Left]->note) ) /
                ( Tools::Midi::toFreq(morph_sounds[MorphLocation::Right]->note) - Tools::Midi::toFreq(morph_sounds[MorphLocation::Left]->note) );
            }
            
            f_mags_interp_factor = f_freqs_interp_factor;
        }
        
        float f_stocs_interp_factor = f_freqs_interp_factor;
        float f_residual_interp_factor = f_freqs_interp_factor;
        
        MorphSoundFrames morph_sound_frames;
        
        // Select current frame
        morph_sound_frames[MorphLocation::Left] = morph_sounds[MorphLocation::Left]->getFrame(i_current_frame, i_frame_length);
        morph_sound_frames[MorphLocation::Right] = morph_sounds[MorphLocation::Right]->getFrame(i_current_frame, i_frame_length);
        
        // Get the maximum number of hamronics and sound length
        int i_max_harmonics = std::max(morph_sound_frames[MorphLocation::Left].getMaxHarmonics(),
                                       morph_sound_frames[MorphLocation::Right].getMaxHarmonics());
        
        // NOTE - If it doesnt sound properly, try to always give more weight
        // to the higer samples (it is necessary to define a function that,
        // given two frequencies, calculate the weight that is given to each
        // one so that it contributes to the morphed sample).
        // Saparate frecuency and magnitude interpolation factors?
        
        // A list with the indexes of the harmonics we want to interpolate
        std::vector<int> idx_harmonics = Tools::Generate::range(0, i_max_harmonics);
        
        //         // Live funcamental transposition
        //         def transposeFreqs(note_hfreq, f_note_freq, f_target_frequency):
        // //             currrent_fundamental = instrument.available_notes[24][100].analysis.output.values.hfreq[0][0]
        // //             explicit_fundamental = i_note
        //             return (note_hfreq / f_note_freq) * f_target_frequency
        
        // Transpose left note frequencies to the target frequency
        Tools::Calculate::divideByScalar(morph_sound_frames[MorphLocation::Left].harmonics_freqs,
                                         Tools::Midi::toFreq(morph_sounds[MorphLocation::Left]->note));
        Tools::Calculate::multiplyByScalar(morph_sound_frames[MorphLocation::Left].harmonics_freqs, f_target_frequency);
        
        // Transpose right note frequencies to the target frequency
        Tools::Calculate::divideByScalar(morph_sound_frames[MorphLocation::Right].harmonics_freqs,
                                         Tools::Midi::toFreq(morph_sounds[MorphLocation::Right]->note));
        Tools::Calculate::multiplyByScalar(morph_sound_frames[MorphLocation::Right].harmonics_freqs, f_target_frequency);
        
        if (morph_sound_frames[MorphLocation::Left].hasHarmonics() or
            morph_sound_frames[MorphLocation::Right].hasHarmonics())
        {
            // Interpolating the frequencies of the given harmonics
            morphed_sound_frame.harmonics_freqs =
            interpolateFrames(FrameType::Frequencies,
                              f_freqs_interp_factor,
                              morph_sound_frames[MorphLocation::Left].harmonics_freqs,
                              morph_sound_frames[MorphLocation::Right].harmonics_freqs,
                              i_max_harmonics,
                              idx_harmonics);
            
            // Interpolating the magnitudes of the given harmonics
            morphed_sound_frame.harmonics_mags =
            interpolateFrames(FrameType::Magnitudes,
                              f_mags_interp_factor,
                              morph_sound_frames[MorphLocation::Left].harmonics_mags,
                              morph_sound_frames[MorphLocation::Right].harmonics_mags,
                              i_max_harmonics,
                              idx_harmonics);
        }
        
        // TODO - Add sinusoidal component
        
        if (morph_sound_frames[MorphLocation::Left].hasStochastic() or
            morph_sound_frames[MorphLocation::Right].hasStochastic())
        {
            // Interpolating the stochastic components of the given harmonics
            morphed_sound_frame.stochastic =
            interpolateFrames(FrameType::Stochastic,
                              f_stocs_interp_factor,
                              morph_sound_frames[MorphLocation::Left].stochastic,
                              morph_sound_frames[MorphLocation::Right].stochastic,
                              i_max_harmonics);
        }
        
        if (morph_sound_frames[MorphLocation::Left].hasResidual() or
            morph_sound_frames[MorphLocation::Right].hasResidual())
        {
            // Interpolating the stochastic components of the given harmonics
            morphed_sound_frame.residual =
            interpolateFrames(FrameType::Residual,
                              f_residual_interp_factor,
                              morph_sound_frames[MorphLocation::Left].residual,
                              morph_sound_frames[MorphLocation::Right].residual,
                              i_frame_length);
        }
        
        return morphed_sound_frame;
    }
    
    std::vector<float> Instrument::interpolateFrames(FrameType frame_type,
                                                     float interp_factor,
                                                     std::vector<float> frame_1,
                                                     std::vector<float> frame_2,
                                                     int i_frame_length,
                                                     std::vector<int> idx_harmonics)
    {
        // TODO - Get rid of the for loops for matrix multiplications,
        // there are more eficient ways to do this
        
        // NOTE - idx_harmonics is not beign used right now
        
        // Output
        std::vector<float> interpolated_frame(i_frame_length, 0.0);
        
        int len_frame_1 = (int)frame_1.size();
        int len_frame_2 = (int)frame_2.size();

        // Magnitudes
        if (frame_type == FrameType::Magnitudes)
        {
            
//            len_frame_1 = (int)count_if(frame_1.begin(), frame_1.end(),
//                                        [](float value) { return value > DEFAULT_DB; });
//
//            len_frame_2 = (int)count_if(frame_2.begin(), frame_2.end(),
//                                        [](float value) { return value > DEFAULT_DB; });
            
            // Fill with -180.0
            std::fill(interpolated_frame.begin(), interpolated_frame.end(), DEFAULT_DB);
        }
        // Harmonics (and everything else)
        else
        {
//            len_frame_1 = (int)count_if(frame_1.begin(), frame_1.end(),
//                                        [](float value) { return value > DEFAULT_HZ; });
//
//            len_frame_2 = (int)count_if(frame_2.begin(), frame_2.end(),
//                                        [](float value) { return value > DEFAULT_HZ; });
            
            // Fill with 0.0
            std::fill(interpolated_frame.begin(), interpolated_frame.end(), DEFAULT_HZ);
        }
        
        // Aux frames
        std::vector<float> master_frame;
        std::vector<float> second_frame;

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
                interpolated_frame[i] = interp_factor * master_frame[i] + (1-interp_factor) * second_frame[i];
//                interpolated_frame[i] = (1-interp_factor) * master_frame[i] + interp_factor * second_frame[i];
            }
            else
            {
                interpolated_frame[i] = master_frame[i];
            }
        }
        
        return interpolated_frame;
    }

} // namespace Core
