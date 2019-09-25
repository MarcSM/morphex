/*
  ==============================================================================

    SMTAdditionalParameters.cpp
    Created: 11 Aug 2019 11:53:14am
    Author:  Sanchez Martinez, Marc

  ==============================================================================
*/

#include "SMTAdditionalParameters.h"

#include "SMTParameters.h"

SMTAdditionalParameters::SMTAdditionalParameters(SpectralMorphingToolAudioProcessor* inProcessor):
    mProcessor(inProcessor)
{
    mSliders.clear();
    
    // Stochastic interpolation factor slider
    stocs_interp_factor = new SMTParameterSlider(mProcessor->parameters,
                                                 SMTParameterID[SMTParameter::kParameter_stocs_interp_factor],
                                                 SMTParameterLabel[SMTParameter::kParameter_stocs_interp_factor]);
    addAndMakeVisible(stocs_interp_factor);
    mSliders.add(stocs_interp_factor);
    
    // Stochastic gain slider
    stocs_gain = new SMTParameterSlider(mProcessor->parameters,
                                                 SMTParameterID[SMTParameter::kParameter_stocs_gain],
                                                 SMTParameterLabel[SMTParameter::kParameter_stocs_gain]);
    addAndMakeVisible(stocs_gain);
    mSliders.add(stocs_gain);
}

SMTAdditionalParameters::~SMTAdditionalParameters()
{
    mSliders.clear();
}

void SMTAdditionalParameters::paint(Graphics& g)
{
    // Text label color
    g.setColour (Colours::white);
    g.setFont(font_2);
    
    const int margin = 10;
    
    const int slider_width = getWidth();
    const int slider_height = 20;
    
    int slider_x = 0;
    int slider_y = margin + 25;
    
    stocs_interp_factor->setBounds(slider_x, slider_y, slider_width, slider_height);
    
    // Paint labels
    for (int i = 0; i < mSliders.size(); i++) {
        paintComponentLabel(g, mSliders[i]);
    }
}
