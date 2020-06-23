/*
 ==============================================================================
 
 MorphexSynth.cpp
 Created: 21 Jul 2019 1:05:43pm
 Author:  Marc Sanchez Martinez
 
 ==============================================================================
 */

#include "MorphexSynth.h"

#include "../Helpers/SMTConstants.h"

MorphexSynth::MorphexSynth (AudioProcessorValueTreeState* parameters)
:   mParameters (parameters)
{
    this->initializeDSP();
    
    // Initialize the instrument
    this->instrument = Instrument();
    
    this->instrument.mode = Instrument::Mode::Morphing;
    this->instrument.interpolation_mode = Instrument::Interpolation::Manual;
//    this->instrument.mode = Instrument::Mode::FullRange;
//    this->instrument.interpolation_mode = Instrument::Interpolation::None;
    
    this->instrument.generate.harmonic = true;
    this->instrument.generate.sinusoidal = false;
    this->instrument.generate.stochastic = false;
    this->instrument.generate.attack = false;
    this->instrument.generate.residual = false;
    
    // For testing teh full range instrument case
    bool load_default_sounds = false;
    
    if (load_default_sounds)
    {
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
        }
        
        DBG("Sound files loaded: " + String (this->instrument.num_samples_loaded));
    }
    
    // Add some voices to our synth, to play the sounds..
    for (int i = 0; i < MAX_VOICES; i++)
    {
        this->addVoice (new Voice (&this->instrument, parameters));
    }
    
    // TODO - NOTE: If we enter on release stated, there is no way back, the
    // sound goes always down, even if a new sound is selected in a pitchbend from the instrument
    // also on stop note, jump to the release section of the sound (all 4 morph sounds)
    
    // TODO - Maybe we should move the moprhing code from Voice to MorphSound
    // and in Voice manipulate the output audio buffer (already morphed) to
    // decorrelate the signal (or maybe add some "spread")
    
    // Add a sound for them to play
    this->clearSounds();
    this->addSound (new MorphSound());
    this->setNoteStealingEnabled (true);
}

MorphexSynth::~MorphexSynth() {}

void MorphexSynth::setCurrentPlaybackSampleRate (double sampleRate)
{
    currentSampleRate = sampleRate;
    
    Voice* morph_voice;
    
    // Set new sample rate to ADSR for each voice instance
    for (int i=0; i < this->getNumVoices();i++)
    {
        if ((morph_voice = dynamic_cast<Voice*> (this->getVoice(i))))
        {
            morph_voice->setADSRSampleRate (sampleRate);
        }
    }
    
    // Call base class method
    Synthesiser::setCurrentPlaybackSampleRate (sampleRate);
}

void MorphexSynth::renderNextBlock (AudioBuffer<float>& outputAudio,
                                    const MidiBuffer& inputMidi,
                                    int startSample, int numSamples)
{
    // Call base class method
    Synthesiser::renderNextBlock (outputAudio, inputMidi, startSample, numSamples);
    
    // Reverb
    mReverb->process (outputAudio, outputAudio.getNumSamples());
    
    // Output gain
    for (int channel = 0; channel < outputAudio.getNumChannels(); ++channel)
    {
        auto* buffer = outputAudio.getWritePointer (channel);
        
        // Output gain
        float output_gain = *mParameters->getRawParameterValue (Morphex::PARAMETERS<float>[Morphex::Parameters::OutputGain].ID);
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
    
    for (int i = 0; i < NUM_CHANNELS; i++)
    {
        mOutputGain[i] = std::make_unique <DSP::Gain>();
    }
}
