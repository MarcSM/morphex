/*
  ==============================================================================

    Tools.h
    Created: 22 Apr 2020 3:53:32pm
    Author:  Marc Sanchez Martinez

  ==============================================================================
*/

#pragma once

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
    }
    
    namespace Audio
    {
        inline static std::vector<float> applyFadeIn(std::vector<float>& given_vector, int fade_in_length)
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
        
        inline std::vector<float> applyFadeOut(std::vector<float>& given_vector, int fade_out_length)
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
                
                // TODO - Generic Type instead of float
                std::vector<float> test(0);
                return test;
            }
        }
        
        // Fade Out
//        reverse(v.begin() + 5, v.begin() + 8);
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
    }
    
    namespace Calculate
    {
        inline int modulo(int n, int M) {
            return ((n % M) + M) % M;
        }
    }
}
