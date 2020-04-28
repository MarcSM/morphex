/*
  ==============================================================================

    SMTADSRParameters.cpp
    Created: 17 Aug 2019 1:12:05pm
    Author:  Marc Sanchez Martinez

  ==============================================================================
*/

#include "SMTADSRParameters.h"

SMTADSRParameters::SMTADSRParameters(SpectralMorphingToolAudioProcessor* inProcessor)
:
    mProcessor(inProcessor)
{
    mSliders.clear();
    
    const int margin = 10;

    const int slider_w = 80;
    const int slider_h = 80;

    int slider_x = 30;
    int slider_y = margin + 25;
    
    // Add ADSR attack slider
    asdr_attack = new SMTParameterSlider(mProcessor->parameters,
                                         SMTParameterID[SMTParameter::kParameter_asdr_attack],
                                         SMTParameterLabel[SMTParameter::kParameter_asdr_attack]);
    asdr_attack->setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxBelow, false, 200, 20);
    asdr_attack->setSliderStyle(Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    asdr_attack->setBounds(slider_x, slider_y, slider_w, slider_h);;
    addAndMakeVisible(asdr_attack);
    mSliders.add(asdr_attack);
    slider_x += slider_w;
    
    // Add ADSR decay slider
    asdr_decay = new SMTParameterSlider(mProcessor->parameters,
                                        SMTParameterID[SMTParameter::kParameter_asdr_decay],
                                        SMTParameterLabel[SMTParameter::kParameter_asdr_decay]);
    asdr_decay->setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxBelow, false, 200, 20);
    asdr_decay->setSliderStyle(Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    asdr_decay->setBounds(slider_x, slider_y, slider_w, slider_h);
    addAndMakeVisible(asdr_decay);
    mSliders.add(asdr_decay);
    slider_x += slider_w;
    
    // Add ADSR sustain slider
    asdr_sustain = new SMTParameterSlider(mProcessor->parameters,
                                         SMTParameterID[SMTParameter::kParameter_asdr_sustain],
                                         SMTParameterLabel[SMTParameter::kParameter_asdr_sustain]);
    asdr_sustain->setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxBelow, false, 200, 20);
    asdr_sustain->setSliderStyle(Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    asdr_sustain->setBounds(slider_x, slider_y, slider_w, slider_h);
    addAndMakeVisible(asdr_sustain);
    mSliders.add(asdr_sustain);
    slider_x += slider_w;
    
    // Add ADSR release slider
    asdr_release = new SMTParameterSlider(mProcessor->parameters,
                                         SMTParameterID[SMTParameter::kParameter_asdr_release],
                                         SMTParameterLabel[SMTParameter::kParameter_asdr_release]);
    asdr_release->setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxBelow, false, 200, 20);
    asdr_release->setSliderStyle(Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    asdr_release->setBounds(slider_x, slider_y, slider_w, slider_h);
    addAndMakeVisible(asdr_release);
    mSliders.add(asdr_release);
}

SMTADSRParameters::~SMTADSRParameters() {}

void SMTADSRParameters::paint(Graphics& g)
{
//    // Paint labels
//    for (int i = 0; i < mSliders.size(); i++) {
//        paintComponentLabel(g, mSliders[i]);
//    }
}
