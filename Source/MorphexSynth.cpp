/*
 ==============================================================================
 
 MorphexSynth.cpp
 Created: 21 Jul 2019 1:05:43pm
 Author:  Marc Sanchez Martinez
 
 ==============================================================================
 */

#include "MorphexSynth.h"

#include "SMTConstants.h"

MorphexSynth::MorphexSynth(AudioProcessorValueTreeState* parameters)
{
    mSound[1] = std::make_unique<Core::Sound>(DEFAULT_SOUND_FILE_1, DEFAULT_SOUND_FILE_1_COLLECTION_PATH);
    mSound[2] = std::make_unique<Core::Sound>(DEFAULT_SOUND_FILE_2, DEFAULT_SOUND_FILE_2_COLLECTION_PATH);
    
    // Add some voices to our synth, to play the sounds..
    for (int i = 0; i < MAX_VOICES; i++)
    {
        // Add the voice to the synth
        this->addVoice( new MorphVoice(mSound, parameters) );
    }
    
    // TODO - Maybe we should move the moprhing code from MorphVoice to MorphSound
    // and in MorphVoice manipulate the output audio buffer (already morphed) to
    // decorrelate the signal (or maybe add some "spread")
    
    // Add a sound for them to play
    this->clearSounds();
    this->addSound( new MorphSound() );
}

MorphexSynth::~MorphexSynth() {}

void MorphexSynth::setCurrentPlaybackSampleRate (double sampleRate)
{
    currentSampleRate = sampleRate;
    
    MorphVoice* morph_voice;
    
    // Set new sample rate to ADSR for each voice instance
    for (int i=0; i < this->getNumVoices();i++)
    {
        if( ( morph_voice = dynamic_cast<MorphVoice*>( this->getVoice(i) ) ) )
        {
            morph_voice->setADSRSampleRate(sampleRate);
        }
    }
    
    // Call base class method
    Synthesiser::setCurrentPlaybackSampleRate(sampleRate);
}

void MorphexSynth::renderNextBlock (AudioBuffer<float>& outputAudio,
                                    const MidiBuffer& inputMidi,
                                    int startSample, int numSamples)
{
    // Call base class method
    Synthesiser::renderNextBlock(outputAudio, inputMidi, startSample, numSamples);
}

