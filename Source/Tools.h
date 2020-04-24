/*
  ==============================================================================

    Tools.h
    Created: 22 Apr 2020 3:53:32pm
    Author:  Marc Sanchez Martinez

  ==============================================================================
*/

#pragma once

namespace Tools
{
    namespace Get
    {
        inline std::vector<float> valuesByIndex(std::vector<float> given_vector, std::vector<int> idxs)
        {
            // TODO - Generic Type instead of float
            std::vector<float> test(0);
            return test;
        }
    }
    
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
    }
}
