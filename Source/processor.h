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

#pragma once

#include "core/synth.h"
#include "core/parameters.h"
#include "managers/preset_manager.h"

class MorphexAudioProcessor : public juce::AudioProcessor
{
public:
    //==============================================================================
    MorphexAudioProcessor();
    ~MorphexAudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

#ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
#endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool                  hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool   acceptsMidi() const override;
    bool   producesMidi() const override;
    bool   isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int          getNumPrograms() override;
    int          getCurrentProgram() override;
    void         setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void         changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    //==============================================================================
    morphex::PresetManager& getPresetManager()
    {
        return m_presetManager;
    }

    juce::MidiKeyboardState& getMidiState()
    {
        return m_midiState;
    }

    juce::AudioProcessorValueTreeState parameters;
    morphex::Synth m_synth;

private:
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout()
    {
        //        std::vector<std::unique_ptr<AudioParameterFloat>> params;
        std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

        for (int i = 0; i < morphex::Parameters::TotalNumParameters; i++)
        {
            morphex::Parameter<float> smt_parameter = morphex::PARAMETERS<float>[i];

            params.push_back (std::make_unique<juce::AudioParameterFloat> (smt_parameter.ID, smt_parameter.ID, juce::NormalisableRange<float> (smt_parameter.min_value, smt_parameter.max_value), smt_parameter.default_value, smt_parameter.label));
        }

        return { params.begin(), params.end() };
    }

    morphex::PresetManager m_presetManager;

    juce::MidiKeyboardState    m_midiState;
    juce::MidiMessageCollector m_midiCollector;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MorphexAudioProcessor)
};
