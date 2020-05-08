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
    mPresetManager = std::make_unique<SMTPresetManager>(this);
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
    
//    // Save the path of the sound 1
//    std::string sound_1_collection_path = sound[1]->path;
//    removeSubStr(sound_1_collection_path, (PLUGIN_DATA_COLLECTIONS_DIRECTORY + directorySeparator).toStdString() );
//    presetBody->setAttribute(SOUND_FILE_1_PATH_PARAMETER_ID, sound_1_collection_path);
//
//    // Save the path of the sound 2
//    std::string sound_2_collection_path = sound[2]->path;
//    removeSubStr(sound_2_collection_path, (PLUGIN_DATA_COLLECTIONS_DIRECTORY + directorySeparator).toStdString() );
//    presetBody->setAttribute(SOUND_FILE_2_PATH_PARAMETER_ID, sound_2_collection_path);
    
    mPresetManager->getXmlForPreset(presetBody);
    
    preset.addChildElement(presetBody);
    copyXmlToBinary(preset, destData);
}

void SpectralMorphingToolAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    
    bool error_loading_sounds = false;
    
    std::unique_ptr<XmlElement> xmlState;
    
    xmlState.reset(getXmlFromBinary(data, sizeInBytes));
    
    if (xmlState != nullptr) {
        forEachXmlChildElement(*xmlState, subChild) {
            
            // Retrieve the path of the sounds
            std::string sound_1_file_path = subChild->getStringAttribute(SOUND_FILE_1_PATH_PARAMETER_ID).toStdString();
            std::string sound_2_file_path = subChild->getStringAttribute(SOUND_FILE_2_PATH_PARAMETER_ID).toStdString();
            
            // Construct the full path
            sound_1_file_path = (PLUGIN_DATA_COLLECTIONS_DIRECTORY + directorySeparator).toStdString() + sound_1_file_path;
            sound_2_file_path = (PLUGIN_DATA_COLLECTIONS_DIRECTORY + directorySeparator).toStdString() + sound_2_file_path;
            
            // Remove the path of the sounds from the xml
            subChild->removeAttribute(SOUND_FILE_1_PATH_PARAMETER_ID);
            subChild->removeAttribute(SOUND_FILE_2_PATH_PARAMETER_ID);
            
            // Check if sound 1 file path is valid
            File sound_1_check(sound_1_file_path);
            
            if (!sound_1_check.existsAsFile())
            {
                error_loading_sounds = true;
//                AlertWindow aux ("Sound not found", "The sound '" + sound_1_file_path + "' is not in your collection", AlertWindow::NoIcon);
//                aux.showMessageBox (AlertWindow::WarningIcon,
//                                    "Sound not found",
//                                    "The preset couldn't be loaded, the sound '" + sound_1_file_path + "' is not in your library",
//                                    "Accept");
            }
            
            // Check if sound 2 file path is valid
            File sound_2_check(sound_2_file_path);
            
            if (!sound_2_check.existsAsFile())
            {
                error_loading_sounds = true;
//                AlertWindow aux ("Sound not found", "The sound '" + sound_2_file_path + "' is not in your collection", AlertWindow::NoIcon);
//                aux.showMessageBox (AlertWindow::WarningIcon,
//                                    "Sound not found",
//                                    "The preset couldn't be loaded, the sound '" + sound_2_file_path + "' is not in your library",
//                                    "Accept");
            }
            
            if (!error_loading_sounds)
            {
                // Load the sounds
//                sound[1]->load(sound_1_file_path);
//                sound[2]->load(sound_2_file_path);
                
                // Load the preset parameters
                mPresetManager->loadPresetForXml(subChild);
            }
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

