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

#include "../JuceLibraryCode/JuceHeader.h"

#include "Instrument.h"
#include "Sound.h"
#include "Tools.h"

#include "../Helpers/SMTConstants.h"

#include <math.h>
#include <vector>

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
    const static int MAX_SINUSOIDS = 100;
    const static int FS = 44100;
    const static int FFT_SIZE = 512; // 512
    const static int HOP_SIZE = int (FFT_SIZE / 4); // 128
    const static int NUM_FRAMES_IN_BUFFER = 8; // 512 * 8 = 4096
    
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
    
    struct Parameters
    {
        int fs;
        int fft_size;
        int hop_size;
    };
    Parameters parameters;

    struct SynthesisLiveValues
    {
        struct FP
        {
            std::vector<float> last_freqs;
            std::vector<float> phases;
        };
        
        FP harmonic;
        FP sinusoidal;
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
    };
    SynthesisGenerated generated;

    struct Buffer
    {
        std::vector<float> channels[Channel::NUM_CHANNELS];
        
        int length;
        
        struct Pointers
        {
            int write;
            // Try to use lambdas if possible
            int clean (Synthesis* parent, int i_move_position = 0) { return parent->getPointerInLimits (write + parent->parameters.fft_size + i_move_position); };
            int play;
//            int play(Synthesis* parent, int i_move_position = 0) { return parent->getPointerInLimits( write - parent->parameters.fft_size + i_move_position ); };
        };
        Pointers pointers;
        
    };
    Buffer buffer;
    
    struct SynthesisWindow
    {
        std::vector<float> harm;
        std::vector<float> stoc;
    };
    SynthesisWindow window;
    
    dsp::FFT* fft;

    Synthesis (Instrument* instrument);
    ~Synthesis();

    void reset();
    
    std::vector<float> getBuffer (BufferSection buffer_section, Channel selected_channel = Channel::Mono, int i_frame_length = 0);
    
    void generateSoundFrame (Sound::Frame sound_frame, bool append_to_generated = false);
    
    void updatePlayPointer (int i_pointer_increment);
    void updatePhases (SynthesisLiveValues::FP& fp_values, std::vector<float> freqs, std::vector<int> idx_freqs, int hop_size, bool append_to_generated = false);
    void updateLastFreqs (SynthesisLiveValues::FP& fp_values, std::vector<float> freqs, std::vector<int> idx_freqs);
    
private:
    
    Instrument* instrument;

    void getWindow();
    
    int getPointerInLimits (int i_pointer_position);
    std::vector<int> getBufferIndexes (int i_head, int i_tail);
    std::vector<int> getBufferSectionIndexes (BufferSection buffer_section, int i_frame_length = 0);
    
    void updateWritePointer (int i_pointer_increment);
    void updateBuffer (BufferSection buffer_section, BufferUpdateMode update_mode, std::vector<float> given_frame = std::vector<float>(), Channel selected_channel = Channel::Mono);
//    void updatePhases (std::vector<float> harmonics_freqs, std::vector<int> idx_harmonics, int hop_size, bool append_to_generated = false);
//    void updateLastFreqs (std::vector<float> harmonics_freqs, std::vector<int> idx_harmonics);
    
    std::vector<float> synthesizeSoundFrame (Sound::Frame sound_frame);
    std::vector<float> generateSines (std::vector<float> iploc, std::vector<float> ipmag, std::vector<float> ipphase, int NS, int fs);
    std::vector<float> generateStocs (std::vector<float> stocs_morph, int H, int NS);
    
    void genspecsines_C (float *iploc, float *ipmag, float *ipphase, int n_peaks, float *real, float*imag, int size_spec);
};
