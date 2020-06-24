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

#include "JuceHeader.h"

#include <math.h>
#include <vector>
#include <numeric>
#include <fstream>

namespace Core::Tools
{
    namespace Generate
    {
        /** Generate a vector within a range */
        inline std::vector<int> range (int min, int max)
        {
            int result_length = max-min;
            std::vector<int> result (result_length);
            std::iota (result.begin(), result.end(), min);
            
            return result;
        }
        
        /** Generate a vector within a range */
        inline std::vector<float> range (float min, float max, float increment = 1.0, bool ensure_max = false)
        {
            // Output
            std::vector<float> result;
            
            for (float i = min; i < max; i += increment)
            {
                result.push_back (i);
            }
            
            // Ensure "max" value is on the vector
            if (ensure_max and result.back() != max) result.push_back (max);
            
            return result;
        }
        
        struct random
        {
            double minValue;
            double maxValue;
            random (double min, double max): minValue (min), maxValue (max) {}
            
            double operator()()
            {
                return (maxValue - minValue) * ((double) std::rand() / (double) RAND_MAX) + minValue;
            }
        };
    }
    
    namespace Audio
    {
        inline void applyFadeIn (std::vector<float>& given_vector, int fade_in_length = 0)
        {
            int given_vector_length = (int) given_vector.size();
            
            if (fade_in_length == 0) fade_in_length = given_vector_length;
            
            // The given vector must be at least 3 elements long
            if (given_vector_length > 2 and given_vector_length >= fade_in_length)
            {
                float increment = 1.0 / fade_in_length;
                
                std::vector<float> fade_in = Core::Tools::Generate::range (0.0, 1.0, increment);
                
                for (int i = 0; i < fade_in.size(); i++)
                {
                    given_vector[i] = given_vector[i] * fade_in[i];
                }
            }
        }
        
        inline void applyFadeOut (std::vector<float>& given_vector, int fade_out_length = 0)
        {
            int given_vector_length = (int) given_vector.size() - 1;
            
            if (fade_out_length == 0) fade_out_length = given_vector_length;
            
            // The given vector must be at least 3 elements long
            if (given_vector_length > 2 and given_vector_length >= fade_out_length)
            {
                float increment = 1.0 / fade_out_length;
                
                std::vector<float> fade_out = Tools::Generate::range (0.0, 1.0, increment);
                
                int first_sample = given_vector_length - fade_out_length;
                
                for (int i = given_vector_length; i > first_sample; i--)
                {
                    given_vector[i] = given_vector[i] * fade_out[i];
                }
            }
        }
        
        /** FFT shift for float data */
        inline void fftShift (std::vector<float> &data)
        {
            int data_size = (int) data.size();
            int samples_to_shift = int (data_size / 2);
            
            // If even number of elements
            if (data_size % 2) samples_to_shift += 1;
            
            std::rotate (data.begin(), data.begin() + samples_to_shift, data.end());
        }
        
        /** FFT shift for double data */
        inline void fftShift_d (float* data, int data_size)
        {
            // even number of elements
            if (data_size % 2)
            {
                std::rotate (&data[0], &data[data_size >> 1], &data[data_size]);
            }
            // even number of elements
            else
            {
                std::rotate (&data[0], &data[(data_size >> 1) + 1], &data[data_size]);
            }
        }
        
        // TODO
        inline void resample (std::vector<float>& given_vector)
        {
//            double OutputsampleRate = 44100;
//
//            juce::File SrcFile = juce::File(/Input wav file path/);
//
//            juce::FileInputStream* InputStream = SrcFile.createInputStream();
//
//            if(InputStream == NULL)
//            {
//                return false;
//            }
//
//            juce::AudioFormat *WavFormat = new juce::WavAudioFormat();
//
//            juce::AudioFormatReader *WavFormatReader = WavFormat->createReaderFor (InputStream, true);
//
//            if(WavFormatReader == NULL)
//            {
//                SAFE_DELETE(InputStream);
//                return false;
//            }
//
//            juce::AudioFormatReaderSource *WavReaderSource = new juce::AudioFormatReaderSource(WavFormatReader,false);
//
//            juce::ResamplingAudioSource *Resample = new juce::ResamplingAudioSource(WavReaderSource,false);
//            Resample->setResamplingRatio(WavFormatReader->sampleRate/(double)OutputsampleRate); Resample->prepareToPlay(128,(double)OutputsampleRate);
        }
        
        // TODO
        inline void readSoundFile (std::string sound_file_path)
        {
//            File file (sound_file_path);
//            AudioFormatManager formatManager;
//            formatManager.registerBasicFormats();
//            ScopedPointer<AudioFormatReader> reader = formatManager.createReaderFor(file);
//            if (reader != 0)
//            {
//                AudioSampleBuffer buffer(reader->numChannels, reader->lengthInSamples);
//                buffer.readFromAudioReader(reader, 0, reader->lengthInSamples, 0, true, true);
//                float* firstChannelSamples = buffer.getSampleData(0, 0);
//            }
        }
        
        template <typename T>
        void write (std::ofstream& stream, const T& t)
        {
            stream.write ((const char*) &t, sizeof (T));
        }
        
//              writeWAVData(path.c_str(),          &buffer[0],         sampleDuration * NUM_OF_CHANNELS * sizeof(buffer[0]),   sampleRate,     NUM_OF_CHANNELS);
//        void  writeWAVData(const char* outFile,   SampleType* buf,    size_t bufSize,                                         int sampleRate, short channels)
        
        template <typename Word>
        std::ostream& write_word (std::ostream& outs, Word value, unsigned size = sizeof (Word))
        {
            for (; size; --size, value >>= 8)
            {
                outs.put( static_cast <char> (value & 0xFF) );
            }
            
            return outs;
        }
        
        // TODO
        template <typename SampleType>
        inline bool writeSoundFile (std::vector<SampleType> given_vector, std::string sound_file_path = "/Users/Marc/Documents/Audio Plugins/Morphex/Tests/buffer.wav")
        {
            const char* sound_file_path_c = sound_file_path.c_str();
            std::ofstream f (sound_file_path_c, std::ios::binary);
            
            int i_channels = 2;
            int i_sample_rate = 44100;
            int i_byte_rate = 176400;
//            int i_byte_rate = i_channels * i_sample_rate * sizeof(int);
            int i_frame_size = 8;
//            int i_frame_size = i_channels * sizeof(int);
            int i_bits_per_sample = 16;
//            int i_bits_per_sample = 8 * sizeof(int);

            // Write the file headers
            f << "RIFF----WAVEfmt ";                // (chunk size to be filled in later)
            write_word( f,  16,                 4 );    // no extension data
            write_word( f,  1,                  2 );    // PCM - integer samples
            write_word( f,  i_channels,         2 );    // two channels (stereo file)
            write_word( f,  i_sample_rate,      4 );    // samples per second (Hz)
            write_word( f,  i_byte_rate,        4 );  // (Sample Rate * BitsPerSample * Channels) / 8
//            write_word( f, 176400, 4 );  // (Sample Rate * BitsPerSample * Channels) / 8
            write_word( f,  i_frame_size,       2 );  // data block size (size of two integer samples, one for each channel, in bytes)
//            write_word( f,      8, 2 );  // data block size (size of two integer samples, one for each channel, in bytes)
            write_word( f,  i_bits_per_sample,  2 );  // number of bits per sample (use a multiple of 8)
//            write_word( f,     16, 2 );  // number of bits per sample (use a multiple of 8)
            
            // Write the data chunk header
            size_t data_chunk_pos = f.tellp();
            f << "data----";  // (chunk size to be filled in later)
            
            constexpr double max_amplitude = 32760;  // "volume"
            
            // Write the audio samples
            for (int n = 0; n < given_vector.size(); n++)
            {
                write_word( f, (int)(given_vector[n] * max_amplitude), 2 );
                write_word( f, (int)0, 2 );
            }
            
            // (We'll need the final file size to fix the chunk sizes above)
            size_t file_length = f.tellp();
            
            // Fix the data chunk header to contain the data size
            f.seekp( data_chunk_pos + 4 );
            write_word( f, file_length - data_chunk_pos + 8 );
            
            // Fix the file header to contain the proper RIFF chunk size, which is (file size - 8) bytes
            f.seekp( 0 + 4 );
            write_word( f, file_length - 8, 4 );
            
            return true;
        }
    }
    
    namespace Midi
    {
        inline float toFreq (const float f_note, const float f_frequency_of_A = 440.0)
        {
            return (f_frequency_of_A / 32.0) * powf (2.0, ((f_note - 9.0) / 12.0));
        }
    }
    
    namespace Get
    {
        template<typename T>
        inline std::vector<T> valuesByIndex (std::vector<T> given_vector, std::vector<int> idxs)
        {
            // Output
            std::vector<T> result;
            
            for (float i = 0; i < idxs.size(); i++)
            {
                result.push_back( given_vector[i] );
            }
            
            return result;
        }
        
        template<typename T>
        inline std::vector<T> valuesInRange (std::vector<T> given_vector, int min, int max)
        {
            // Output
            std::vector<T> result;
            
            for (float i = min; i < max; i++)
            {
                result.push_back (given_vector[i]);
            }
            
            return result;
        }
    }
    
    namespace Calculate
    {
        inline int modulo (int n, int M)
        {
            return ((n % M) + M) % M;
        }

        template<typename T, typename TS>
        inline void divideByScalar (std::vector<T> &given_vector, TS scalar)
        {
            for (float i = 0; i < given_vector.size(); i++)
            {
                given_vector[i] /= scalar;
            }
        }
        
        template<typename T, typename TS>
        inline void multiplyByScalar (std::vector<T> &given_vector, TS scalar)
        {
            for (float i = 0; i < given_vector.size(); i++)
            {
                given_vector[i] *= scalar;
            }
        }
        
        template<typename T>
        inline T dbToLinear (T db_value)
        {
            return (T) std::powf (10.0, db_value / 20.0);
        }
    }
    
    namespace Cast
    {
        template<typename IT, typename OT>
        inline void matrix (std::vector<IT> &input_vector, std::vector<OT> &output_vector)
        {
            output_vector.reserve (input_vector.size());
            for (auto&& v : input_vector) output_vector.emplace_back (std::begin(v), std::end(v));
        }
    }
}
