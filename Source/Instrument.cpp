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
    
    Instrument::~Instrument() {}
    
    void Instrument::startNote()
    {
        this->playing_note = true;
        this->adsr = StateADSR::Attack;
        this->synthesis.reset();
    }
    
    void Instrument::endNote()
    {
        this->adsr = StateADSR::Release;
    }
    
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
        
        std::vector<Note*> loaded_notes = getLoadedNotes();
        
        // Output
        MorphNotes closer_notes;
        
        // For loaded_notes sorted from min to max
        //        for note in loaded_notes
        
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
            
            if (closer_notes[MorphLocation::Left] == NULL) closer_notes[MorphLocation::Left] = closer_notes[MorphLocation::Right];
            if (closer_notes[MorphLocation::Right] == NULL) closer_notes[MorphLocation::Right] = closer_notes[MorphLocation::Left];
            
            return closer_notes;
        }
    }
    
    MorphSounds Instrument::getCloserSounds(float f_target_note, int i_velocity)
    {
        MorphNotes closer_notes = getCloserNotes(f_target_note);
        
        // TODO - Select the closest velocity for range
        // NOTE: or interpolate velocities, if so, target also current
        // frame to not interpolate all everything on "analysis.output.values.hfreq"
        // left_note_and_velocity = left_note[ i_velocity ]
        // right_note_and_velocity = right_note[ i_velocity ]
        
        // Output
        MorphSounds closer_sounds;
        
        // TODO - Remove this velocity bypass
        i_velocity = 0;
        closer_sounds[MorphLocation::Left] = closer_notes[MorphLocation::Left]->getLoadedVelocities()[i_velocity]->sound;
        closer_sounds[MorphLocation::Right] = closer_notes[MorphLocation::Right]->getLoadedVelocities()[i_velocity]->sound;
        
        return closer_sounds;
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
    
    Sound::Frame Instrument::getSoundFrame(float f_note, int i_velocity, int i_current_frame, int i_frame_length, float f_interpolation_factor)
    {
        MorphSounds morph_sounds = getCloserSounds( f_note, i_velocity );
        
        if (morph_sounds[MorphLocation::Left] == morph_sounds[MorphLocation::Right])
        {
            return morph_sounds[MorphLocation::Left]->getFrame(i_current_frame, i_frame_length);
        }
        else
        {
            return morphSoundFrames(f_note, morph_sounds, i_current_frame, i_frame_length, f_interpolation_factor);
        }
    }
    
    // NOTE: About looping, two modes, the first one is that each note loops
    // on their own loop points and then do the morphing, this will cause unalignment.
    // The second one is two have the maximum starting loop point and the minimum ending
    // loop point, the loop section will be smaller but it will sound homogeneous
    // among iterations.
    Sound::Frame Instrument::morphSoundFrames(float f_target_note, MorphSounds morph_sounds, int i_current_frame, int i_frame_length, float f_interpolation_factor)
    {
        // Output
        Sound::Frame morphed_sound_frame;

        // Get the maximum number of hamronics and sound length
        int i_max_harmonics = std::max( morph_sounds[MorphLocation::Left]->max_harmonics, morph_sounds[MorphLocation::Right]->max_harmonics );
        
        // Get target frequency
        float f_target_frequency = Tools::Midi::toFreq(f_target_note);
//        float f_target_frequency = MidiMessage::getMidiNoteInHertz(int i_note);
        
        // Interpolation factor is calculated taking into account
        // how far is each note from the target frequency (normalized)
        if (f_interpolation_factor == -1)
        {
            f_interpolation_factor =
            ( f_target_frequency - Tools::Midi::toFreq(morph_sounds[MorphLocation::Left]->note) ) /
            ( Tools::Midi::toFreq(morph_sounds[MorphLocation::Right]->note) - Tools::Midi::toFreq(morph_sounds[MorphLocation::Left]->note) );
        }
        
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
        
        MorphSoundFrames morph_sound_frames;
        
        // Select current frame
        morph_sound_frames[MorphLocation::Left] = morph_sounds[MorphLocation::Left]->getFrame(i_current_frame, i_frame_length);
        morph_sound_frames[MorphLocation::Right] = morph_sounds[MorphLocation::Right]->getFrame(i_current_frame, i_frame_length);
        // Transpose left note frequencies to the target frequency
        Tools::Calculate::divideByScalar(morph_sound_frames[MorphLocation::Left].harmonics_freqs,
                                         (Tools::Midi::toFreq(morph_sounds[MorphLocation::Left]->note) * f_target_frequency) );
        
        // Transpose right note frequencies to the target frequency
        Tools::Calculate::divideByScalar(morph_sound_frames[MorphLocation::Right].harmonics_freqs,
                                         (Tools::Midi::toFreq(morph_sounds[MorphLocation::Right]->note) * f_target_frequency) );
        
        if ( morphed_sound_frame.hasHarmonics() )
        {
            // Interpolating the frequencies of the given harmonics
            morphed_sound_frame.harmonics_freqs =
            interpolateFrames(FrameType::Frequencies,
                              f_interpolation_factor,
                              morph_sound_frames[MorphLocation::Left].harmonics_freqs,
                              morph_sound_frames[MorphLocation::Right].harmonics_freqs,
                              i_max_harmonics,
                              idx_harmonics);
            
            // Interpolating the magnitudes of the given harmonics
            morphed_sound_frame.harmonics_mags =
            interpolateFrames(FrameType::Magnitudes,
                              f_interpolation_factor,
                              morph_sound_frames[MorphLocation::Left].harmonics_mags,
                              morph_sound_frames[MorphLocation::Right].harmonics_mags,
                              i_max_harmonics,
                              idx_harmonics);
        }
        
        // TODO - Add sinusoidal component
        
        if ( morphed_sound_frame.hasStochastic() )
        {
            // Interpolating the stochastic components of the given harmonics
            morphed_sound_frame.stochastic =
            interpolateFrames(FrameType::Stochastic,
                              f_interpolation_factor,
                              morph_sound_frames[MorphLocation::Left].stochastic,
                              morph_sound_frames[MorphLocation::Right].stochastic,
                              i_max_harmonics);
        }
        
        if ( morphed_sound_frame.hasResidual() )
        {
            // Interpolating the stochastic components of the given harmonics
            morphed_sound_frame.residual =
            interpolateFrames(FrameType::Residual,
                              f_interpolation_factor,
                              morph_sound_frames[MorphLocation::Left].residual,
                              morph_sound_frames[MorphLocation::Right].residual,
                              i_frame_length);
        }
        
        return morphed_sound_frame;
    }
    
    std::vector<float> Instrument::getNextFrame(float f_note, int i_velocity, int i_frame_length, float f_interpolation_factor)
    {
        // If a note is being played
        if (this->playing_note)
        {
            // Synthesis parameters shortcuts
            const int i_fft_size = this->synthesis.parameters.fft_size;
            const int i_hop_size = this->synthesis.parameters.hop_size;
            int* i_current_frame = &this->synthesis.live_values.i_current_frame;
            
            // Output
            std::vector<float> frame;
            
            while (this->synthesis.live_values.i_samples_ready < i_frame_length)
            {
                MorphSounds morph_sounds = getCloserSounds( f_note, i_velocity );
                
                // If ADSR is NOT on "Release" state
                if (this->adsr != StateADSR::Release)
                {
                    // Compute common looping regions
                    int max_loop_start = std::max(morph_sounds[MorphLocation::Left]->loop.start,
                                                  morph_sounds[MorphLocation::Right]->loop.start);
                    
                    int min_loop_end = std::min(morph_sounds[MorphLocation::Left]->loop.end,
                                                morph_sounds[MorphLocation::Right]->loop.end);
                    
                    // If loop mode is enabled
                    if (this->loop_mode)
                    {
                        // Keep the current_frame pointer inside the lowest "note.loop.end"
                        // and the highest "note.loop.start"
                        if ( (*i_current_frame * i_hop_size) >= min_loop_end )
                        {
                            this->adsr = StateADSR::Sustain;
                            *i_current_frame = int( max_loop_start / i_hop_size );
                        }
                    }
                    // If "loop_mode" is NOT activated
                    else
                    {
                        // If we are beyond the end loop point
                        if (*i_current_frame >= min_loop_end)
                        {
                            // Switch ADSR to "Relase" mode
                            this->adsr = StateADSR::Release;
                        }
                    }
                }
                
                // If ADSR is on "Release" state
                if (this->adsr == StateADSR::Release)
                {
                    // TODO - JUMP CURRENT FRAME TO RELEASE SECTION, if defined of course,
                    // if not, play the rest of the other morph_sounds or shorten the whole thing
                    // to the shortest one and apply a fade out on hramonics_mags 4 frames before the end
                    
                    // Get the minimum length of both notes
                    int min_note_end = std::min(morph_sounds[MorphLocation::Left]->max_frames,
                                                morph_sounds[MorphLocation::Right]->max_frames);
                    
                    // If we are on the last frame of the shortest note
                    if (*i_current_frame >= min_note_end)
                    {
                        // End note playback
                        this->playing_note = false;
                        this->synthesis.live_values.last_frame = true;
                    }
                }
                
                Sound::Frame sound_frame;
                
                if (morph_sounds[MorphLocation::Left] == morph_sounds[MorphLocation::Right])
                {
                    sound_frame = morph_sounds[MorphLocation::Left]->getFrame(*i_current_frame, i_hop_size);
                    
                    // Get target frequency
                    float f_target_frequency = Tools::Midi::toFreq(f_note);
                    
                    // Transpose left note frequencies to the target frequency
                    Tools::Calculate::divideByScalar(sound_frame.harmonics_freqs,
                                                     (Tools::Midi::toFreq(morph_sounds[MorphLocation::Left]->note) * f_target_frequency) );
                }
                else
                {
                    sound_frame = morphSoundFrames(f_note, morph_sounds, *i_current_frame, i_frame_length);
//                    sound_frame = morphSoundFrames(f_note, morph_sounds, *i_current_frame, i_hop_size, f_interpolation_factor);
                }
                
                // NOTE - "frame" will have "i_hop_size" more samples ready
                // to be played after each call
                frame = this->synthesis.generateSoundFrame(sound_frame, i_fft_size);
                // std::vector<float> generated_sound = this->synthesis.generateSoundFrame(sound_frame, i_frame_length);
                // generated_sound = self.synthesis.generateSoundFrame(sound, hop_size)
                
                i_current_frame ++;
            }
            
            // Update samples ready to be played
            this->synthesis.live_values.i_samples_ready -= frame.size();
            
            return frame;
        }
        // If no note is being played
        else
        {
            return std::vector<float>(0.0, i_frame_length);
        }
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
        
        int len_frame_1 = 0;
        int len_frame_2 = 0;

        // Magnitudes
        if (frame_type == FrameType::Magnitudes)
        {
            
            len_frame_1 = (int)count_if(frame_1.begin(), frame_1.end(),
                                        [](float value) { return value > DEFAULT_DB; });
            
            len_frame_2 = (int)count_if(frame_2.begin(), frame_2.end(),
                                        [](float value) { return value > DEFAULT_DB; });
            
            // Fill with -180.0
            std::fill(interpolated_frame.begin(), interpolated_frame.end(), DEFAULT_DB);
        }
        // Harmonics (and everything else)
        else
        {
            len_frame_1 = (int)count_if(frame_1.begin(), frame_1.end(),
                                        [](float value) { return value > DEFAULT_HZ; });
            
            len_frame_2 = (int)count_if(frame_2.begin(), frame_2.end(),
                                        [](float value) { return value > DEFAULT_HZ; });
            
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
                interpolated_frame[i] = (1-interp_factor) * master_frame[i] + interp_factor * second_frame[i];
            }
            else
            {
                interpolated_frame[i] = master_frame[i];
            }
        }
        
        return interpolated_frame;
    }

} // namespace Core
