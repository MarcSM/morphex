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
//    mSound[1] = std::make_unique<Core::Sound>(DEFAULT_SOUND_FILE_1, DEFAULT_SOUND_FILE_1_COLLECTION_PATH);
//    mSound[2] = std::make_unique<Core::Sound>(DEFAULT_SOUND_FILE_2, DEFAULT_SOUND_FILE_2_COLLECTION_PATH);
    
    // Initialize the instrument
    this->instrument = Instrument();
    this->instrument.name = "Test";

    //    JUCE::DirectoryIterator()
    std::string instrument_folder = "/Users/Marc/Research/Repos/morphex-research/data/instruments/Morph";
    //    std::string instrument_folder = "/Users/Marc/Research/Repos/morphex-research/data/instruments/Suitcase Dry Test";

    DirectoryIterator iter (File (instrument_folder), true, "*.had");
    
//    int i = 1;
    while (iter.next())
    {
        File sound_file (iter.getFile());
    //        String sound_file_name = sound_file.getFullPathName();
        std::string sound_file_name = sound_file.getFullPathName().toStdString();
        DBG(sound_file_name);

        // TODO - Find a better approach to do this (try to avoid using copy constructor)
        Core::Sound sound = Core::Sound( sound_file_name );

    //        Core::Sound scopy = sound;
        
//        mSound[i] = std::make_unique<Core::Sound>(sound_file_name);
//
//        i++;
        
        this->instrument.note[ sound.note ]->velocity[ sound.velocity ]->sound = sound;
        this->instrument.note[ sound.note ]->velocity[ sound.velocity ]->loaded = true;
        
    //        this->instrument.note[ sound.note ]->velocity[ sound.velocity ]->setSound( sound );
    //        this->instrument.note[ sound.note ]->velocity[ sound.velocity ]->loadSound( sound_file_name );
    }
    
    // Add some voices to our synth, to play the sounds..
    for (int i = 0; i < MAX_VOICES; i++)
    {
        // Add the voice to the synth
//        this->addVoice( new Voice(this->instrument, parameters) );
        this->addVoice( new MorphVoice(this->instrument, parameters) );
//        this->addVoice( new MorphVoice(mSound, parameters) );
    }
    
    // TODO - NOTE: If we enter on release stated, there is no way back, the
    // sound goes always down, even if a new sound is selected in a pitchbend from the instrument
    // also on stop note, jump to the release section of the sound (all 4 morph sounds)
    
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

