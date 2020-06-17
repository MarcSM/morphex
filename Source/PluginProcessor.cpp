/*
 ==============================================================================
 
 This file was auto-generated!
 
 It contains the basic framework code for a JUCE plugin processor.
 
 ==============================================================================
 */

#include "PluginProcessor.h"
#include "PluginEditor.h"

#include "SMTConstants.h"
#include "SMTUtils.h"


//==============================================================================
SpectralMorphingToolAudioProcessor::SpectralMorphingToolAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
: AudioProcessor (BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
                  .withInput  ("Input",  AudioChannelSet::stereo(), true)
#endif
                  .withOutput ("Output", AudioChannelSet::stereo(), true)
#endif
                  ),
parameters(*this,                   /** reference to processor */
           nullptr,                 /** null pointer to undoManager (optional) */
           juce::Identifier("SMT"), /** valueTree identifier */
           createParameterLayout()) /** initialize parameters */
#endif
{
//    sound[1] = std::make_unique<Core::Sound>(DEFAULT_SOUND_FILE_1, DEFAULT_SOUND_FILE_1_COLLECTION_PATH);
//    sound[2] = std::make_unique<Core::Sound>(DEFAULT_SOUND_FILE_2, DEFAULT_SOUND_FILE_2_COLLECTION_PATH);
    
//    int max_len, max_harmonics;
    
//    // Get the maximum overall shape (length, number of harmonics)
//    std::tie(max_len, max_harmonics) = getMaxShape(sound[1]->model->values.harmonics_freqs,
//                                                   sound[2]->model->values.harmonics_freqs);
    
//    // Zero padding the "harmonic_frequencies" vectors to have the same size
//    sound[1]->model->values.harmonics_freqs.resize(max_len, std::vector<float>(max_harmonics));
//    sound[2]->model->values.harmonics_freqs.resize(max_len, std::vector<float>(max_harmonics));
//
//    // Zero padding the "harmonic_magnitudes" vectors to have the same size
//    sound[1]->model->values.harmonics_mags.resize(max_len, std::vector<float>(max_harmonics));
//    sound[2]->model->values.harmonics_mags.resize(max_len, std::vector<float>(max_harmonics));
//
//    // Zero padding the "harmonic_phases" vectors to have the same size
//    sound[1]->model->values.harmonics_phases.resize(max_len, std::vector<float>(max_harmonics));
//    sound[2]->model->values.harmonics_phases.resize(max_len, std::vector<float>(max_harmonics));
    
    // Zero padding the "stochastic_residual" vectors to have the same size
//    sound[1]->stochastic_residual.resize(max_len, std::vector<float>(max_harmonics));
//    sound[2]->stochastic_residual.resize(max_len, std::vector<float>(max_harmonics));
    
    // Initialize the preset manager
    mPresetManager = std::make_unique<PresetManager>(this, &mMorphexSynth);
}

SpectralMorphingToolAudioProcessor::~SpectralMorphingToolAudioProcessor()
{
}

//==============================================================================
const String SpectralMorphingToolAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool SpectralMorphingToolAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool SpectralMorphingToolAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

bool SpectralMorphingToolAudioProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
    return true;
#else
    return false;
#endif
}

double SpectralMorphingToolAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int SpectralMorphingToolAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
    // so this should be at least 1, even if you're not really implementing programs.
}

int SpectralMorphingToolAudioProcessor::getCurrentProgram()
{
    return 0;
}

void SpectralMorphingToolAudioProcessor::setCurrentProgram (int index)
{
}

const String SpectralMorphingToolAudioProcessor::getProgramName (int index)
{
    return {};
}

void SpectralMorphingToolAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void SpectralMorphingToolAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    ignoreUnused(samplesPerBlock);
//    lastSampleRate = sampleRate
    
    // Pre-playback initializations
    midiCollector.reset(sampleRate);
    mMorphexSynth.setCurrentPlaybackSampleRate(sampleRate);
}

void SpectralMorphingToolAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool SpectralMorphingToolAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
#if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
#else
    // This is the place where you check if the layout is supported.
    // In this template code we only support stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;
    
    // This checks if the input layout matches the output layout
#if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
#endif
    
    return true;
#endif
}
#endif

void SpectralMorphingToolAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    buffer.clear();
    
    // Get next midi events
    midiState.processNextMidiBuffer(midiMessages, 0, buffer.getNumSamples(), true);
    
    // TODO CHECK - (buffer, midiMessages, -> 0 <- , buffer.getNumSamples());
    
    // Get the synth to process the midi events and generate its output
    mMorphexSynth.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());
}

//==============================================================================
bool SpectralMorphingToolAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* SpectralMorphingToolAudioProcessor::createEditor()
{
    return new SpectralMorphingToolAudioProcessorEditor (*this);
}

//==============================================================================
void SpectralMorphingToolAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    
    XmlElement preset("MPF_StateInfo");
    
    XmlElement* presetBody = new XmlElement("MPF_Preset");
    
//    // Save morph sound's file path
//    MorphSounds morph_sounds = this->mMorphexSynth.instrument.getMorphSounds();
//
//    for (int i = 0; i < morph_sounds.size(); i++)
//    {
//        std::string sound_file_path = morph_sounds[i]->path;
//        removeSubStr (sound_file_path, (PLUGIN_DATA_COLLECTIONS_DIRECTORY + directorySeparator).toStdString());
//        String sound_file_path_id = SOUND_FILE_PATH_PARAMETER_ID + String (i + 1);
//        presetBody->setAttribute (sound_file_path_id, sound_file_path);
//    }
    
    mPresetManager->getXmlForPreset(presetBody);
    
    preset.addChildElement(presetBody);
    copyXmlToBinary(preset, destData);
}

void SpectralMorphingToolAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    
    std::unique_ptr<XmlElement> xmlState;
    
//    pluginStateInfoXML = parameters.state.createXml();
//    and
//    pluginStateInfoXML = getXmlFromBinary(data, sizeInBytes);
    
    xmlState = getXmlFromBinary(data, sizeInBytes);
//    xmlState.reset();
//    xmlState.reset(getXmlFromBinary(data, sizeInBytes));
    
//    std::unique_ptr<XmlElement> model;
//    model = AudioProcessor::getXmlFromBinary(data, sizeInBytes);
//    model = XmlDocument::parse(f);
//    model = std::make_unique<XmlElement>("Test");
    
    if (xmlState != nullptr) {
        forEachXmlChildElement(*xmlState, subChild) {
            
//            // Get morph sound's file path
//            MorphSounds morph_sounds = this->mMorphexSynth.instrument.getMorphSounds();
//            
//            // Error vars
//            bool error_loading_sounds = false;
//            String error_message = "The preset couldn't be loaded, the following sounds are not in your library:\n";
//            
//            for (int i = 0; i < morph_sounds.size(); i++)
//            {
//                // Generate sound ID
//                String sound_file_path_id = SOUND_FILE_PATH_PARAMETER_ID + String (i + 1);
//                
//                // Retrieve the sound file path
//                std::string sound_file_path = subChild->getStringAttribute (sound_file_path_id).toStdString();
//                
//                // If it's not empty
//                if (!sound_file_path.empty())
//                {
//                    // Prepend the user's plugin data directory
//                    sound_file_path = (PLUGIN_DATA_COLLECTIONS_DIRECTORY + directorySeparator).toStdString() + sound_file_path;
//                    
//                    // Check if sound file path is valid
//                    File sound_file_path_check (sound_file_path);
//                    
//                    if (!sound_file_path_check.existsAsFile())
//                    {
//                        error_loading_sounds = true;
//                        error_message += String ("\n" + sound_file_path);
//                    }
//                    else
//                    {
//                        this->mMorphexSynth.instrument.loadSound (sound_file_path, (MorphLocation) i);
//                    }
//                }
//                
//                // Remove the sound file path element from the xml
//                subChild->removeAttribute ( String(sound_file_path_id) );
//            }
//            
//            if (error_loading_sounds)
//            {
//                AlertWindow aux ("Sounds not found", "", AlertWindow::NoIcon);
//                aux.showMessageBox (AlertWindow::WarningIcon, "Sounds not found", error_message, "Accept");
//            }
//            else
//            {
//                // Load the sounds
//                //                sound[1]->load(sound_1_file_path);
//                //                sound[2]->load(sound_2_file_path);
//                
////                // Load the preset parameters
////                mPresetManager->loadPresetForXml(subChild);
//            }
            
            // Load the preset parameters
            mPresetManager->loadPresetForXml (subChild);
        }
    } else {
        jassertfalse;
    }
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SpectralMorphingToolAudioProcessor();
}

