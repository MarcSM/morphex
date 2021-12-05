/* Copyright (C) 2020 Marc Sanchez Martinez
 *
 * https://github.com/MarcSM/morphex
 *
 * Morphex is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Morphex is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Morphex. If not, see <http://www.gnu.org/licenses/>.
 */

#include "processor.h"
#include "editor.h"

MorphexAudioProcessor::MorphexAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
    :
    AudioProcessor (BusesProperties()
#if !JucePlugin_IsMidiEffect
#if !JucePlugin_IsSynth
                        .withInput ("Input", juce::AudioChannelSet::stereo(), true)
#endif
                        .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
#endif
                    ),
    parameters (*this,                    /** reference to processor */
                nullptr,                  /** null pointer to undoManager (optional) */
                juce::Identifier ("MPX"), /** valueTree identifier */
                createParameterLayout()), /** initialize parameters */
#endif
    m_synth (parameters),
    m_presetManager(*this, m_synth)
{
    // Create an initial new preset
    m_presetManager.createNewPreset();
}

MorphexAudioProcessor::~MorphexAudioProcessor()
{
}

//==============================================================================
const juce::String MorphexAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool MorphexAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool MorphexAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

bool MorphexAudioProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
    return true;
#else
    return false;
#endif
}

double MorphexAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int MorphexAudioProcessor::getNumPrograms()
{
    return 1; // NB: some hosts don't cope very well if you tell them there are 0 programs,
    // so this should be at least 1, even if you're not really implementing programs.
}

int MorphexAudioProcessor::getCurrentProgram()
{
    return 0;
}

void MorphexAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String MorphexAudioProcessor::getProgramName (int index)
{
    return {};
}

void MorphexAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void MorphexAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    juce::ignoreUnused (samplesPerBlock);

    // Pre-playback initializations
    m_midiCollector.reset (sampleRate);
    m_synth.setCurrentPlaybackSampleRate (sampleRate);
}

void MorphexAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool MorphexAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
#if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
#else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
        && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

        // This checks if the input layout matches the output layout
#if !JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
#endif

    return true;
#endif
}
#endif

void MorphexAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    buffer.clear();

    // Get next midi events
    m_midiState.processNextMidiBuffer (midiMessages, 0, buffer.getNumSamples(), true);

    // TODO - CHECK (buffer, midiMessages, -> 0 <- , buffer.getNumSamples());

    // Get the synth to process the midi events and generate its output
    m_synth.renderNextBlock (buffer, midiMessages, 0, buffer.getNumSamples());
}

//==============================================================================
bool MorphexAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* MorphexAudioProcessor::createEditor()
{
    return new MorphexAudioProcessorEditor (*this);
}

//==============================================================================
void MorphexAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.

    juce::XmlElement preset ("MPF_StateInfo");

    juce::XmlElement* presetBody = new juce::XmlElement ("MPF_Preset");

    m_presetManager.getXmlForPreset (presetBody);

    preset.addChildElement (presetBody);
    copyXmlToBinary (preset, destData);
}

void MorphexAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState = getXmlFromBinary (data, sizeInBytes);

    if (xmlState != nullptr)
    {
        forEachXmlChildElement (*xmlState, subChild)
        {
            // Load the preset parameters
            m_presetManager.loadPresetFromXml (subChild);
        }
    }
    else
    {
        jassertfalse;
    }
}

// This creates new instances of the plugin
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new MorphexAudioProcessor();
}
