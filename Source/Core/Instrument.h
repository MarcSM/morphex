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
    //    typedef std::array<std::unique_ptr<Sound>, MorphLocation::NUM_MORPH_LOCATIONS> MorphSounds;
    
    const static int NUM_MIDI_NOTES = 128;
}

//const static float DEFAULT_FREQ = 0.0;
//const static float DEFAULT_MAG = -180.0;
//const static float DEFAULT_STOC = -180.0;
//const static float DEFAULT_HZ = 0.0;
//const static float DEFAULT_DB = -180.0;
//const static int XML_DECIMAL_PLACES = 5;

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
    
//    // Voices
//    std::vector<Voice*> voice;
    
    // Data
    std::string name;
    std::string samples_dirpath;
    int num_samples_loaded = 0;

    // Mode
    Mode mode = Mode::Morphing;
//    Mode mode = Mode::FullRange;

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
    
    std::vector<float> getNextFrame (float f_note, float f_velocity, int i_frame_length, float f_freqs_interp_factor, float f_mags_interp_factor);
    
    std::vector<float> interpolateFrames (FrameType frame_type,
                                          float interp_factor,
                                          std::vector<float> frame_1,
                                          std::vector<float> frame_2,
                                          int i_frame_length,
                                          std::vector<int> idx_harmonics = std::vector<int>());
private:
    
    MorphNotes morph_notes;
    MorphSounds morph_sounds {};

//    std::vector<float> interpolateFrames(FrameType frame_type,
//                                         float interp_factor,
//                                         std::vector<float> frame_1,
//                                         std::vector<float> frame_2,
//                                         int i_frame_length,
//                                         std::vector<int> idx_harmonics = std::vector<int>());
    
//    def interpolateFrames(self, interpolation_mode, interp_factor, frame_1, frame_2, max_harmonics, harmonics = []):
//    
//#     # TODO CHECK - Compute common looping regions
//#     max_loop_start = \
//#         max(left_note_and_velocity.loop.start,
//#             right_note_and_velocity.loop.start)
//    
//#     interpolated_note.harmonics = np.arange(start=0, stop=MAX_HARMONICS, step=1)
//    
//# Default initialization
//#     interpolated_frame = np.zeros( len(frame_1) )
//    
//# print("len(harmonics): ", len(harmonics))
//# print("len(frame_1): ", len(frame_1))
//# print("len(frame_2): ", len(frame_2))
//# assert False
//    
//    interpolated_frame = np.zeros(max_harmonics)
//    
//# len_frame_1 = len(frame_1)
//# len_frame_2 = len(frame_2)
//    
//# Magnitudes Initialization
//    if interpolation_mode == FrameInterpolationMode.Magnitudes:
//    
//    len_frame_1 = np.sum(frame_1 > DEFAULT_MAG)
//    len_frame_2 = np.sum(frame_2 > DEFAULT_MAG)
//    
//# master_frame = np.zeros(max_harmonics)
//# second_frame = np.zeros(max_harmonics)
//    
//# Fill with -180.0
//    interpolated_frame.fill(DEFAULT_MAG)
//# master_frame.fill(DEFAULT_MAG)
//# second_frame.fill(DEFAULT_MAG)
//    
//# if len_frame_1 > len_frame_2:
//#     master_frame[:len(frame_1)] = frame_1
//#     second_frame[:len(frame_2)] = frame_2
//# else:
//#     master_frame[:len(frame_2)] = frame_2
//#     second_frame[:len(frame_1)] = frame_1
//    
//# frame_1_padded.fill(DEFAULT_MAG)
//# frame_2_padded.fill(DEFAULT_MAG)
//    
//# # Harmonics Initialization
//# if interpolation_mode == FrameInterpolationMode.Frequencies:
//    else:
//    
//    len_frame_1 = np.sum(frame_1 > DEFAULT_FREQ)
//    len_frame_2 = np.sum(frame_2 > DEFAULT_FREQ)
//    
//# Fill with 0.0
//    interpolated_frame.fill(DEFAULT_FREQ)
//# frame_1_padded.fill(DEFAULT_FREQ)
//# frame_2_padded.fill(DEFAULT_FREQ)
//    
//# if len_frame_1 > len_frame_2:
//#     master_frame = copy.copy( frame_1 )
//#     second_frame = copy.copy( frame_2 )
//# else:
//#     master_frame = copy.copy( frame_2 )
//#     second_frame = copy.copy( frame_1 )
//    
//    
//    if len_frame_1 > len_frame_2:
//    master_frame = copy.copy( frame_1 )
//    second_frame = copy.copy( frame_2 )
//    else:
//    master_frame = copy.copy( frame_2 )
//    second_frame = copy.copy( frame_1 )
//    
//    interpolated_frame[:len(master_frame)] = master_frame
//    interpolated_frame[:len(second_frame)] = (1-interp_factor) * master_frame[:len(second_frame)] + interp_factor * second_frame[:len(second_frame)]
//    
//    
//    
//    
//    
//    
//    
//# interpolated_frame[harmonics] = (1-interp_factor) * frame_1_padded[harmonics] + interp_factor * frame_2_padded[harmonics] # OTC
//    
//# frame_1_padded = np.zeros(max_harmonics)
//# frame_2_padded = np.zeros(max_harmonics)
//    
//    
//# frame_1_padded[:len(frame_1)] = frame_1
//# frame_2_padded[:len(frame_2)] = frame_2
//    
//# TODO - Interpolations can be done with the same function, fix this
//    
//# interpolated_frame[harmonics] = (1-interp_factor) * frame_1_padded[harmonics] + interp_factor * frame_1_padded[harmonics]
//    
//# print("frame_1_padded: ", frame_1_padded)
//# print("frame_2_padded: ", frame_2_padded)
//# print("A - interpolated_frame[harmonics]: ", interpolated_frame[harmonics])
//    
//    
//    
//# print("B - interpolated_frame[harmonics]: ", interpolated_frame[harmonics])
//    
//# # Stochastic Interpolation
//# if interpolation_mode == FrameInterpolationMode.Stochastic:
//# #         print("- FrameInterpolationMode.Stochastic -")
//#     interpolated_frame = (1-interp_factor) * frame_1_padded[:] + interp_factor * frame_2_padded[:]
//    
//# # Residual Interpolation
//# elif interpolation_mode == FrameInterpolationMode.Residual:
//#     interpolated_frame = (1-interp_factor) * frame_1_padded + interp_factor * frame_2_padded
//# else:
//    
//# #         print("interpolated_frame: ", interpolated_frame)
//# #         print("len(interpolated_frame): ", len(interpolated_frame))
//# #         print("harmonics: ", harmonics)
//# #         print("len(frame_1): ", len(frame_1))
//# #         print("len(frame_2): ", len(frame_2))
//    
//#     # print("harmonics: ", harmonics)
//#     # print("interp_factor: ", interp_factor)
//#     # print("type(frame_1): ", type(frame_1))
//#     # print("type(frame_2): ", type(frame_2))
//#     # print("interpolated_frame[harmonics]: ", interpolated_frame[harmonics])
//#     # print("interp_factor: ", interp_factor)
//    
//    
//#     # print("- - - - - - - - - - - - -\n\n")
//    
//#     # print("max_harmonics: ", max_harmonics)
//#     # print("frame_1.shape: ", frame_1.shape)
//#     # print("frame_2.shape: ", frame_2.shape)
//#     # print("frame_1: ", frame_1)
//#     # print("frame_2: ", frame_2)
//    
//#     # print("frame_1[harmonics]: ", frame_1[harmonics])
//#     # print("frame_2[harmonics]: ", frame_2[harmonics])
//    
//#     # print("(1-interp_factor) * frame_1[harmonics]: ", (1-interp_factor) * frame_1[harmonics])
//#     # print("interp_factor * frame_2[harmonics]: ", interp_factor * frame_2[harmonics])
//#     # print("(1-interp_factor) * frame_1[harmonics] + interp_factor * frame_2[harmonics]: ", (1-interp_factor) * frame_1[harmonics] + interp_factor * frame_2[harmonics])
//    
//#     # print("len(harmonics): ", len(harmonics))
//#     # print("len(frame_1): ", len(frame_1))
//#     # print("len(frame_2): ", len(frame_2))
//    
//#     # print("harmonics: ", harmonics)
//#     # print("frame_1: ", frame_1)
//#     # print("frame_2: ", frame_2)
//#     # print("frame_1[harmonics]: ", frame_1[harmonics])
//#     # print("frame_2[harmonics]: ", frame_2[harmonics])
//    
//#     interpolated_frame[harmonics] = (1-interp_factor) * frame_1_padded[harmonics] + interp_factor * frame_2_padded[harmonics]
//    
//# #         print("interpolated_frame after: ", interpolated_frame)
//    
//# #         assert False
//    
//    return interpolated_frame
};