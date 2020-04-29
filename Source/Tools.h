/*
  ==============================================================================

    Tools.h
    Created: 22 Apr 2020 3:53:32pm
    Author:  Marc Sanchez Martinez

  ==============================================================================
*/

#pragma once

#include <math.h>

#include <vector>
#include <numeric>

namespace Core::Tools
{
    namespace Generate
    {
        /** Generate a vector within a range */
        inline std::vector<int> range(int min, int max)
        {
            int result_length = max-min;
            std::vector<int> result(result_length);
            std::iota(result.begin(), result.end(), min);
            
            return result;
        }
        
        /** Generate a vector within a range */
        inline std::vector<float> range(float min, float max, float increment = 1.0, bool ensure_max = false)
        {
            // Output
            std::vector<float> result;
            
            for (float i = min; i <= max; i += increment)
            {
                result.push_back(i);
            }
            
            // Ensure "max" value is on the vector
            if (ensure_max and result.back() != max) result.push_back(max);
            
            return result;
        }
        
        struct random
        {
            double minValue;
            double maxValue;
            random(double min, double max): minValue(min), maxValue(max) {}
            
            double operator()()
            {
                return (maxValue - minValue) * ( (double)std::rand() / (double)RAND_MAX ) + minValue;
            }
        };
    }
    
    namespace Audio
    {
        inline void applyFadeIn(std::vector<float>& given_vector, int fade_in_length)
        {
            int given_vector_length = (int)given_vector.size();
            
            // The given vector must be at least 3 elements long
            if (given_vector_length > 2 and given_vector_length >= fade_in_length)
            {
                float increment = fade_in_length / 1.0;
                
                std::vector<float> fade_in = Core::Tools::Generate::range(0.0, 1.0, increment);
                
                for (int i = 0; i < fade_in.size(); i++)
                {
                    given_vector[i] = given_vector[i] * fade_in[i];
                }
            }
        }
        
        inline void applyFadeOut(std::vector<float>& given_vector, int fade_out_length)
        {
            int given_vector_length = (int)given_vector.size();
            
            // The given vector must be at least 3 elements long
            if (given_vector_length > 2 and given_vector_length >= fade_out_length)
            {
                float increment = fade_out_length / 1.0;
                
                std::vector<float> fade_out = Tools::Generate::range(0.0, 1.0, increment);
                
                for (int i = given_vector_length; i < given_vector_length - fade_out_length; i--)
                {
                    given_vector[i] = given_vector[i] * fade_out[i];
                }
            }
        }
        
        /** FFT shift for double data */
        inline void fftShift(std::vector<float> data, int data_size)
        {
            // Even number of elements
            if (data_size % 2)
            {
                std::rotate(&data[0], &data[data_size >> 1], &data[data_size]);
            }
            // Uneven number of elements
            else
            {
                std::rotate(&data[0], &data[(data_size >> 1) + 1], &data[data_size]);
            }
        }
    }
    
    namespace Midi
    {
        inline int toFreq(const int f_note, const float f_frequency_of_A = 440.0) {
            return (f_frequency_of_A / 32.0) * powf( 2.0, ((f_note - 9.0) / 12.0) );
//            return (f_frequency_of_A / 32.0) * (2.0 ** ((f_note - 9.0) / 12.0));
        }
    }
    
    namespace Get
    {
        template<typename T>
        inline std::vector<T> valuesByIndex(std::vector<T> given_vector, std::vector<int> idxs)
        {
            // Output
            std::vector<T> result;
            
            for (float i = 0; i <= idxs.size(); i++)
            {
                result.push_back( given_vector[i] );
            }
            
            return result;
        }
        
        template<typename T>
        inline std::vector<T> valuesInRange(std::vector<T> given_vector, int min, int max)
        {
            // Output
            std::vector<T> result;
            
            for (float i = min; i <= max; i++)
            {
                result.push_back( given_vector[i] );
            }
            
            return result;
        }
    }
    
    namespace Calculate
    {
        inline int modulo(int n, int M) {
            return ((n % M) + M) % M;
        }
        
        template<typename T, typename TS>
        inline void divideByScalar(std::vector<T> &given_vector, TS scalar) {
            
            for (float i = 0; i <= given_vector.size(); i++)
            {
                given_vector[i] /= scalar;
            }
        }
    }
}
