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

#include <vector>

//static double MidiMessage::getMidiNoteInHertz(int     noteNumber,
//                                              double     frequencyOfA = 440.0)

//static double MidiMessage::getMidiNoteInHertz    (    int     noteNumber,
//                                                  double     frequencyOfA = 440.0)

enum Channel
{
    Left = 0,
    Right,
    NUM_CHANNELS
};

const static int MAX_HARMONICS = 100;

int FFT_SIZE = 512; // 512
int HOP_SIZE = int( FFT_SIZE / 4 ); // 128

class Synthesis
{
public:
    
    struct SynthesisParameters
    {
        int fs;
        int fft_size;
        int hop_size;
    };
    SynthesisParameters parameters;
    
    struct SynthesisLiveValues
    {
        std::vector<float> last_freqs;
        std::vector<float> phases;
        int i_samples_ready;
        int i_current_frame;
    };
    SynthesisLiveValues live_values;
    
    struct SynthesisGenerated
    {
        std::vector<float> harmonics_freqs;
        std::vector<float> harmonics_mags;
        std::vector<float> harmonics_phases;
        std::vector<float> stochastic;
        std::vector<float> residual;
    } generated;
//    SynthesisGenerated generated;

    struct SynthesisBuffer
    {
        std::vector<float> channels[Channel::NUM_CHANNELS];
//        std::vector<float> channels[Channel::NUM_CHANNELS];

        struct SynthesisBufferPointers
        {
            int write;
            int clean();
//            int clean() { return write + this->parameters.fft_size };
//            int clean;
            // TODO - change to minus hop_size
            int play();
//            int play() { return clean() + this->parameters.hop_size };
//            int play;
        } pointers;
//        SynthesisBufferPointers(parent) pointers;
        
        // TODO - Check if needed
//        struct SynthesisBufferSelected
//        {
//            std::vector<int> to_write();
//            std::vector<int> to_clean();
//            std::vector<int> to_play();
//        } selected;
//        SynthesisBufferSelected(parent) selected;
        
    } buffer;
//    SynthesisBuffer(this) buffer;
    
//    ref class StaticProperties {
//        static int MyInt;
//        static int MyInt2;
//        
//    public:
//        static property int Static_Data_Member_Property;
//        
//        static property int Static_Block_Property {
//            int get() {
//                return MyInt;
//            }
//            
//            void set(int value) {
//                MyInt = value;
//            }
//        }
//    };
//    
//    int main() {
//        StaticProperties::Static_Data_Member_Property = 96;
//        Console::WriteLine(StaticProperties::Static_Data_Member_Property);
//        
//        StaticProperties::Static_Block_Property = 47;
//        Console::WriteLine(StaticProperties::Static_Block_Property);
//    }
    
    struct SynthesisWindow
    {
        std::vector<float> harm;
        std::vector<float> stoc;
    };
    SynthesisWindow window;
    
    Synthesis()
    {
        
    }
    
    void reset()
    {
        this->live_values.i_current_frame = 0;
        
        this->generated.harmonics_freqs.clear();
        this->generated.harmonics_mags.clear();
        this->generated.harmonics_phases.clear();
        this->generated.residual.clear();
        this->generated.stochastic.clear();
        
        this->live_values.last_freqs.resize(MAX_HARMONICS);
        this->live_values.phases.resize(MAX_HARMONICS);
        
        std::fill(this->live_values.last_freqs.begin(),
                  this->live_values.last_freqs.end(), 0.0);
        
        std::fill(this->live_values.phases.begin(),
                  this->live_values.phases.end(), 0.0);

//        std::vector<float> empty_vector(MAX_HARMONICS, 0.0);
//        this->live_values.last_freqs(empty_vector.begin(), empty_vector.end());
//        this->live_values.phases(empty_vector.begin(), empty_vector.end());
//        transform(this->live_values.phases.begin(),
//                  this->live_values.phases.end(),
//                  this->live_values.phases.begin(),
//                  _1 * (2 * np.pi) );
////        this->live_values.last_freqs(MAX_HARMONICS, 0.0);
//        this->live_values.phases(MAX_HARMONICS, 0.0);
//        std::vector<float>* phases = this->live_values.phases;
//        transform(&phases.begin(), &phases.end(), &phases.begin(), _1 * (2 * np.pi) );
//        this->live_values.phases = 2 * np.pi * np.zeros(MAX_HARMONICS)
    }
    
    std::vector<float> synthesizeAudioFrame(Sound sound)
    {
        // Parameters shortcut
        const int NS = this->parameters.fft_size;
        const int H = this->parameters.hop_size;
        const int HNS = int( NS / 2 );
        
        std::vector<int> idx_harmonics = Tools::Generate::range(0, sound.max_harmonics);
//        l_harmonics = np.arange( sound.max_harmonics )
        
        // Instead of Sound, define Sound Frame, Model, or Model frame
        std::vector<float> harmonics_freqs;

        std::vector<float> harmonics_freqs_to_interpolate = Tools::Get::valuesByIndex(harmonics_freqs, idx_harmonics);
        
//        selected_play_samples.insert(selected_play_samples.end(), section_to_play.begin(), section_to_play.end());
        
        // Generate sines in the spectrum
        Y = UF.genSpecSines(
                            sound.model.values.harmonics_freqs,
                            sound.model.values.harmonics_mags,
                            self.live_values.phases[l_harmonics],
                            ns, self.parameters.fs
                            )
        //             self.live_values.phases[note.harmonics],

        // Compute inverse FFT
        y = np.real( fftshift( ifft(Y) ) )

        // Apply the window
        yw = (self.window.harmw * y)

        // Stochastic component
        if sound.model.stochastic:
            
        // y_stoc = stochasticModelSynth(stocs_morph[i], H*2, NS)
            y_stoc = stochasticModelSynth(note.stoc, h * 2, ns)
            yw += (self.window.stocw * y_stoc)
            
        //         yw = (self.harmw * y) + (self.stocw * y_stoc)
        //         yw = (self.harmw * y)
        //         yw = (self.stocw * y_stoc)
            
            return yw
    }
    
    
private:
    
    std::vector<float> getWindow()
    {
        // Parameters shortcut
        const int NS = this->parameters.fft_size;
        const int H = this->parameters.hop_size;
        const int HNS = int( NS / 2 );
        
//        dsp::WindowingFunction<float>::fillWindowingTables( 2 * H, WindowingMethod.triangular, false)
        
//        int a = juce::dsp::WindowingMethod.rectangular;
//        // Window
//
//        std::vector<float> buffer(1024);
//        std::vector<float> asdf(1024);
//        window.fillWindowingTables(buffer.data(), buffer.size(), winName.second, normalize, beta);
//
        
        std::vector<float> ow(2 * H);
        dsp::WindowingFunction<float>::fillWindowingTables( ow.data(), ow.size(), dsp::WindowingFunction<float>::triangular, false);
//        ow = triang( 2 * H )
        
        std::vector<float> bh(NS);
        dsp::WindowingFunction<float>::fillWindowingTables( bh.data(), bh.size(), dsp::WindowingFunction<float>::blackmanHarris, true);
//        bh = blackmanharris( NS )
//        bh = bh / sum(bh)
        
        // Harmonics Window
        this->window.harm.resize(NS);
        std::fill(this->window.harm.begin(),
                  this->window.harm.end(), 0.0);
//        this->window.harm = np.zeros( NS );
        
        for (int i = HNS-H; i < HNS+H; i++)
        {
            this->window.harm[i] = ow[i] / bh[i];
        }
//        this->window.harm[ HNS-H : HNS+H ] = ow;
//        this->window.harm[ HNS-H : HNS+H ] = this->window.harm[ HNS-H : HNS+H ] / bh[ HNS-H : HNS+H ]
        
        // Stochastic Component Window
        this->window.stoc.resize(NS);
        dsp::WindowingFunction<float>::fillWindowingTables( this->window.stoc.data(), this->window.stoc.size(), dsp::WindowingFunction<float>::blackmanHarris, true);
//        this->window.stoc = hann(NS)
        
//        return w
    }
    
    // TODO - Check if this method is still neded
//    std::vector<int> getBufferIndexes(int i_head, int i_tail)
    int getBufferIndexes(int i_head, int i_tail)
    {
        // If the tail pointer is ahead the head pointer
        if ( (i_tail - i_head) < 0 )
        {
            return 0;
//            return np.r_[ i_head : self.buffer.length, 0 : i_tail]
        }
        else
        {
            return 0;
//            return np.r_[ i_head : i_tail ]
        }
        
//        return selected_buffer_indexes;
    }
    
    std::vector<float> getBufferSection(Channel selected_channel, int i_head, int i_tail)
    {
        std::vector<float> buffer = this->buffer.channels[selected_channel];
        std::vector<float> buffer_section;
        
        // If the tail pointer is ahead the head pointer
        if ( (i_tail - i_head) < 0 )
        {
            std::vector<float>::const_iterator fh_head = buffer.begin() + i_head;
            std::vector<float>::const_iterator fh_tail = buffer.begin() + buffer.size();
            std::vector<float> first_half(fh_head, fh_tail);
            
            std::vector<float>::const_iterator sh_head = buffer.begin() + i_head;
            std::vector<float>::const_iterator sh_tail = buffer.begin() + i_tail;
            std::vector<float> second_half(sh_head, sh_tail);
            
            // Concatenate "first_half" and "second_half"
            buffer_section = first_half;
            buffer_section.insert( buffer_section.end(), second_half.begin(), second_half.end() );
            
//            return np.r_[ i_head : self.buffer.length, 0 : i_tail]
        }
        else
        {
            std::vector<float>::const_iterator head = buffer.begin() + i_head;
            std::vector<float>::const_iterator tail = buffer.begin() + i_tail;
            std::vector<float> buffer_section(head, tail);
            
//            return np.r_[ i_head : i_tail ]
        }
        
        return buffer_section;
    }
};
