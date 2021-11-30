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

#include "../components/slider.h"

class FxPanel : public juce::Component
{
public:
    FxPanel (MorphexAudioProcessor& inProcessor)
    {
        // TODO - Filters FX
        // TODO - Distortion FX
        // TODO - Delay FX

        // Reverb FX
        FxRow* reverb_panel = new FxRow (inProcessor, "Reverb");
        addAndMakeVisible (m_fxRows.add (reverb_panel));

        // Reverb FX - Dry / Wet
        morphex::Slider* reverb_drywet_slider = new morphex::Slider (inProcessor.parameters, morphex::Parameters::ReverbDryWet);
        addAndMakeVisible (reverb_panel->m_sliders.add (reverb_drywet_slider));
    }

    ~FxPanel()
    {
    }

    void paint (juce::Graphics& g) override
    {
        g.fillAll (juce::Colours::black);

        // Draw borders
        GUI::Paint::drawBorders (g, getLocalBounds());
    }

    void resized() override
    {
        auto bounds     = getLocalBounds();
        auto row_height = bounds.getHeight() * 0.18;

        for (int i = 0; i < m_fxRows.size(); ++i)
        {
            m_fxRows[i]->setBounds (0, row_height * i, getWidth(), row_height);
        }
    }

private:
    struct FxRow : public Component
    {
        FxRow (MorphexAudioProcessor& inProcessor, juce::String name) :
            m_name (name)
        {
            m_sliders.clear();
        }

        void paint (juce::Graphics& g) override
        {
            g.fillAll (GUI::Color::BackgroundDark);
            g.setColour (juce::Colours::white);
            g.setFont (14.0f);
            g.drawText (m_name, getLocalBounds(), juce::Justification::topLeft, true);

            // Draw borders
            GUI::Paint::drawBorders (g, getLocalBounds());
        }

        void resized() override
        {
            juce::FlexBox fb;
            fb.flexDirection  = juce::FlexBox::Direction::column;
            fb.flexWrap       = juce::FlexBox::Wrap::wrap;
            fb.justifyContent = juce::FlexBox::JustifyContent::flexEnd;

            juce::FlexItem drywet_knob (getWidth(), getHeight(), *m_sliders[0]);

            fb.items.addArray ({ drywet_knob });
            fb.performLayout (getLocalBounds().toFloat());
        }

        juce::String m_name;

        juce::OwnedArray<juce::Slider> m_sliders;
    };

    juce::OwnedArray<FxRow> m_fxRows;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FxPanel)
};
