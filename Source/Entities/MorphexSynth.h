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

#include "../Core/Instrument.h"
#include "../Core/Sound.h"
#include "../Core/Voice.h"

#include "../DSP/Reverb.h"
#include "../DSP/Gain.h"

using namespace Core;

class MorphexSynth
:   public Synthesiser
{
public:
    
    MorphexSynth (AudioProcessorValueTreeState* parameters);
    ~MorphexSynth();
    
    void setCurrentPlaybackSampleRate (double sampleRate) override;
    void renderNextBlock (AudioBuffer<float>& outputAudio,
                          const MidiBuffer& inputMidi,
                          int startSample, int numSamples);
    
    void reset();
    void initializeDSP();

    Instrument instrument;
    
protected:
    
    OwnedArray<Voice> voices;
    
private:

    AudioProcessorValueTreeState* mParameters;
    
    std::unique_ptr<DSP::Reverb> mReverb;
    std::unique_ptr<DSP::Gain> mOutputGain[NUM_CHANNELS];

    double currentSampleRate;
};

