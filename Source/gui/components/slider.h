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

#include <JuceHeader.h>

namespace morphex
{
class Slider : public juce::Slider
{
public:
    Slider (juce::AudioProcessorValueTreeState& state_to_control, const Morphex::Parameters parameter_num) :
        juce::Slider()
    {
        Morphex::Parameter morphex_parameter = Morphex::PARAMETERS<float>[parameter_num];

        setName (morphex_parameter.label);

        setSliderStyle (Slider::SliderStyle::RotaryHorizontalVerticalDrag);
        //        setTextBoxStyle (Slider::NoTextBox, true, 0, 0);
        setTextBoxStyle (Slider::TextEntryBoxPosition::TextBoxBelow, false, 0, 0);
        setRange (morphex_parameter.min_value, morphex_parameter.max_value, 0.001f);

        m_attachment = new juce::AudioProcessorValueTreeState::SliderAttachment (state_to_control, morphex_parameter.ID, *this);
    }

    ~Slider() {}

private:
    // TODO
    //    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> m_attachment;
    juce::ScopedPointer<juce::AudioProcessorValueTreeState::SliderAttachment> m_attachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Slider);
};
}; // namespace morphex
