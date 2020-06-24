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

#include "Instrument.h"
#include "Tools.h"

namespace Core
{
    Instrument::Instrument()
    {
        // Notes
        this->note = std::vector<Note*> (NUM_MIDI_NOTES);
        
        for (int i = 0; i < this->note.size(); i++)
        {
            this->note[i] = new Note (i);
        }
        
        this->init();
    };
    
    Instrument::~Instrument() {}
    
    void Instrument::init()
    {
        // Data
        this->name = "New Instrument";
        this->samples_dirpath = "";
        
        // Morph Notes
        for (int i = 0; i < MorphLocation::NUM_MORPH_LOCATIONS; i++)
        {
            this->setMorphNote (this->note[i], (MorphLocation) i);
        }
        
        // Mode
        if (this->mode == Mode::FullRange) this->interpolation_mode = Interpolation::None;
    }
    
    void Instrument::reset()
    {
        this->init();
        
        // Reset Notes
        for (int i = 0; i < this->note.size(); i++)
        {
            this->note[i]->reset();
        }
    }
    
    void Instrument::loadSound (std::string file_path, MorphLocation morph_location)
    {
        Sound sound = Sound (file_path);
        
        // TODO - Improve this code
        if (morph_location < NUM_MORPH_LOCATIONS)
        {
            MorphLocation other_sound_morph_location = MorphLocation::NUM_MORPH_LOCATIONS;
            
            if (morph_location == MorphLocation::Left)
            {
                other_sound_morph_location = MorphLocation::Right;
            }
            else if (morph_location == MorphLocation::Right)
            {
                other_sound_morph_location = MorphLocation::Left;
            }
            
            Sound* other_loaded_sound = this->getMorphSound (other_sound_morph_location);
            
            if (sound.note == other_loaded_sound->note and
                sound.velocity == other_loaded_sound->velocity)
            {
                if (sound.velocity == 0) sound.velocity = MAX_MIDI_VELOCITY;
                else if (sound.velocity == 1) sound.velocity = MAX_MIDI_VELOCITY;
                else sound.velocity = 1;
            }
        }
        
        this->note[sound.note]->velocity[sound.velocity]->sound = sound;
        this->note[sound.note]->velocity[sound.velocity]->loaded = true;
        
        if (morph_location < NUM_MORPH_LOCATIONS)
        {
            this->setMorphNote (this->note[sound.note], morph_location, sound.velocity);
        }
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
                loaded_notes.push_back (note);
            }
        }
        
        return loaded_notes;
    }
    
    MorphNotes Instrument::getCloserNotes (float f_target_note)
    {
        int l_i_target_note = round (f_target_note);
        int h_i_target_note = ceil (f_target_note);
        
        // Output (use "{}" to ensure "nullptr" initialization)
        MorphNotes closer_notes {};
        
        std::vector<Note*> loaded_notes = getLoadedNotes();

        // Instrument::Mode::Morphing
        if (this->mode == Instrument::Mode::Morphing)
        {
            int i_notes_to_load = std::min ((int) loaded_notes.size(), (int) MorphLocation::NUM_MORPH_LOCATIONS);

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
    
    MorphSounds Instrument::getCloserSounds (float f_target_note, float f_velocity)
    {
        MorphNotes closer_notes = getCloserNotes (f_target_note);
        
        // Output
        MorphSounds closer_sounds {};
        
        // Transform velocity range from 0-1 to 0-127
        float f_velocity_midi_range = jmap (f_velocity, 0.0f, 1.0f, 1.0f, 127.0f);
        
        // Getting closer velocities
        for (int i = 0; i < closer_notes.size(); i++)
        {
            if (closer_notes[i] != nullptr)
            {
                std::vector<Velocity*> loaded_velocities = closer_notes[i]->getLoadedVelocities();
                
                int i_closer_velocity = 0;
                float f_shortest_velocity_distance = MAX_MIDI_VELOCITY;
                
                for (int j = 0; j < loaded_velocities.size(); j++)
                {
                    float f_velocity_distance = std::abs (loaded_velocities[j]->value - f_velocity_midi_range);
                    
                    // Priority to lower velocities ("<=" for upper velocities)
                    if (f_velocity_distance < f_shortest_velocity_distance)
                    {
                        i_closer_velocity = j;
                        f_shortest_velocity_distance = f_velocity_distance;
                    }
                }
                
                closer_sounds[i] = &loaded_velocities[i_closer_velocity]->sound;
            }
        }
        
        return closer_sounds;
    }
    
    MorphNotes Instrument::getMorphNotes()
    {
        return morph_notes;
    }
    
    void Instrument::setMorphNote (Note* note, MorphLocation morph_location, int midi_velocity)
    {
        morph_notes[morph_location] = note;
        
        this->setMorphSound (&morph_notes[morph_location]->velocity[midi_velocity]->sound, morph_location);
    }
    
    MorphSounds Instrument::getMorphSounds()
    {
        return morph_sounds;
    }
    
    Sound* Instrument::getMorphSound (MorphLocation morph_location)
    {
        return morph_sounds[morph_location];
    }
    
    void Instrument::setMorphSound (Sound* sound, MorphLocation morph_location)
    {
        morph_sounds[morph_location] = sound;
    }
    
    Sound::Frame Instrument::getSoundFrame (float f_note, float f_velocity, int i_current_frame, int i_frame_length, float f_freqs_interp_factor, float f_mags_interp_factor)
    {
        MorphSounds morph_sounds = getCloserSounds (f_note, f_velocity);
        
        if (morph_sounds[MorphLocation::Left] == morph_sounds[MorphLocation::Right])
        {
            return morph_sounds[MorphLocation::Left]->getFrame (i_current_frame, i_frame_length);
        }
        else
        {
            return morphSoundFrames (f_note, morph_sounds, i_current_frame, i_frame_length, f_freqs_interp_factor, f_mags_interp_factor);
        }
    }
    
    // NOTE: About looping, two modes, the first one is that each note loops
    // on their own loop points and then do the morphing, this will cause unalignment.
    // The second one is two have the maximum starting loop point and the minimum ending
    // loop point, the loop section will be smaller but it will sound homogeneous
    // among iterations.
    Sound::Frame Instrument::morphSoundFrames (float f_target_note, MorphSounds morph_sounds, int i_current_frame, int i_frame_length, float f_freqs_interp_factor, float f_mags_interp_factor)
    {
        // Output
        Sound::Frame morphed_sound_frame;

//        // Get the maximum number of hamronics and sound length
//        int i_max_harmonics = std::max( morph_sounds[MorphLocation::Left]->max_harmonics, morph_sounds[MorphLocation::Right]->max_harmonics );
//
        // Get target frequency
        float f_target_frequency = Tools::Midi::toFreq (f_target_note);
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
                (f_target_frequency - Tools::Midi::toFreq (morph_sounds[MorphLocation::Left]->note)) /
                (Tools::Midi::toFreq (morph_sounds[MorphLocation::Right]->note) - Tools::Midi::toFreq (morph_sounds[MorphLocation::Left]->note));
            }
            
            f_mags_interp_factor = f_freqs_interp_factor;
        }
        
        float f_stocs_interp_factor = f_freqs_interp_factor;
        float f_attack_interp_factor = f_freqs_interp_factor;
        float f_residual_interp_factor = f_freqs_interp_factor;

        MorphSoundFrames morph_sound_frames;
        
        // Select current frame
        morph_sound_frames[MorphLocation::Left] = morph_sounds[MorphLocation::Left]->getFrame (i_current_frame, i_frame_length);
        morph_sound_frames[MorphLocation::Right] = morph_sounds[MorphLocation::Right]->getFrame (i_current_frame, i_frame_length);
        
        // Get the maximum number of hamronics and sound length
        int i_max_harmonics = std::max (morph_sound_frames[MorphLocation::Left].getMaxHarmonics(),
                                        morph_sound_frames[MorphLocation::Right].getMaxHarmonics());
        
        // NOTE - If it doesn't sound properly, try to always give more weight
        // to the higer samples (it is necessary to define a function that,
        // given two frequencies, calculate the weight that is given to each
        // one so that it contributes to the morphed sample).
        // Saparate frecuency and magnitude interpolation factors?
        
        // A list with the indexes of the harmonics we want to interpolate
        std::vector<int> idx_harmonics = Tools::Generate::range (0, i_max_harmonics);
        
//         // Live funcamental transposition
//         def transposeFreqs(note_hfreq, f_note_freq, f_target_frequency):
// //             currrent_fundamental = instrument.available_notes[24][100].analysis.output.values.hfreq[0][0]
// //             explicit_fundamental = i_note
//             return (note_hfreq / f_note_freq) * f_target_frequency
        
        if (morph_sound_frames[MorphLocation::Left].hasHarmonic() or
            morph_sound_frames[MorphLocation::Right].hasHarmonic())
        {
            // TODO - Check parameter morph_sounds[MorphLocation::Left]->parameters.transpose.harmonic
            // if false, do not transpose the frame of this note
            
            // Transpose left note frequencies to the target frequency
            Tools::Calculate::divideByScalar (morph_sound_frames[MorphLocation::Left].harmonic.freqs,
                                              Tools::Midi::toFreq(morph_sounds[MorphLocation::Left]->note));
            Tools::Calculate::multiplyByScalar (morph_sound_frames[MorphLocation::Left].harmonic.freqs,
                                                f_target_frequency);
            
            // Transpose right note frequencies to the target frequency
            Tools::Calculate::divideByScalar (morph_sound_frames[MorphLocation::Right].harmonic.freqs,
                                              Tools::Midi::toFreq(morph_sounds[MorphLocation::Right]->note));
            Tools::Calculate::multiplyByScalar (morph_sound_frames[MorphLocation::Right].harmonic.freqs,
                                                f_target_frequency);
            
            // Interpolating the frequencies of the given harmonics
            morphed_sound_frame.harmonic.freqs =
            interpolateFrames (FrameType::Frequencies,
                               f_freqs_interp_factor,
                               morph_sound_frames[MorphLocation::Left].harmonic.freqs,
                               morph_sound_frames[MorphLocation::Right].harmonic.freqs,
                               i_max_harmonics, idx_harmonics);
            
            // Interpolating the magnitudes of the given harmonics
            morphed_sound_frame.harmonic.mags =
            interpolateFrames (FrameType::Magnitudes,
                               f_mags_interp_factor,
                               morph_sound_frames[MorphLocation::Left].harmonic.mags,
                               morph_sound_frames[MorphLocation::Right].harmonic.mags,
                               i_max_harmonics,
                               idx_harmonics);
        }
        
        // TODO - Add sinusoidal component
//        if (morph_sound_frames[MorphLocation::Left].hasSinusoidal() or
//            morph_sound_frames[MorphLocation::Right].hasSinusoidal())
//        {
//            // Transpose left note frequencies to the target frequency
//            Tools::Calculate::divideByScalar(morph_sound_frames[MorphLocation::Left].sinusoidal.freqs,
//                                             Tools::Midi::toFreq(morph_sounds[MorphLocation::Left]->note));
//            Tools::Calculate::multiplyByScalar(morph_sound_frames[MorphLocation::Left].sinusoidal.freqs, f_target_frequency);
//
//            // Transpose right note frequencies to the target frequency
//            Tools::Calculate::divideByScalar(morph_sound_frames[MorphLocation::Right].sinusoidal.freqs,
//                                             Tools::Midi::toFreq(morph_sounds[MorphLocation::Right]->note));
//            Tools::Calculate::multiplyByScalar(morph_sound_frames[MorphLocation::Right].sinusoidal.freqs, f_target_frequency);
//
//            // Interpolating the frequencies of the given sinusoids
//            morphed_sound_frame.sinusoidal.freqs =
//            interpolateFrames(FrameType::Frequencies,
//                              f_freqs_interp_factor,
//                              morph_sound_frames[MorphLocation::Left].sinusoidal.freqs,
//                              morph_sound_frames[MorphLocation::Right].sinusoidal.freqs,
//                              i_max_harmonics,
//                              idx_harmonics);
//
//            // Interpolating the magnitudes of the given sinusoids
//            morphed_sound_frame.sinusoidal.mags =
//            interpolateFrames(FrameType::Magnitudes,
//                              f_mags_interp_factor,
//                              morph_sound_frames[MorphLocation::Left].sinusoidal.mags,
//                              morph_sound_frames[MorphLocation::Right].sinusoidal.mags,
//                              i_max_harmonics,
//                              idx_harmonics);
//        }
        
        if (morph_sound_frames[MorphLocation::Left].hasStochastic() or
            morph_sound_frames[MorphLocation::Right].hasStochastic())
        {
            // Interpolating the stochastic components of the given harmonics
            morphed_sound_frame.stochastic =
            interpolateFrames (FrameType::Stochastic,
                               f_stocs_interp_factor,
                               morph_sound_frames[MorphLocation::Left].stochastic,
                               morph_sound_frames[MorphLocation::Right].stochastic,
                               i_max_harmonics);
        }
        
        if (morph_sound_frames[MorphLocation::Left].hasAttack() or
            morph_sound_frames[MorphLocation::Right].hasAttack())
        {
            // TODO - Check parameter morph_sounds[MorphLocation::Left]->parameters.transpose.attack
            // if false, do not transpose the frame of this note
            
            // Interpolating the stochastic components of the given harmonics
            morphed_sound_frame.attack =
            interpolateFrames (FrameType::Waveform,
                               f_attack_interp_factor,
                               morph_sound_frames[MorphLocation::Left].attack,
                               morph_sound_frames[MorphLocation::Right].attack,
                               i_frame_length);
        }
        
        if (morph_sound_frames[MorphLocation::Left].hasResidual() or
            morph_sound_frames[MorphLocation::Right].hasResidual())
        {
            // TODO - Check parameter morph_sounds[MorphLocation::Left]->parameters.transpose.residual
            // if false, do not transpose the frame of this note
            
            // Interpolating the stochastic components of the given harmonics
            morphed_sound_frame.residual =
            interpolateFrames (FrameType::Waveform,
                               f_residual_interp_factor,
                               morph_sound_frames[MorphLocation::Left].residual,
                               morph_sound_frames[MorphLocation::Right].residual,
                               i_frame_length);
        }
        
        return morphed_sound_frame;
    }
    
    std::vector<float> Instrument::interpolateFrames (FrameType frame_type,
                                                      float interp_factor,
                                                      std::vector<float> frame_1,
                                                      std::vector<float> frame_2,
                                                      int i_frame_length,
                                                      std::vector<int> idx_harmonics)
    {
        // TODO - Get rid of the for loops, use matrix multiplications instead
        // NOTE - idx_harmonics is not beign used right now
        
        // Stop synthesis when shortest sound has finished
        bool stop_at_shortest = true;
        
        float DEFAULT_VALUE = (frame_type == FrameType::Magnitudes) ? DEFAULT_DB : DEFAULT_HZ;
        
        // Output
        std::vector<float> interpolated_frame (i_frame_length, DEFAULT_VALUE);
        
        if (i_frame_length != 0)
        {
            // TODO - Check if all elements in the magnitude frame are 0.0
            // do this when loading the sound
            
//            if (frame_type == FrameType::Magnitudes and
//                (std::equal (frame_1.begin() + 1, v.end(), v.begin()) or
//                 std::equal (v.begin() + 1, v.end(), v.begin()))
//            {
//
//            }
            
            // Aux values
            float aux_value_1 = DEFAULT_VALUE;
            float aux_value_2 = DEFAULT_VALUE;
            
            for (int i = 0; i < i_frame_length; i++)
            {
                if (stop_at_shortest and (frame_1.size() == 0 or frame_2.size() == 0))
                {
                    interpolated_frame[i] = DEFAULT_VALUE;
                }
                else
                {
                    if (frame_type == FrameType::Magnitudes)
                    {
                        if (i < frame_1.size() and abs (frame_1[i]) != 0.0) aux_value_1 = frame_1[i];
                        else aux_value_1 = DEFAULT_VALUE;
                        
                        if (i < frame_2.size() and abs (frame_2[i]) != 0.0) aux_value_2 = frame_2[i];
                        else aux_value_2 = DEFAULT_VALUE;
                    }
                    else
                    {
                        if (i < frame_1.size()) aux_value_1 = frame_1[i];
                        else aux_value_1 = DEFAULT_VALUE;
                        
                        if (i < frame_2.size()) aux_value_2 = frame_2[i];
                        else aux_value_2 = DEFAULT_VALUE;
                    }
                    
                    interpolated_frame[i] = interp_factor * aux_value_2 + (1-interp_factor) * aux_value_1;
                }
            }
        }
        
        return interpolated_frame;
    }
} // namespace Core
