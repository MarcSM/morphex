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
#include "Voice.h"
#include "Sound.h"

//#include "MorphVoice.h"

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
    
protected:
    
    OwnedArray<Voice> voices;
    
private:
    
//    Voices mVoice;
//    SoundArray& mSound;

    double currentSampleRate;
    
    // Instrument
    Core::Instrument instrument;
};
