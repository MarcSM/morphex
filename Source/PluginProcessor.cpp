/*
 ==============================================================================
 
 This file was auto-generated!
 
 It contains the basic framework code for a JUCE plugin processor.
 
 ==============================================================================
 */

#include "PluginProcessor.h"
#include "PluginEditor.h"

#include "Helpers/SMTConstants.h"
#include "Helpers/SMTUtils.h"


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
           juce::Identifier ("SMT"), /** valueTree identifier */
           createParameterLayout()) /** initialize parameters */
#endif
{
    // Initialize the preset manager
    mPresetManager = std::make_unique<PresetManager> (this, &mMorphexSynth);
    
    // Create an initial new preset
    mPresetManager->createNewPreset();
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
    ignoreUnused (samplesPerBlock);
//    lastSampleRate = sampleRate
    
    // Pre-playback initializations
    midiCollector.reset (sampleRate);
    mMorphexSynth.setCurrentPlaybackSampleRate (sampleRate);
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
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
        && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
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
    midiState.processNextMidiBuffer (midiMessages, 0, buffer.getNumSamples(), true);
    
    // TODO - CHECK (buffer, midiMessages, -> 0 <- , buffer.getNumSamples());
    
    // Get the synth to process the midi events and generate its output
    mMorphexSynth.renderNextBlock (buffer, midiMessages, 0, buffer.getNumSamples());
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
    
    XmlElement preset ("MPF_StateInfo");
    
    XmlElement* presetBody = new XmlElement ("MPF_Preset");
    
    mPresetManager->getXmlForPreset (presetBody);
    
    preset.addChildElement (presetBody);
    copyXmlToBinary (preset, destData);
}

void SpectralMorphingToolAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<XmlElement> xmlState = getXmlFromBinary (data, sizeInBytes);
    
    if (xmlState != nullptr)
    {
        forEachXmlChildElement (*xmlState, subChild)
        {
            // Load the preset parameters
            mPresetManager->loadPresetFromXml (subChild);
        }
    }
    else
    {
        jassertfalse;
    }
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SpectralMorphingToolAudioProcessor();
}

