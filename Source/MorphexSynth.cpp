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
    // Initialize the instrument
    this->instrument = Core::Instrument();
    this->instrument.name = "Test";
    
//    JUCE::DirectoryIterator()
    std::string instrument_folder = "/Users/Marc/Research/Repos/morphex-research/data/instruments/Suitcase Dry Test";
    
    DirectoryIterator iter (File (instrument_folder), true, "*.had");
    
    while (iter.next())
    {
        File sound_file (iter.getFile());
//        String sound_file_name = sound_file.getFullPathName();
        std::string sound_file_name = sound_file.getFullPathName().toStdString();
        DBG(sound_file_name);
        
        // TODO - Find a better approach to do this (try to avoid using copy constructor)
        Core::Sound sound = Core::Sound( sound_file_name );
        
//        Core::Sound scopy = sound;
        
        this->instrument.note[ sound.note ]->velocity[ sound.velocity ]->sound = sound;
        this->instrument.note[ sound.note ]->velocity[ sound.velocity ]->loaded = true;
//        this->instrument.note[ sound.note ]->velocity[ sound.velocity ]->setSound( sound );
//        this->instrument.note[ sound.note ]->velocity[ sound.velocity ]->loadSound( sound_file_name );
    }
    
//    std::vector<Velocity*> laoded = this->instrument.note[70]->getLoadedVelocities();
    
    // Add some voices to the synth
    for (int i = 0; i < MAX_VOICES; i++)
    {
        // Add a new voice to the synth
        this->addVoice( new Voice(this->instrument, parameters) );
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
    
    Voice* voice;
    
    // Set new sample rate to ADSR for each voice instance
    for (int i=0; i < this->getNumVoices();i++)
    {
        if( ( voice = dynamic_cast<Voice*>( this->getVoice(i) ) ) )
        {
            voice->setADSRSampleRate(sampleRate);
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
