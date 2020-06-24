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

#include <JuceHeader.h>

namespace DSP { class Reverb; }

class DSP::Reverb
{
public:
    
    Reverb (AudioProcessorValueTreeState* parameters)
    :   mParameters (parameters)
    {
        updateParameters();
    }
    
    ~Reverb() {}
    
//    void process (float* buffer, int inNumSamplesToRender)
    void process (AudioSampleBuffer& buffer, int inNumSamplesToRender)
    {
        updateParameters();
        
        ScopedNoDenormals noDenormals;
        
        const int totalNumInputChannels  = 2;
        const int totalNumOutputChannels = 2;
        
        for (int i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        {
            buffer.clear (i, 0, inNumSamplesToRender);
        }
        
        const auto numChannels = jmin (totalNumInputChannels, totalNumOutputChannels);
        
        if (numChannels == 1) theReverb.processMono (buffer.getWritePointer (0), inNumSamplesToRender);
        else if (numChannels == 2) theReverb.processStereo (buffer.getWritePointer (0), buffer.getWritePointer (1), inNumSamplesToRender);
    }
    
    void updateParameters()
    {
        reverbParameters.wetLevel = *mParameters->getRawParameterValue (Morphex::PARAMETERS<float>[Morphex::Parameters::ReverbDryWet].ID);
        reverbParameters.dryLevel = 1 - reverbParameters.wetLevel;
        reverbParameters.roomSize = 0.8;
        reverbParameters.damping = 0.1;
        theReverb.setParameters (reverbParameters);
    }
    
private:
    
    AudioProcessorValueTreeState* mParameters;
    
    juce::Reverb theReverb;
    juce::Reverb::Parameters reverbParameters;
};
