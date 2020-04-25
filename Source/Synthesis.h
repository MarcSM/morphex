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
    };
    
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
        bool first_frame;
        bool last_frame;
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

    struct SynthesisBuffer
    {
        std::vector<float> channels[Channel::NUM_CHANNELS];
        
        int length;
        
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
    
    dsp::FFT* fft;
    
    Synthesis()
    {
        // Default values
        const int NS = this->parameters.fft_size = FFT_SIZE;
        const int H = this->parameters.hop_size = HOP_SIZE;
        
        // TODO - Change this fixes values with the line below
        int fftOrder = 9; // 9 -> 2^9 = 512
        this->fft = new dsp::FFT(fftOrder);
        
        this->buffer.length = NS * NUM_FRAMES_IN_BUFFER;
        
        // Initialize buffer channels
        for (int i = 0; i < Channel::NUM_CHANNELS; i++)
        {
            this->buffer.channels[i].resize(NS);
            std::fill(this->buffer.channels[i].begin(),
                      this->buffer.channels[i].end(), 0.0);
        }
    }
    
    void reset()
    {
        this->live_values.i_current_frame = 0;
        this->live_values.first_frame = true;
        this->live_values.last_frame = false;

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
        // TODO - // Instead of Sound, define SoundFrame, Model, or Model frame
        
        // Parameters shortcut
        const int NS = this->parameters.fft_size;
        const int H = this->parameters.hop_size;
        
        // Output
        std::vector<float> yw(NS);
        
        // A list with the indexes of the harmonics we want to interpolate
        std::vector<int> idx_harmonics = Tools::Generate::range(0, sound.max_harmonics);
//        std::vector<int> idx_harmonics_to_interpolate = Tools::Generate::range(0, sound.max_harmonics);
//        l_harmonics = np.arange( sound.max_harmonics )
        
        // Instead of Sound, define Sound Frame, Model, or Model frame
        std::vector<float> harmonics_freqs;
        std::vector<float> harmonics_mags;
        std::vector<float> stochastic;

        std::vector<float> harmonics_freqs_to_interpolate = Tools::Get::valuesByIndex(harmonics_freqs, idx_harmonics);
        std::vector<float> harmonics_mags_to_interpolate = Tools::Get::valuesByIndex(harmonics_mags, idx_harmonics);
        std::vector<float> harmonics_phases = Tools::Get::valuesByIndex(this->live_values.phases, idx_harmonics);

//        selected_play_samples.insert(selected_play_samples.end(), section_to_play.begin(), section_to_play.end());
        
        // Generate sines
        std::vector<float> y_harmonics = generateSines(harmonics_freqs_to_interpolate,
                                                       harmonics_mags_to_interpolate,
                                                       harmonics_phases,
                                                       NS, this->parameters.fs);
        
        // Applying the window and saving the result on the output vector "harmonic"
        std::vector<float> yw_harmonics(NS);
        for (int i = 0; i < NS; i++)
        {
            yw_harmonics[i] = y_harmonics[i] * this->window.harm[i];
            yw[i] = yw_harmonics[i];
        }

        // Stochastic component
        if (sound.model->stochastic)
        {
            std::vector<float> y_stocs = generateStocs(stochastic, H * 2, NS);
            
            // Applying the window and saving the result on the output vector "harmonic"
            std::vector<float> yw_stocs(NS);
            for (int i = 0; i < NS; i++)
            {
                yw_stocs[i] = y_stocs[i] * this->window.stoc[i];
                yw[i] += yw_stocs[i];
//                yw_stocs[i] += y_stocs[i] * this->window.stoc[i];
            }
        }
        
        return yw;
    }
    
    std::vector<float> generateSoundFrame(Sound sound, int i_frame_length, bool append_to_generated = false)
    {
        // TODO - // Instead of Sound, define SoundFrame, Model, or Model frame
        
        // Parameters shortcut
        const int NS = this->parameters.fft_size;
        const int H = this->parameters.hop_size;
        
        // A list with the indexes of the harmonics we want to interpolate
        std::vector<int> idx_harmonics = Tools::Generate::range(0, sound.max_harmonics);
        
        // Update phases
        if (!this->live_values.first_frame) updatePhases(sound.model->values.harmonics_freqs[0], idx_harmonics, H);
        
        // Generate windowed audio frame
        std::vector<float> windowed_audio_frame = synthesizeAudioFrame(sound);
        
        // Save the current frequencies to be available fot the next iteration
        updateLastFreqs(sound.model->values.harmonics_freqs[0], idx_harmonics);
        
        // Add the audio frame to the circular buffer
        updateBuffer(BufferSection::Write, BufferUpdateMode::Add, windowed_audio_frame, Channel::Mono);
        
        // Selecting the processed samples
        std::vector<float> next_frame = getBuffer(BufferSection::Play, Channel::Mono, i_frame_length);
        
        next_frame = this->buffer.channels[LEFT_CH][this->buffer.selected.to_play]

        // Update samples ready
        this->live_values.i_samples_ready -= len(next_frame)

        return next_frame
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
    
    void updatePhases(std::vector<float> harmonics_freqs, std::vector<int> idx_harmonics, int hop_size, bool append_to_generated = false)
    {
        // For each selected harmonic
        for (int i = 0; i < idx_harmonics.size(); i++)
        {
            // Update phase value
            this->live_values.phases[i] +=
            ( M_PI * ( this->live_values.phases[i] + harmonics_freqs[i] ) / this->parameters.fs ) * hop_size;
            
            // Keep phase inside 2 * pi
            this->live_values.phases[i] = Tools::Calculate::modulo( this->live_values.phases[i], ( 2 * M_PI ) );
            
            // Append to generated phases
            if (append_to_generated) this->generated.harmonics_phases.push_back( this->live_values.phases[i] );
        }
    }
    
    void updateLastFreqs(std::vector<float> harmonics_freqs, std::vector<int> idx_harmonics)
    {
        // For each selected harmonic
        for (int i = 0; i < idx_harmonics.size(); i++)
        {
            // Update last freq value
            this->live_values.last_freqs[i] = harmonics_freqs[i];
        }
    }
    
    /** FFT shift for double data */
    inline void fftShift(std::vector<float> data, int data_size)
    {
        // even number of elements
        if (data_size % 2)
            std::rotate(&data[0], &data[data_size >> 1], &data[data_size]);
        // even number of elements
        else
            std::rotate(&data[0], &data[(data_size >> 1) + 1], &data[data_size]);
    }
    
//    /** FFT shift for double data */
//    inline void fftShift(float* data, int data_size)
//    {
//        // even number of elements
//        if (data_size % 2)
//            std::rotate(&data[0], &data[data_size >> 1], &data[data_size]);
//        // even number of elements
//        else
//            std::rotate(&data[0], &data[(data_size >> 1) + 1], &data[data_size]);
//    }
    
    
    std::vector<int> getBufferIndexes(int i_head, int i_tail)
    {
        // Output
        std::vector<int> buffer_indexes;
        
        // If the tail pointer is ahead the head pointer
        if ( (i_tail - i_head) < 0 )
        {
            std::vector<int> first_half = Tools::Generate::range(i_head, this->buffer.length);
            std::vector<int> second_half = Tools::Generate::range(0, i_tail);
            
            // Concatenate "first_half" and "second_half"
            buffer_section = first_half;
            buffer_section.insert( buffer_section.end(), second_half.begin(), second_half.end() );
        }
        else
        {
            buffer_indexes = Tools::Generate::range(i_head, i_tail);
        }
        
        return buffer_indexes;
    }
    
    std::vector<int> getBufferSectionIndexes(BufferSection buffer_section, int i_frame_length = 0)
    {
        // Parameters shortcut
        const int NS = this->parameters.fft_size;
        const int H = this->parameters.hop_size;
        
        if (i_frame_length == 0) i_frame_length = NS;
        
        // Output
        std::vector<int> buffer_indexes;
        
        switch (buffer_section)
        {
            case BufferSection::Write:
                buffer_indexes = getBufferIndexes(this->buffer.pointers.write,
                                                  this->buffer.pointers.clean());
                break;
            case BufferSection::Clean:
                buffer_indexes = getBufferIndexes(this->buffer.pointers.clean(),
                                                  this->buffer.pointers.clean() + H);
                break;
            case BufferSection::Play:
                buffer_indexes = getBufferIndexes(this->buffer.pointers.play(),
                                                  this->buffer.pointers.play() + i_frame_length);
                break;
            default:
                // Return an empty vector
                return std::vector<int>();
        }
    }
    
    std::vector<float> getBuffer(BufferSection buffer_section, Channel selected_channel = Channel::Mono, int i_frame_length = this->parameters.fft_size)
    {
        // Parameters shortcut
        const int NS = this->parameters.fft_size;
        const int H = this->parameters.hop_size;
        
        // Output
        std::vector<float> buffer_section;
        
        std::vector<int> buffer_indexes = getBufferSectionIndexes(buffer_section, i_frame_length);
        
        // For each index
        for (int i = 0; i < buffer_indexes.size(); i++)
        {
            buffer_section.push_back( this->buffer.channels[selected_channel][i] )
        }
        
        return buffer_section;
    }
    
    void updateBuffer(BufferSection buffer_section, BufferUpdateMode update_mode, std::vector<float> given_frame, Channel selected_channel = Channel::Mono)
    {
//        std::vector<float>& buffer = *this->buffer.channels[selected_channel];
        
        std::vector<int> buffer_indexes = getBufferSectionIndexes(buffer_section);
        
        // For each index
        for (int i = 0; i < buffer_indexes.size(); i++)
        {
            float new_value = 0.0;
            if ( i < given_frame.size() ) new_value = given_frame[i]
            
            switch (buffer_section)
            {
                case BufferUpdateMode::Add:
                    this->buffer.channels[selected_channel][i] = this->buffer.channels[selected_channel][i] + new_value[i];
                    break;
                case BufferSection::Clean:
                    this->buffer.channels[selected_channel][i] = new_value[i];
                    break;
            }
        }
    }
    
    std::vector<float> generateSines(std::vector<float> iploc, std::vector<float> ipmag, std::vector<float> ipphase, int NS, int fs)
    {
        for (int i=0; i<iploc.size(); i++) iploc[i] = NS * iploc[i] / (float)fs;
        
        std::vector<float> real(NS);
        std::vector<float> imag(NS);
        
        genspecsines_C(iploc.data(), ipmag.data(), ipphase.data(), (int)iploc.size(), real.data(), imag.data(), NS);
        
        std::vector<std::complex<float>> generated_sines(NS);
        
        std::transform( real.begin(), real.end(), imag.begin(), generated_sines.begin(), []( float dreal, float dimag )
        {
            return std::complex<float>( dreal, dimag );
        });
        
//        return generated_sines;
        
        // Perform the IFFT
        dsp::Complex<float>* Y_harmonics = new dsp::Complex<float>[NS];
        fft->perform(generated_sines.data(), Y_harmonics, true);

        // Get the real part, we don't get the second half because it's empty
        std::vector<float> y_harmonics(NS);
//        float* y_harmonics = new float[NS];
        for (int i = 0; i < NS; i++) y_harmonics[i] = Y_harmonics[i].real();
        
        // Perform an FFT shift
        fftShift(y_harmonics, NS);
        
        return y_harmonics;
    }
    
    void genspecsines_C(float *iploc, float *ipmag, float *ipphase, int n_peaks, float *real, float*imag, int size_spec)
    {
        int ii=0,jj=0, ploc_int;
        int size_spec_half = (int)floor(size_spec/2);
        float bin_remainder,loc,mag;
        
        for(ii=0;ii<n_peaks; ii++)
        {
            loc = iploc[ii];
            bin_remainder = floor(loc + 0.5)-loc;
            ploc_int = (int)floor(loc+0.5);
            
            if((loc>=5)&&(loc<size_spec_half-4))
            {
                mag = pow(10,(ipmag[ii]/20.0));
                
                for(jj=-4;jj<5;jj++)
                {
                    real[ploc_int+jj] += mag*BH_92_1001[(int)((bin_remainder+jj)*100) + BH_SIZE_BY2]*cos(ipphase[ii]);
                    imag[ploc_int+jj] += mag*BH_92_1001[(int)((bin_remainder+jj)*100) + BH_SIZE_BY2]*sin(ipphase[ii]);
                }
            }
            else if((loc>0)&&(loc<5))
            {
                mag = pow(10,(ipmag[ii]/20.0));
                
                for(jj=-4;jj<5;jj++)
                {
                    
                    if(ploc_int+jj<0)
                    {
                        real[-1*(ploc_int+jj)] += mag*BH_92_1001[(int)((bin_remainder+jj)*100) + BH_SIZE_BY2]*cos(ipphase[ii]);
                        imag[-1*(ploc_int+jj)] += -1*mag*BH_92_1001[(int)((bin_remainder+jj)*100) + BH_SIZE_BY2]*sin(ipphase[ii]);
                        
                    }
                    else if (ploc_int+jj==0)
                    {
                        real[(ploc_int+jj)] += 2*mag*BH_92_1001[(int)((bin_remainder+jj)*100) + BH_SIZE_BY2]*cos(ipphase[ii]);
                    }
                    else
                    {
                        real[(ploc_int+jj)] += mag*BH_92_1001[(int)((bin_remainder+jj)*100) + BH_SIZE_BY2]*cos(ipphase[ii]);
                        imag[ploc_int+jj] += mag*BH_92_1001[(int)((bin_remainder+jj)*100) + BH_SIZE_BY2]*sin(ipphase[ii]);
                    }
                }
            }
            else if((loc>=size_spec_half-4)&&(loc<size_spec_half-1))
            {
                mag = pow(10,(ipmag[ii]/20.0));
                
                for(jj=-4;jj<5;jj++)
                {
                    if(ploc_int+jj>size_spec_half)
                    {
                        real[size_spec-(ploc_int+jj)] += mag*BH_92_1001[(int)((bin_remainder+jj)*100) + BH_SIZE_BY2]*cos(ipphase[ii]);
                        imag[size_spec-(ploc_int+jj)] += -1*mag*BH_92_1001[(int)((bin_remainder+jj)*100) + BH_SIZE_BY2]*sin(ipphase[ii]);
                        
                    }
                    else if(ploc_int+jj==size_spec_half)
                    {
                        real[(ploc_int+jj)] += 2*mag*BH_92_1001[(int)((bin_remainder+jj)*100) + BH_SIZE_BY2]*cos(ipphase[ii]);
                        
                    }
                    else
                    {
                        real[(ploc_int+jj)] += mag*BH_92_1001[(int)((bin_remainder+jj)*100) + BH_SIZE_BY2]*cos(ipphase[ii]);
                        imag[ploc_int+jj] += mag*BH_92_1001[(int)((bin_remainder+jj)*100) + BH_SIZE_BY2]*sin(ipphase[ii]);
                    }
                }
            }
        }
        
        for(ii=1;ii<size_spec_half;ii++)
        {
            real[size_spec_half+ii]=real[size_spec_half-ii];
            imag[size_spec_half+ii]=-1*imag[size_spec_half-ii];
        }
    }
    
//    std::vector<std::complex<float>> generateStocs(std::vector<float> stocs_morph, int H, int NS)
    std::vector<float> generateStocs(std::vector<float> stocs_morph, int H, int NS)
    {
        // Interpolate to original size
        std::vector<float> mY = stocs_morph;
        
        // Generate phase random values
        int hN = int(NS/2+1); // positive size of fft
        
        std::vector<float> pY_aux(hN);
        std::generate(pY_aux.begin(), pY_aux.end(), RandomGenerator(0.0, 1.0));
        
        for (int i=0; i<pY_aux.size(); i++) pY_aux[i] = pY_aux[i] * 2 * M_PI;
        
        std::vector<std::complex<float>> pY(hN);
        for (int i=0; i<pY.size(); i++) pY[i] = std::complex<float>(pY_aux[i]);
        
        // Initialize synthesis spectrum
        std::vector<std::complex<float>> Y(NS, 0.0);
        
        for (int i=0; i<hN; i++)
        {
            // Generate positive freq
            std::complex<float> aux_comp = std::complex<float>(0, -1) * std::complex<float>(pY[i]);
            std::complex<float> aux_exp_comp = std::exp<float>(aux_comp);
            
            pY[i] = std::powf(10,mY[i]/20.0) * aux_exp_comp;
        }
        
        for (int i=hN; i<Y.size(); i++)
        {
            // Generate negative freq
            std::complex<float> aux_comp = std::complex<float>(0, -1) * std::complex<float>(pY[i]);
            std::complex<float> aux_exp_comp = std::exp<float>(aux_comp);
            
            pY[i] = std::powf(10,mY[i]/20.0) * aux_exp_comp;
        }
        
//        return pY;
        
        // Perform the IFFT
        dsp::Complex<float>* Y_stocs = new dsp::Complex<float>[NS];
        fft->perform(pY.data(), Y_stocs, true);
//        fft->perform(generated_stocs.data(), Y_stocs, true);

        // Get the real part, we don't get the second half because it's empty
        std::vector<float> y_stocs(NS);
//        float* y_stocs = new float[NS];
        for (int i = 0; i < NS; i++) y_stocs[i] = Y_stocs[i].real();
        
        // Perform an FFT shift
        fftShift(y_stocs, NS);
        
        return y_stocs;
    }
};
