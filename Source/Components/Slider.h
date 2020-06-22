/*
  ==============================================================================

    Slider.h
    Created: 22 Jun 2020 3:49:02pm
    Author:  Marc Sanchez Martinez

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

namespace Morphex { class Slider; }

class Morphex::Slider : public juce::Slider
{
public:
    
    Slider (AudioProcessorValueTreeState& state_to_control,
            const Morphex::Parameters parameter_num)
    :   juce::Slider ()
    {
        Morphex::Parameter morphex_parameter = Morphex::PARAMETERS<float>[parameter_num];
        
        setName (morphex_parameter.label);
        
        setSliderStyle (Slider::SliderStyle::RotaryHorizontalVerticalDrag);
//        setTextBoxStyle (Slider::NoTextBox, true, 0, 0);
        setTextBoxStyle (Slider::TextEntryBoxPosition::TextBoxBelow, false, 0, 0);
        setRange (morphex_parameter.min_value, morphex_parameter.max_value, 0.001f);
        
//        mAttachment = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>
//                      (state_to_control, parameter_ID, this);

        mAttachment = new AudioProcessorValueTreeState::SliderAttachment
                      (state_to_control, morphex_parameter.ID, *this);
    }
    
    ~Slider() {}
    
private:
    
//    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> mAttachment;
    ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> mAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Slider);
};
