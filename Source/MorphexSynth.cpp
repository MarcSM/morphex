/*
  ==============================================================================

    MorphexSynth.cpp
    Created: 21 Jul 2019 1:05:43pm
    Author:  Marc Sanchez Martinez

  ==============================================================================
*/

#include "MorphexSynth.h"

#include "SMTConstants.h"

// TODO - Remove this reference, it's only for testing

MorphexSynth::MorphexSynth(AudioProcessorValueTreeState* parameters)
{
//    JUCE::DirectoryIterator()
    std::string instrument_folder = "/Users/Marc/Research/Repos/morphex-research/data/instruments/Suitcase Dry Test";
    
    DirectoryIterator iter (File (instrument_folder), true, "*.had");
    
    while (iter.next())
    {
        File theFileItFound (iter.getFile());
        
        DBG(theFileItFound.getFullPathName());
    }
    
//    throw std::exception();
    
    // Initialize the instrument
    this->instrument = Core::Instrument();
    
    // Add some voices to our synth, to play the sounds..
    for (int i = 0; i < MAX_VOICES; i++)
    {
        // TODO - MorphVoice can be an instante of Intrument->Synthesis
        
        // Add the voice to the synth
        this->addVoice( new Voice(&this->instrument, parameters) );
//        this->addVoice( new MorphVoice(sound, parameters) );
    }
    
    // TODO - Maybe we should move the moprhing code from MorphVoice to MorphSound
    // and in MorphVoice manipulate the output audio buffer (already morphed) to
    // decorrelate the signal (or maybe add some "spread")
    
//    // Add a sound for them to play
//    this->clearSounds();
//    this->addSound( new MorphSound() );
}

MorphexSynth::~MorphexSynth() {}

void MorphexSynth::setCurrentPlaybackSampleRate (double sampleRate)
{
    currentSampleRate = sampleRate;
    
    Voice* voice;
    
    // Set new sample rate to ADSR for each voice instance
    for (int i=0; i < this->getNumVoices();i++)
    {
        if( ( voice = dynamic_cast<Voice*>( this->getVoice(i) ) ) )
        {
//            morph_voice->setADSRSampleRate(sampleRate);
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
