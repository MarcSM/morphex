/*
  ==============================================================================

    SMTParameterSlider.cpp
    Created: 14 Jun 2019 7:08:38pm
    Author:  Marc Sanchez Martinez

  ==============================================================================
*/

#include "SMTParameterSlider.h"

#include "SMTInterfaceDefines.h"

#include "SMTParameters.h"

SMTParameterSlider::SMTParameterSlider(AudioProcessorValueTreeState& stateToControl,
                                       const String& parameterID,
                                       const String& parameterLabel)
:   juce::Slider(parameterLabel)
{
    ParameterStruct smt_parameter = getParameterByID(parameterID);
    
    // Set colours
    setColour(Slider::backgroundColourId, Colours::white);
    setColour(Slider::thumbColourId, Colour(200, 200, 200));
    setColour(Slider::trackColourId, Colours::white);
    
    setColour(Slider::rotarySliderFillColourId, Colours::white);
    setColour(Slider::rotarySliderOutlineColourId, Colours::white);
    
    setColour(Slider::textBoxTextColourId, Colours::white);
    setColour(Slider::textBoxBackgroundColourId, SMTTransparentColour);
    setColour(Slider::textBoxHighlightColourId, Colours::white);
    setColour(Slider::textBoxOutlineColourId, SMTTransparentColour);

    setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxBelow, false, 0, 0);
    setRange(smt_parameter.min_value, smt_parameter.max_value, 0.001f);

    mAttachment =
    new AudioProcessorValueTreeState::
    SliderAttachment(stateToControl, parameterID, *this);
}

SMTParameterSlider::~SMTParameterSlider() {}
