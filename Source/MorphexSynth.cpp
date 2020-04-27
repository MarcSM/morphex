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
#include <filesystem>

MorphexSynth::MorphexSynth(AudioProcessorValueTreeState* parameters)
{
//    JUCE::DirectoryIterator()
    char* instrument_folder = "/Users/Marc/Research/Repos/morphex-research/data/instruments/Suitcase Dry Test";
    

    std::string path = "/path/to/directory";
    for (const auto & entry : std::filesystem::directory_iterator(path))
        std::cout << entry.path() << std::endl;
    
//    DIR *dir;
//    struct dirent *ent;
//
//    if ((dir = opendir (instrument_folder)) != NULL) {
//
////        if file_name.endswith(".had"):
//        {
//            had_file_path = os.path.join(instrument_folder, file_name)
//            sound = Sound( had_file_path )
//            sound.loadHadFile()
//            test_instrument.note[ sound.note ].velocity[ sound.velocity ].loadSound( sound )
//        }
//
//        /* print all the files and directories within directory */
//        while ((ent = readdir (dir)) != NULL) {
//            printf ("%s\n", ent->d_name);
//        }
//        closedir (dir);
//    } else {
//        /* could not open directory */
//        perror ("");
//    }
    
    throw std::exception();
    
    // Add some voices to our synth, to play the sounds..
    for (int i = 0; i < MAX_VOICES; i++)
    {
        // TODO - MorphVoice can be an instante of Intrument->Synthesis
        
        // Add the voice to the synth
        this->addVoice( new MorphVoice(sound, parameters) );
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
