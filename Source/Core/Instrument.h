/*
  ==============================================================================

    Instrument.h
    Created: 22 Apr 2020 11:40:34am
    Author:  Marc Sanchez Martinez

  ==============================================================================
*/

#pragma once

#include "Note.h"

#include <vector>

namespace Core
{
    class Instrument;
    
    enum MorphLocation
    {
        // TODO - Nested enum for Left Low, Left High and NUM_LEFT_MORPH_SOUNDS
        Left = 0,
        // TODO - Nested enum for Right Low, Right High and NUM_RIGHT_MORPH_SOUNDS
        Right,
        // TODO - NUM_MORPH_LOCATIONS = NUM_LEFT_MORPH_SOUNDS + NUM_RIGHT_MORPH_SOUNDS
        NUM_MORPH_LOCATIONS
    };
    
    typedef std::array<Note*, MorphLocation::NUM_MORPH_LOCATIONS> MorphNotes;
    typedef std::array<Sound*, MorphLocation::NUM_MORPH_LOCATIONS> MorphSounds;
    typedef std::array<Sound::Frame, MorphLocation::NUM_MORPH_LOCATIONS> MorphSoundFrames;
    
    const static int NUM_MIDI_NOTES = 128;
}

class Core::Instrument
{
public:
    
    enum class Mode
    {
        Morphing = 0,
        FullRange
    };
    
    enum class Interpolation
    {
        None = 0,
        Manual,
        FrequencyBased
    };
    
    enum FrameType
    {
        Frequencies = 0,    // Interpolate Frequencies
        Magnitudes,         // Interpolate Magnitudes
        Stochastic,         // Interpolate Stochastic
        Waveform,           // Interpolate Waveforms
    };
    
    struct Generate
    {
        bool harmonic = true;
        bool sinusoidal = true;
        bool stochastic = true;
        bool attack = true;
        bool residual = false;
    };
    Generate generate;
    
    // Notes
    std::vector<Note*> note;
    
    // Data
    std::string name;
    std::string samples_dirpath;
    int num_samples_loaded = 0;

    // Mode
    Mode mode = Mode::Morphing;

    // Interpolation
    Interpolation interpolation_mode = Interpolation::Manual;
    
    Instrument();
    ~Instrument();
    
    void init();
    void reset();
    
    void loadSound (std::string file_path, MorphLocation morph_location = MorphLocation::NUM_MORPH_LOCATIONS);
    
    std::vector<Note*> getLoadedNotes();
    
    MorphNotes getCloserNotes (float f_target_note);
    MorphSounds getCloserSounds (float f_target_note, float f_velocity);
    
    MorphNotes getMorphNotes();
    void setMorphNote (Note* note, MorphLocation morph_location, int midi_velocity = MAX_MIDI_VELOCITY);
    MorphSounds getMorphSounds();
    Sound* getMorphSound (MorphLocation morph_location);
    void setMorphSound (Sound* sound, MorphLocation morph_location);

    Sound* getSound (float f_note, float f_velocity);
    Sound::Frame getSoundFrame (float f_note, float f_velocity, int i_current_frame, int i_frame_length, float f_freqs_interp_factor, float f_mags_interp_factor);
    
    Sound::Frame morphSoundFrames (float f_target_note, MorphSounds morph_sounds, int i_current_frame, int i_frame_length, float f_freqs_interp_factor = -1, float f_mags_interp_factor = -1);
    
    std::vector<float> getNextFrame (float f_note, float f_velocity, int i_frame_length,
                                     float f_freqs_interp_factor, float f_mags_interp_factor);
    
    std::vector<float> interpolateFrames (FrameType frame_type, float interp_factor,
                                          std::vector<float> frame_1, std::vector<float> frame_2,
                                          int i_frame_length, std::vector<int> idx_harmonics = std::vector<int>());
private:
    
    MorphNotes morph_notes;
    MorphSounds morph_sounds {};
};
