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

#define kMeterSmoothingCoeff 0.02

namespace DSP { class Gain; }

class DSP::Gain
{
public:
    
    Gain()
    {
        mOutputSmoothed = 0;
    }
    
    ~Gain() {}
    
    void process (float* buffer, float inGain, int inNumSamplesToRender)
    {
        float gainMapped = jmap (inGain, 0.0f, 1.0f, -24.0f, 24.0f);
        gainMapped = Decibels::decibelsToGain (gainMapped, -24.0f);
        
        for (int i = 0; i < inNumSamplesToRender; i++)
        {
            buffer[i] = buffer[i] * gainMapped;
        }
        
        float absValue = fabs (buffer[0]);
        mOutputSmoothed = kMeterSmoothingCoeff * (mOutputSmoothed - absValue) + absValue;
    }
    
    float getMeterLevel()
    {
        return mOutputSmoothed;
    }
    
private:
    
    float mOutputSmoothed;
};
