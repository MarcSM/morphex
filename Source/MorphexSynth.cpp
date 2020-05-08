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
//    std::string instrument_folder = "/Users/Marc/Research/Repos/morphex-research/data/instruments/Morph";
//    std::string instrument_folder = "/Users/Marc/Research/Repos/morphex-research/data/instruments/Suitcase Dry Test";
//    std::string instrument_folder = "/Users/Marc/Documents/Audio Plugins/Morphex/Instruments/Suitcase Dry Full Velocity Test";
    //    std::string instrument_folder = "/Users/Marc/Documents/Audio Plugins/Morphex/Instruments/Suitcase Dry Full";
    
    
    std::string instrument_folder = "Suitcase Dry Full";
    std::string full_path = PLUGIN_DATA_DIRECTORY.toStdString() + directorySeparator.toStdString() + "Instruments" + directorySeparator.toStdString() + instrument_folder;

    DirectoryIterator iter (File (full_path), true, "*.had");
//    DirectoryIterator iter (File (full_path), true, "*.had");
    
//    static const String PLUGIN_DATA_DIRECTORY = (File::getSpecialLocation(File::userDesktopDirectory)).getFullPathName() + directorySeparator + PLUGIN_NAME;
    
    while (iter.next())
    {
        File sound_file (iter.getFile());
    //        String sound_file_name = sound_file.getFullPathName();
        std::string sound_file_name = sound_file.getFullPathName().toStdString();
//        DBG(sound_file_name);

        Core::Sound sound = Core::Sound( sound_file_name );

    //        Core::Sound scopy = sound;
        
//        mSound[i] = std::make_unique<Core::Sound>(sound_file_name);
//
        this->instrument.num_samples_loaded++;
        
        this->instrument.note[ sound.note ]->velocity[ sound.velocity ]->sound = sound;
        this->instrument.note[ sound.note ]->velocity[ sound.velocity ]->loaded = true;
        
    //        this->instrument.note[ sound.note ]->velocity[ sound.velocity ]->setSound( sound );
    //        this->instrument.note[ sound.note ]->velocity[ sound.velocity ]->loadSound( sound_file_name );
    }
    
    DBG("Sound files loaded: " + String(this->instrument.num_samples_loaded));
    
    AlertWindow aux ("Sound files loaded", "Sound files loaded: " + String(this->instrument.num_samples_loaded), AlertWindow::NoIcon);
    aux.showMessageBox (AlertWindow::WarningIcon,
                        "Sound files loaded",
                        "Number of sound files loaded: " + String(this->instrument.num_samples_loaded),
                        "Accept");
    
    if ( this->instrument.num_samples_loaded == 0)
    {
        JUCEApplication::getInstance()->systemRequestedQuit();
    }
    
    //                AlertWindow aux ("Sound not found", "The sound '" + sound_2_file_path + "' is not in your collection", AlertWindow::NoIcon);
    //                aux.showMessageBox (AlertWindow::WarningIcon,
    //                                    "Sound not found",
    //                                    "The preset couldn't be loaded, the sound '" + sound_2_file_path + "' is not in your library",
    //                                    "Accept");
    
    // Add some voices to our synth, to play the sounds..
    for (int i = 0; i < MAX_VOICES; i++)
    {
        // Add the voice to the synth
//        this->addVoice( new Voice(this->instrument, parameters) );
        this->addVoice( new Voice(this->instrument, parameters, i) );
//        this->addVoice( new Voice(mSound, parameters) );
    }
    
    // TODO - NOTE: If we enter on release stated, there is no way back, the
    // sound goes always down, even if a new sound is selected in a pitchbend from the instrument
    // also on stop note, jump to the release section of the sound (all 4 morph sounds)
    
    // TODO - Maybe we should move the moprhing code from Voice to MorphSound
    // and in Voice manipulate the output audio buffer (already morphed) to
    // decorrelate the signal (or maybe add some "spread")
    
    // Add a sound for them to play
    this->clearSounds();
    this->addSound( new MorphSound() );
    
    // TODO TEST
    this->setNoteStealingEnabled (true);
//    this->setNoteStealingEnabled (false);
}

MorphexSynth::~MorphexSynth() {}

void MorphexSynth::setCurrentPlaybackSampleRate (double sampleRate)
{
    currentSampleRate = sampleRate;
    
    Voice* morph_voice;
    
    // Set new sample rate to ADSR for each voice instance
    for (int i=0; i < this->getNumVoices();i++)
    {
        if( ( morph_voice = dynamic_cast<Voice*>( this->getVoice(i) ) ) )
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

