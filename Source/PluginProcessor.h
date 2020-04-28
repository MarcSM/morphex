/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

#include "MorphexSynth.h"

#include "SMTPresetManager.h"
#include "SMTParameters.h"

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
    
    SMTPresetManager* getPresetManager()
    {
        return mPresetManager.get();
    }
    
    MidiKeyboardState& getMidiState()
    {
         return midiState;
    }
    
    SoundArray sound;
    
private:
    
    AudioProcessorValueTreeState::ParameterLayout createParameterLayout()
    {
        std::vector<std::unique_ptr<AudioParameterFloat>> params;
        
        for(int i = 0; i < kParameter_TotalNumParameters; i++) {
            
            ParameterStruct smt_parameter = SMT_PARAMETERS[i];
        
            params.push_back(std::make_unique<AudioParameterFloat>( smt_parameter.parameter_ID,
                                                                    smt_parameter.parameter_ID,
                                                                    NormalisableRange<float>(
                                                                        smt_parameter.min_value,
                                                                        smt_parameter.max_value
                                                                    ),
                                                                    smt_parameter.default_value,
                                                                    smt_parameter.parameter_label)  );
        }
        
        return { params.begin(), params.end() };
    }
    
    MorphexSynth mMorphexSynth { &parameters };

    MidiKeyboardState midiState;
    MidiMessageCollector midiCollector;

    std::unique_ptr<SMTPresetManager> mPresetManager;
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SpectralMorphingToolAudioProcessor)
};
