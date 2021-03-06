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

#include "Synthesis.h"

namespace Core
{
    Synthesis::Synthesis (Instrument* instrument)
    :   instrument (instrument)
    {
        // Default values
        this->parameters.fft_size = FFT_SIZE;
        this->parameters.hop_size = HOP_SIZE;
        this->parameters.fs = FS;
        
        // TODO - Change this fixes values with the line below
        int fftOrder = 9; // 9 -> 2^9 = 512
        this->fft = new dsp::FFT (fftOrder);
        
        this->buffer.length = this->parameters.fft_size * NUM_FRAMES_IN_BUFFER;
        
        // Generate synthesis windows
        this->getWindow();
        
        this->reset();
    }
    
    Synthesis::~Synthesis() {};
    
    void Synthesis::reset()
    {
        this->live_values.i_current_frame = 0;
        this->live_values.i_samples_ready = 0;
        this->live_values.first_frame = true;
        this->live_values.last_frame = false;
        
        std::vector<float> last_freqs;
        std::vector<float> phases;
        
        // Harmonic live values
        this->live_values.harmonic.last_freqs.resize (MAX_HARMONICS);
        this->live_values.harmonic.phases.resize (MAX_HARMONICS);
        
        std::fill (this->live_values.harmonic.last_freqs.begin(),
                   this->live_values.harmonic.last_freqs.end(), 0.0);
        
        std::fill (this->live_values.harmonic.phases.begin(),
                   this->live_values.harmonic.phases.end(), 0.0);
        
        // Sinusoidal live values
        this->live_values.sinusoidal.last_freqs.resize (MAX_SINUSOIDS);
        this->live_values.sinusoidal.phases.resize (MAX_SINUSOIDS);
        
        std::fill (this->live_values.sinusoidal.last_freqs.begin(),
                   this->live_values.sinusoidal.last_freqs.end(), 0.0);
        
        std::fill (this->live_values.sinusoidal.phases.begin(),
                   this->live_values.sinusoidal.phases.end(), 0.0);
        
        this->buffer.pointers.write = 0;
        this->buffer.pointers.play = 0;
        
        // Clean buffer channels
        for (int i = 0; i < Channel::NUM_CHANNELS; i++)
        {
            this->buffer.channels[i].resize (this->buffer.length);
            std::fill (this->buffer.channels[i].begin(),
                       this->buffer.channels[i].end(), 0.0);
        }
    }
    
    std::vector<float> Synthesis::synthesizeSoundFrame (Sound::Frame sound_frame)
    {
        // Parameters shortcut
        const int FS = this->parameters.fs;
        const int NS = this->parameters.fft_size;
        
        // Output
        std::vector<float> yw (NS, 0.0);
        
        // Harmonic component
        if (this->instrument->generate.harmonic and sound_frame.hasHarmonic())
        {
            // TODO - TOFIX - This does not work
            std::vector<float> y_harmonic =
            generateSines (sound_frame.harmonic.freqs,
                           sound_frame.harmonic.mags,
                           sound_frame.harmonic.phases,
                           NS, FS);
            
            // Applying the window and saving the result on the output vector "harmonic"
            std::vector<float> yw_harmonic (NS);
            for (int i = 0; i < NS; i++)
            {
                // TODO - Is this faster in two separated for loops?
                yw_harmonic[i] = y_harmonic[i] * this->window.harm[i];
                yw[i] += yw_harmonic[i];
            }
        }

        // Sinusoidal component
        if (this->instrument->generate.sinusoidal and sound_frame.hasSinusoidal())
        {
            std::vector<float> y_sinusoidal =
            generateSines (sound_frame.sinusoidal.freqs,
                           sound_frame.sinusoidal.mags,
                           sound_frame.sinusoidal.phases,
                           NS, FS);
            
            // Applying the window and saving the result on the output vector "harmonic"
            std::vector<float> yw_sinusoidal (NS);
            for (int i = 0; i < NS; i++)
            {
                // TODO - Is this faster in two separated for loops?
                yw_sinusoidal[i] = y_sinusoidal[i] * this->window.harm[i];
                yw[i] += yw_sinusoidal[i];
            }
        }
        
        // TODO - Stochastic component
//        if ( sound_frame.hasStochastic() )
//        {
//            std::vector<float> y_stocs = generateStocs(sound_frame.stochastic, H * 2, NS);
//            
//            // Applying the window and saving the result on the output vector "harmonic"
//            std::vector<float> yw_stocs(NS);
//            for (int i = 0; i < NS; i++)
//            {
//                // TODO - Is this faster in two separated for loops?
//                yw_stocs[i] = y_stocs[i] * this->window.stoc[i];
//                yw[i] += yw_stocs[i];
//                //                yw_stocs[i] += y_stocs[i] * this->window.stoc[i];
//            }
//        }
        
        // Attack compontent
        if (this->instrument->generate.attack and sound_frame.hasAttack())
        {
            for (int i = 0; i < sound_frame.attack.size(); i++)
            {
                yw[i] += sound_frame.attack[i];
            }
        }
        
        // Residual compontent
        if (this->instrument->generate.residual and sound_frame.hasResidual())
        {
            for (int i = 0; i < sound_frame.residual.size(); i++)
            {
                yw[i] += sound_frame.residual[i];
            }
        }
        
        return yw;
    }
    
    void Synthesis::generateSoundFrame (Sound::Frame sound_frame, bool append_to_generated)
    {
        // Get parameters
        const int H = this->parameters.hop_size;
        const int MAX_HARMONICS = sound_frame.getMaxHarmonics();
        const int MAX_SINUSOIDS = sound_frame.getMaxSinusoids();

        // A list with the indexes of the harmonics we want to interpolate
        std::vector<int> idx_harmonics = Tools::Generate::range (0, MAX_HARMONICS);
        std::vector<int> idx_sinusoids = Tools::Generate::range (0, MAX_SINUSOIDS);
        
        // If the sound frame has harmonics but not its phases
        if (sound_frame.hasHarmonic() && !sound_frame.hasPhases (sound_frame.harmonic))
        {
            // Get phase values
            sound_frame.harmonic.phases = Tools::Get::valuesByIndex (this->live_values.harmonic.phases, idx_harmonics);

            // Save the current frequencies to be available fot the next iteration
            updateLastFreqs (this->live_values.harmonic, sound_frame.harmonic.freqs, idx_harmonics);

            // Update phases for the next iteration
            updatePhases (this->live_values.harmonic, sound_frame.harmonic.freqs, idx_harmonics, H);
        }

        // If the sound frame has sinusoids but not its phases
        if (sound_frame.hasSinusoidal() && !sound_frame.hasPhases (sound_frame.sinusoidal))
        {
            // Get phase values
            sound_frame.sinusoidal.phases = Tools::Get::valuesByIndex (this->live_values.sinusoidal.phases, idx_sinusoids);

            // Save the current frequencies to be available fot the next iteration
            updateLastFreqs (this->live_values.sinusoidal, sound_frame.sinusoidal.freqs, idx_sinusoids);

            // Update phases for the next iteration
            updatePhases (this->live_values.sinusoidal, sound_frame.sinusoidal.freqs, idx_sinusoids, H);
        }
        
        // Generate windowed audio frame
        std::vector<float> windowed_audio_frame = synthesizeSoundFrame (sound_frame);
        
        if (this->live_values.first_frame)
        {
            // Apply fade in
            Tools::Audio::applyFadeIn (windowed_audio_frame);
            this->live_values.first_frame = false;
        }
            
        if (this->live_values.last_frame)
        {
            // Apply fade out
            Tools::Audio::applyFadeOut (windowed_audio_frame);
            this->live_values.last_frame = false;
        }
        
        // Add the audio frame to the circular buffer
        updateBuffer (BufferSection::Write, BufferUpdateMode::Add, windowed_audio_frame, Channel::Mono);
        
        // Update samples ready to be played
        this->live_values.i_samples_ready += H;
        
        // Clean the part of the buffer that we must override
        updateBuffer (BufferSection::Clean, BufferUpdateMode::Delete);
        
        // Update write pointer position
        this->updateWritePointer (H);
    }
    
    void Synthesis::getWindow()
    {
        // Get parameters
        const int NS = this->parameters.fft_size;
        const int H = this->parameters.hop_size;
        const int HNS = int (NS / 2);
        
        std::vector<float> ow (2 * H);
        dsp::WindowingFunction<float>::fillWindowingTables (ow.data(), ow.size(), dsp::WindowingFunction<float>::triangular, false);
        
        std::vector<float> bh (NS);
        dsp::WindowingFunction<float>::fillWindowingTables (bh.data(), bh.size(), dsp::WindowingFunction<float>::blackmanHarris, false);
        float bh_sum = std::accumulate (bh.begin(), bh.end(), 0.0);
        Tools::Calculate::divideByScalar (bh, bh_sum);
        
        // Harmonic window
        this->window.harm.resize (NS);
        std::fill (this->window.harm.begin(), this->window.harm.end(), 0.0);
        
        int j = 0;
        for (int i = HNS-H; i < HNS+H; i++)
        {
            this->window.harm[i] = ow[j] / bh[i];
            j ++;
        }
        
        // Stochastic window
        this->window.stoc.resize (NS);
        dsp::WindowingFunction<float>::fillWindowingTables (this->window.stoc.data(), this->window.stoc.size(), dsp::WindowingFunction<float>::blackmanHarris, true);
    }
    
    // TODO - Check limits of "i_pointer_position" when it's "i_frame_length"
    // if "i_pointer_position" == "this->buffer.length" modulo may return 0, which may
    // not be expected if the the range asked to the buffer is equal to "i_frame_length"
    // we will retrieve 0 samples from the buffer instead of "i_frame_length" samples
    int Synthesis::getPointerInLimits (int i_pointer_position)
    {
        return Tools::Calculate::modulo (i_pointer_position, this->buffer.length);
    }
    
    std::vector<int> Synthesis::getBufferIndexes (int i_head, int i_tail)
    {
        // Output
        std::vector<int> buffer_indexes;
        
        // If the tail pointer is ahead the head pointer
        if ( (i_tail - i_head) < 0 )
        {
            std::vector<int> first_half = Tools::Generate::range (i_head, this->buffer.length);
            std::vector<int> second_half = Tools::Generate::range (0, i_tail);
            
            // Concatenate "first_half" and "second_half"
            buffer_indexes = first_half;
            buffer_indexes.insert (buffer_indexes.end(), second_half.begin(), second_half.end());
        }
        else
        {
            buffer_indexes = Tools::Generate::range (i_head, i_tail);
        }
        
        return buffer_indexes;
    }
    
    std::vector<int> Synthesis::getBufferSectionIndexes (BufferSection buffer_section, int i_frame_length)
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
                buffer_indexes = getBufferIndexes (this->buffer.pointers.write,
                                                   this->buffer.pointers.clean (this));
                break;
            case BufferSection::Clean:
                buffer_indexes = getBufferIndexes (this->buffer.pointers.clean (this),
                                                   this->buffer.pointers.clean (this, H));
                break;
            case BufferSection::Play:
                buffer_indexes = getBufferIndexes (this->buffer.pointers.play,
                                                   this->getPointerInLimits (this->buffer.pointers.play + i_frame_length));
                break;
            default:
                // Return an empty vector
                return std::vector<int>();
        }
        
        return buffer_indexes;
    }
    
    std::vector<float> Synthesis::getBuffer (BufferSection buffer_section, Channel selected_channel, int i_frame_length)
    {
        // TODO - Check argument: if int i_frame_length == this->parameters.fft_size
        
        // Output
        std::vector<float> buffer_section_samples;
        
        std::vector<int> buffer_indexes = getBufferSectionIndexes (buffer_section, i_frame_length);
        
        // For each index
        for (int i = 0; i < buffer_indexes.size(); i++)
        {
            buffer_section_samples.push_back (this->buffer.channels[selected_channel][ buffer_indexes[i] ]);
        }
        
        return buffer_section_samples;
    }
    
    void Synthesis::updateWritePointer (int i_pointer_increment)
    {
        int new_pointer_position = this->buffer.pointers.write + i_pointer_increment;
        this->buffer.pointers.write = this->getPointerInLimits (new_pointer_position);
    }
    
    void Synthesis::updatePlayPointer (int i_pointer_increment)
    {
        int new_pointer_position = this->buffer.pointers.play + i_pointer_increment;
        this->buffer.pointers.play = this->getPointerInLimits (new_pointer_position);
    }
    
    void Synthesis::updateBuffer (BufferSection buffer_section, BufferUpdateMode update_mode, std::vector<float> given_frame, Channel selected_channel)
    {
        std::vector<int> buffer_indexes = getBufferSectionIndexes (buffer_section);
        
        // For each index
        for (int i = 0; i < buffer_indexes.size(); i++)
        {
            float new_value = 0.0;
            if (i < given_frame.size())
            {
                new_value = given_frame[i];
            }
            else
            {
                if (update_mode != BufferUpdateMode::Delete)
                {
                    DBG("We should not reach this point");
//                    jassertfalse;
                }
            }
            
            switch (update_mode)
            {
                case BufferUpdateMode::Set:
                    this->buffer.channels[selected_channel][ buffer_indexes[i] ] = new_value;
                    break;
                case BufferUpdateMode::Add:
                    this->buffer.channels[selected_channel][ buffer_indexes[i] ] += new_value;
                    break;
                case BufferUpdateMode::Delete:
                    this->buffer.channels[selected_channel][ buffer_indexes[i] ] = 0.0;
                    break;
            }
        }
    }
    
    void Synthesis::updatePhases (SynthesisLiveValues::FP& fp_values,
                                  std::vector<float> freqs,
                                  std::vector<int> idx_freqs,
                                  int hop_size,
                                  bool append_to_generated)
    {
        int idx_freq = 0;
        
        // For each selected frequency
        for (int i = 0; i < idx_freqs.size(); i++)
        {
            idx_freq = idx_freqs[i];
            
            // Update phase value
            fp_values.phases[idx_freq] +=
            (M_PI * (fp_values.last_freqs[idx_freq] + freqs[idx_freq]) / this->parameters.fs) * hop_size;
            
            // Keep phase inside 2 * pi
            fp_values.phases[idx_freq] = std::fmod (fp_values.phases[idx_freq], (2.0 * M_PI));
        }
    }
    
    // TODO - Evaluate if "idx_harmonics" will be necessary in the near future
    void Synthesis::updateLastFreqs (SynthesisLiveValues::FP& fp_values, std::vector<float> freqs, std::vector<int> idx_freqs)
    {
        int idx_freq = 0;
        
        // For each selected harmonic
        for (int i = 0; i < idx_freqs.size(); i++)
        {
            idx_freq = idx_freqs[i];
            
            // Update last freq value
            fp_values.last_freqs[idx_freq] = freqs[idx_freq];
        }
    }
    
    std::vector<float> Synthesis::generateSines (std::vector<float> iploc, std::vector<float> ipmag, std::vector<float> ipphase, int NS, int fs)
    {
        for (int i=0; i<iploc.size(); i++) iploc[i] = NS * iploc[i] / (float) fs;
        
        std::vector<float> real (NS);
        std::vector<float> imag (NS);
        
        genspecsines_C (iploc.data(), ipmag.data(), ipphase.data(), (int) iploc.size(), real.data(), imag.data(), NS);
        
        std::vector<std::complex<float>> generated_sines (NS);
        
        std::transform (real.begin(), real.end(), imag.begin(), generated_sines.begin(), [] (float dreal, float dimag)
        {
            return std::complex<float>( dreal, dimag );
            
        });
        
        // Perform the IFFT
        dsp::Complex<float>* Y_harmonics = new dsp::Complex<float>[NS];
        fft->perform (generated_sines.data(), Y_harmonics, true);
        
        // Get the real part, we don't get the second half because it's empty
        std::vector<float> y_harmonics (NS);
        for (int i = 0; i < NS; i++) y_harmonics[i] = Y_harmonics[i].real();
        
        // TODO - Avoid using deletes
        delete[] Y_harmonics;
        
        // Perform an FFT shift
        Tools::Audio::fftShift (y_harmonics);

        return y_harmonics;
    }
    
    std::vector<float> Synthesis::generateStocs (std::vector<float> stocs_morph, int H, int NS)
    {
        // Interpolate to original size
        std::vector<float> mY = stocs_morph;
        
        // Generate phase random values
        int hN = int (NS / 2 + 1); // positive size of fft
        
        std::vector<float> pY_aux (hN);
        std::generate (pY_aux.begin(), pY_aux.end(), Tools::Generate::random (0.0, 1.0));
        
        for (int i=0; i<pY_aux.size(); i++) pY_aux[i] = pY_aux[i] * 2 * M_PI;
        
        std::vector<std::complex<float>> pY (hN);
        for (int i=0; i<pY.size(); i++) pY[i] = std::complex<float> (pY_aux[i]);
        
        // Initialize synthesis spectrum
        std::vector<std::complex<float>> Y (NS, 0.0);
        
        for (int i=0; i<hN; i++)
        {
            // Generate positive freq
            std::complex<float> aux_comp = std::complex<float> (0, -1) * std::complex<float> (pY[i]);
            std::complex<float> aux_exp_comp = std::exp<float> (aux_comp);
            
            pY[i] = std::powf (10, mY[i] / 20.0) * aux_exp_comp;
        }
        
        for (int i=hN; i<Y.size(); i++)
        {
            // Generate negative freq
            std::complex<float> aux_comp = std::complex<float> (0, -1) * std::complex<float> (pY[i]);
            std::complex<float> aux_exp_comp = std::exp<float> (aux_comp);
            
            pY[i] = std::powf (10, mY[i] / 20.0) * aux_exp_comp;
        }
        
        // Perform the IFFT
        dsp::Complex<float>* Y_stocs = new dsp::Complex<float>[NS];
        fft->perform (pY.data(), Y_stocs, true);
        
        // Get the real part, we don't get the second half because it's empty
        std::vector<float> y_stocs (NS);
        for (int i = 0; i < NS; i++) y_stocs[i] = Y_stocs[i].real();
        
        // TODO - Avoid using deletes
        delete[] Y_stocs;
        
        // Perform an FFT shift
        Tools::Audio::fftShift (y_stocs);

        return y_stocs;
    }
    
    void Synthesis::genspecsines_C (float *iploc, float *ipmag, float *ipphase, int n_peaks, float *real, float*imag, int size_spec)
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
                        // TODO - Is this faster in two separated for loops?
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
}
