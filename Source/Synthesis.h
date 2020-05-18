/*
  ==============================================================================

    Synthesis.h
    Created: 22 Apr 2020 11:41:19am
    Author:  Marc Sanchez Martinez

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

#include "Tools.h"
#include "Sound.h"

#include "SMTConstants.h"

#include <math.h>
#include <vector>

//static double MidiMessage::getMidiNoteInHertz(int     noteNumber,
//                                              double     frequencyOfA = 440.0)

//static double MidiMessage::getMidiNoteInHertz    (    int     noteNumber,
//                                                  double     frequencyOfA = 440.0)

enum Channel
{
    Left = 0,
    Right,
    NUM_CHANNELS,
    Mono = Left
};

namespace Core { class Synthesis; }

class Core::Synthesis
{
public:
    
    const static int MAX_HARMONICS = 100;
    const static int FS = 44100;
    const static int FFT_SIZE = 512; // 512
    const static int HOP_SIZE = int( FFT_SIZE / 4 ); // 128
    const static int NUM_FRAMES_IN_BUFFER = 4;
    
    enum BufferSection
    {
        Write = 0,
        Clean,
        Play
    };
    
    enum BufferUpdateMode
    {
        Set = 0,    // The given frame will replace the current content of the buffer
        Add,        // The given frame will be added to the current content of the buffer
        Delete      // The current content of the buffer will be set to 0.0
    };
    
    struct SynthesisParameters
    {
        int fs;
        int fft_size;
        int hop_size;
        bool generate_residual = true;
    };
    SynthesisParameters parameters;
    
    struct SynthesisLiveValues
    {
        std::vector<float> last_freqs;
        std::vector<float> harmonic_phases;
        std::vector<float> sinusoidal_phases;
        int i_samples_ready;
        int i_current_frame;
        bool first_frame;
        bool last_frame;
    };
    SynthesisLiveValues live_values;
    
    struct SynthesisGenerated
    {
        std::vector<float> y;
        std::vector<float> harmonics_freqs;
        std::vector<float> harmonics_mags;
        std::vector<float> harmonics_phases;
        std::vector<float> stochastic;
        std::vector<float> residual;
    } generated;

    struct Buffer
    {
        std::vector<float> channels[Channel::NUM_CHANNELS];
        
        int length;
        
        struct Pointers
        {
//            Synthesis& parent;
//            Pointers(Synthesis& parent_) : parent(parent_) {}
            
//            Synthesis* parent;
//            Synthesis& parent;

            int write;
            // Try to use lambdas if possible
            int clean(Synthesis* parent, int i_move_position = 0) { return parent->getPointerInLimits( write + parent->parameters.fft_size + i_move_position ); };
            int play;
//            int play(Synthesis* parent, int i_move_position = 0) { return parent->getPointerInLimits( write - parent->parameters.fft_size + i_move_position ); };
        } pointers;
//        SynthesisBufferPointers(parent) pointers;
        
    } buffer;
//    SynthesisBuffer(this) buffer;
    
    struct SynthesisWindow
    {
        std::vector<float> harm;
        std::vector<float> stoc;
    };
    SynthesisWindow window;
    
    dsp::FFT* fft;

    Synthesis();
    ~Synthesis();

    void reset();
    
    std::vector<float> getBuffer(BufferSection buffer_section, Channel selected_channel = Channel::Mono, int i_frame_length = 0);
    void generateSoundFrame(Sound::Frame sound_frame, bool append_to_generated = false);
    
    void updatePlayPointer(int i_pointer_increment);
    void updatePhases(std::vector<float> harmonics_freqs, std::vector<int> idx_harmonics, int hop_size, bool append_to_generated = false);
    void updateLastFreqs(std::vector<float> harmonics_freqs, std::vector<int> idx_harmonics);
    
private:
    
//    std::vector<int> idx_harmonics;
//    std::vector<float> windowed_audio_frame;

    void getWindow();
    
    int getPointerInLimits(int i_pointer_position);
    std::vector<int> getBufferIndexes(int i_head, int i_tail);
    std::vector<int> getBufferSectionIndexes(BufferSection buffer_section, int i_frame_length = 0);
    
    void updateWritePointer(int i_pointer_increment);
    void updateBuffer(BufferSection buffer_section, BufferUpdateMode update_mode, std::vector<float> given_frame = std::vector<float>(), Channel selected_channel = Channel::Mono);
//    void updatePhases(std::vector<float> harmonics_freqs, std::vector<int> idx_harmonics, int hop_size, bool append_to_generated = false);
//    void updateLastFreqs(std::vector<float> harmonics_freqs, std::vector<int> idx_harmonics);
    
    std::vector<float> synthesizeSoundFrame(Sound::Frame sound_frame, std::vector<int> idx_harmonics);
    std::vector<float> generateSines(std::vector<float> iploc, std::vector<float> ipmag, std::vector<float> ipphase, int NS, int fs);
    std::vector<float> generateStocs(std::vector<float> stocs_morph, int H, int NS);
    
    void genspecsines_C(float *iploc, float *ipmag, float *ipphase, int n_peaks, float *real, float*imag, int size_spec);
};
