/*
 ==============================================================================
 
 MorphexSynth.cpp
 Created: 21 Jul 2019 1:05:43pm
 Author:  Marc Sanchez Martinez
 
 ==============================================================================
 */

#include "MorphexSynth.h"

#include "../Helpers/SMTConstants.h"

MorphexSynth::MorphexSynth(AudioProcessorValueTreeState* parameters)
:   mParameters (parameters)
{
//    mSound[1] = std::make_unique<Core::Sound>(DEFAULT_SOUND_FILE_1, DEFAULT_SOUND_FILE_1_COLLECTION_PATH);
//    mSound[2] = std::make_unique<Core::Sound>(DEFAULT_SOUND_FILE_2, DEFAULT_SOUND_FILE_2_COLLECTION_PATH);
    
    initializeDSP();
    
    // Initialize the instrument
    this->instrument = Instrument();
    
//    // Initialize the instrument
//    this->instrument = Instrument();

    //    JUCE::DirectoryIterator()
//    std::string instrument_folder = "/Users/Marc/Research/Repos/morphex-research/data/instruments/Morph";
//    std::string instrument_folder = "/Users/Marc/Research/Repos/morphex-research/data/instruments/Suitcase Dry Test";
//    std::string instrument_folder = "/Users/Marc/Documents/Audio Plugins/Morphex/Instruments/Suitcase Dry Full Velocity Test";
    //    std::string instrument_folder = "/Users/Marc/Documents/Audio Plugins/Morphex/Instruments/Suitcase Dry Full";
    
    
//    std::string instrument_folder = "Suitcase Dry Full Velocity Test";
//    std::string instrument_folder = "Suitcase Dry Full";
//    std::string instrument_folder = "Morphing Test";
//    std::string instrument_folder = "Suitcase Dry Test 20200520";
//    std::string instrument_folder = "Morphex Test";
    
    this->instrument.mode = Instrument::Mode::Morphing;
    this->instrument.interpolation_mode = Instrument::Interpolation::Manual;
//    this->instrument.mode = Instrument::Mode::FullRange;
//    this->instrument.interpolation_mode = Instrument::Interpolation::None;
    
    this->instrument.generate.harmonic = true;
    this->instrument.generate.sinusoidal = false;
    this->instrument.generate.stochastic = false;
    this->instrument.generate.attack = false;
    this->instrument.generate.residual = false;
    
    std::string full_path = PLUGIN_DATA_COLLECTIONS_FACTORY_DIRECTORY.toStdString();
    
    if (this->instrument.mode == Instrument::Mode::FullRange)
    {
        //    std::string instrument_folder = "Suitcase Dry 20200615";
        std::string instrument_folder = "Suitcase Dry Test";
        //    std::string instrument_folder = "Suitcase Dry Full";
        //    std::string instrument_folder = "Morphing Test Optimized";
        full_path = PLUGIN_DATA_DIRECTORY.toStdString() + directorySeparator.toStdString() + "Instruments" + directorySeparator.toStdString() + instrument_folder;
    }

    
    DirectoryIterator iter (File (full_path), true, "*.had");
//    DirectoryIterator iter (File (full_path), true, "*.had");
    
//    static const String PLUGIN_DATA_DIRECTORY = (File::getSpecialLocation(File::userDesktopDirectory)).getFullPathName() + directorySeparator + PLUGIN_NAME;
    
    while (iter.next())
    {
        File sound_file (iter.getFile());
        std::string sound_file_path = sound_file.getFullPathName().toStdString();

        
        
//        Core::Sound sound = Core::Sound( sound_file_path );
        
//        this->instrument.note[ sound.note ]->velocity[ sound.velocity ]->sound = sound;
//        this->instrument.note[ sound.note ]->velocity[ sound.velocity ]->loaded = true;
        
        MorphLocation morph_location = MorphLocation::NUM_MORPH_LOCATIONS;
        
        if (this->instrument.mode == Instrument::Mode::Morphing)
        {
            if (this->instrument.num_samples_loaded < MorphLocation::NUM_MORPH_LOCATIONS)
            {
                morph_location = (MorphLocation) this->instrument.num_samples_loaded;
                this->instrument.loadSound (sound_file_path, morph_location);
                this->instrument.num_samples_loaded++;
            }
        }
        else
        {
            this->instrument.loadSound (sound_file_path, morph_location);
            this->instrument.num_samples_loaded++;
        }
        
        

//        if (this->instrument.mode == Instrument::Mode::Morphing)
//        {
//            if (this->instrument.num_samples_loaded == 0)
//            {
//                this->instrument.setMorphSound (&this->instrument.note[ sound.note ]->velocity[ sound.velocity ]->sound, MorphLocation::Left);
//            }
//            else if (this->instrument.num_samples_loaded == 1)
//            {
//                this->instrument.setMorphSound (&this->instrument.note[ sound.note ]->velocity[ sound.velocity ]->sound, MorphLocation::Right);
//            }
//        }
        
        
    //        this->instrument.note[ sound.note ]->velocity[ sound.velocity ]->setSound( sound );
    //        this->instrument.note[ sound.note ]->velocity[ sound.velocity ]->loadSound( sound_file_path );
    }
    
    DBG("Sound files loaded: " + String(this->instrument.num_samples_loaded));


    /*
    AlertWindow aux ("Sound files loaded", "Sound files loaded: " + String(this->instrument.num_samples_loaded), AlertWindow::InfoIcon);
    aux.showMessageBox (AlertWindow::NoIcon,
                        "Sound files loaded",
                        "Instrument path: " + String(full_path) +
                        "\nNumber of sound files loaded: " + String(this->instrument.num_samples_loaded),
                        "Accept");
    
    if ( this->instrument.num_samples_loaded == 0)
    {
        JUCEApplication::getInstance()->systemRequestedQuit();
    }
    */
    
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
        this->addVoice( new Voice (&this->instrument, parameters) );
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
    Synthesiser::renderNextBlock (outputAudio, inputMidi, startSample, numSamples);
    
//    int total_num_input_channels  = getTotalNumInputChannels();
//    int total_num_output_channels = getTotalNumOutputChannels();
    
//    for (int i_sample = 0; i_sample < numSamples; i_sample++)
//    {
//        auto currentSample = (float) (frame[i_sample] * this->level * this->tailOff);
//        //                    auto currentSample = (float) (std::sin (this->currentAngle) * this->level * this->tailOff);
//
//        for (auto i = outputBuffer.getNumChannels(); --i >= 0;)
//            outputBuffer.addSample (i, startSample, currentSample);
//
//        ++startSample;
//    }
    
    mReverb->process (outputAudio, outputAudio.getNumSamples());
    
    for (int channel = 0; channel < outputAudio.getNumChannels(); ++channel)
    {
        auto* buffer = outputAudio.getWritePointer (channel);
        
        // Output gain
        float output_gain = *mParameters->getRawParameterValue (Morphex::PARAMETERS<float>[Morphex::Parameters::OutputGain].parameter_ID);
        mOutputGain[channel]->process (buffer, output_gain, outputAudio.getNumSamples());
    }
}

void MorphexSynth::reset()
{
    // Reset the instrument
    this->instrument.reset();
}

void MorphexSynth::initializeDSP()
{
    mReverb = std::make_unique <DSP::Reverb> (mParameters);
    
    for (int i = 0; i < NUM_CHANNELS; i++) {
        mOutputGain[i] = std::make_unique <DSP::Gain>();
    }
}
