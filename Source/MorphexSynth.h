/*
 ==============================================================================
 
 MorphexSynth.h
 Created: 21 Jul 2019 1:05:43pm
 Author:  Marc Sanchez Martinez
 
 ==============================================================================
 */

#pragma once

#include "JuceHeader.h"

#include "Instrument.h"
#include "Sound.h"
#include "Voice.h"

#include "DSP/Reverb.h"
#include "DSP/Gain.h"

using namespace Core;

class MorphexSynth
:   public Synthesiser
{
public:
    
    MorphexSynth(AudioProcessorValueTreeState* parameters);
    ~MorphexSynth();
    
    void setCurrentPlaybackSampleRate (double sampleRate) override;
    void renderNextBlock (AudioBuffer<float>& outputAudio,
                          const MidiBuffer& inputMidi,
                          int startSample, int numSamples);
    
    void setup();
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

