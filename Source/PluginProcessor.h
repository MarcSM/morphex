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

#include "../JuceLibraryCode/JuceHeader.h"

#include "Entities/MorphexSynth.h"
#include "Managers/PresetManager.h"

#include "Helpers/SMTParameters.h"

//==============================================================================
/**
 */
class SpectralMorphingToolAudioProcessor
:   public AudioProcessor
{
public:
    //==============================================================================
    SpectralMorphingToolAudioProcessor();
    ~SpectralMorphingToolAudioProcessor();
    
    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
    
#ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
#endif
    
    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;
    
    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;
    
    //==============================================================================
    const String getName() const override;
    
    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;
    
    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;
    
    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    //==============================================================================
    AudioProcessorValueTreeState parameters;
    
    PresetManager* getPresetManager()
    {
        return mPresetManager.get();
    }
    
    MidiKeyboardState& getMidiState()
    {
        return midiState;
    }
    
    MorphexSynth mMorphexSynth { &parameters };
    
private:
    
    AudioProcessorValueTreeState::ParameterLayout createParameterLayout()
    {
//        std::vector<std::unique_ptr<AudioParameterFloat>> params;
        std::vector<std::unique_ptr<RangedAudioParameter>> params;

        for(int i = 0; i < Morphex::Parameters::TotalNumParameters; i++) {
            
            Morphex::Parameter<float> smt_parameter = Morphex::PARAMETERS<float>[i];
            
            params.push_back
                (std::make_unique<AudioParameterFloat>
                    (smt_parameter.ID, smt_parameter.ID,
                     NormalisableRange<float> (smt_parameter.min_value, smt_parameter.max_value),
                     smt_parameter.default_value, smt_parameter.label)
                 );
        }
        
        return { params.begin(), params.end() };
    }
    
    MidiKeyboardState midiState;
    MidiMessageCollector midiCollector;
    
    std::unique_ptr<PresetManager> mPresetManager;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SpectralMorphingToolAudioProcessor)
};

