/*
  ==============================================================================

    MorphexSynth.h
    Created: 21 Jul 2019 1:05:43pm
    Author:  Marc Sanchez Martinez

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"

#include "Sound.h"

#include "MorphVoice.h"

class MorphexSynth
:   public Synthesiser
{
public:
    
    MorphexSynth(SoundArray& sound, AudioProcessorValueTreeState* parameters);
    ~MorphexSynth();
    
    void setCurrentPlaybackSampleRate (double sampleRate) override;
    void renderNextBlock (AudioBuffer<float>& outputAudio,
                          const MidiBuffer& inputMidi,
                          int startSample, int numSamples);
    
    void setup();
    
protected:
    
    OwnedArray<MorphVoice> voices;
    
private:
    
    SoundArray& mSound;
    
    double currentSampleRate;
};
