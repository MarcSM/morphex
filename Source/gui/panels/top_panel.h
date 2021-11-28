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

#include "../components/slider.h"

#include "preset_manager_panel.h"

class TopPanel : public Component
{
public:
    TopPanel (MorphexAudioProcessor& processor) :
        m_centerPanel (processor),
        m_rightPanel (processor)
    {
        addAndMakeVisible (m_leftPanel);
        addAndMakeVisible (m_centerPanel);
        addAndMakeVisible (m_rightPanel);
    }

    ~TopPanel() {}

    void paint (Graphics& g) override {}

    void resized() override
    {
        FlexBox fb;

        float centerPanelWidth = getWidth() * 0.75f * 0.45f;
        float soundPanelWidth  = (getWidth() - centerPanelWidth) / 2.0f;

        FlexItem left (soundPanelWidth, getHeight(), m_leftPanel);
        FlexItem center (centerPanelWidth, getHeight(), m_centerPanel);
        FlexItem right (soundPanelWidth, getHeight(), m_rightPanel);

        fb.items.addArray ({ left, center, right });
        fb.performLayout (getLocalBounds().toFloat());
    }

private:
    struct LeftSidePanel : public Component
    {
        LeftSidePanel()
        {
            // Morphex Logo
            m_morphexLongLogo = Drawable::createFromImageData (BinaryData::morphex_long_logo_svg, BinaryData::morphex_long_logo_svgSize);
            m_morphexLongLogo->replaceColour (Colours::white, GUI::Color::Logo);
        }

        void paint (Graphics& g) override
        {
            m_morphexLongLogo->drawWithin (g, m_morphexLongLogoBounds, RectanglePlacement::xLeft, 1.0f);

            // Plugin Version
            g.setColour (Colours::white);
            g.setFont (14.0f);
            g.drawText ("v" + static_cast<std::string>(ProjectInfo::versionString), -m_morphexLongLogoMargin, m_morphexLongLogoMargin, getWidth(), getHeight(), Justification::right);

            // Draw borders
            GUI::Paint::drawBorders (g, getLocalBounds());
        }

        void resized() override
        {
            m_morphexLongLogoMargin = getWidth() * 0.04f;

            m_morphexLongLogoBounds.setBounds (m_morphexLongLogoMargin, m_morphexLongLogoMargin, getWidth() - 2 * m_morphexLongLogoMargin, getHeight() - 2 * m_morphexLongLogoMargin);
        }

        std::unique_ptr<Drawable> m_morphexLongLogo;
        Rectangle<float>          m_morphexLongLogoBounds;
        float                     m_morphexLongLogoMargin;
    };

    struct CenterPanel : public Component
    {
        CenterPanel (MorphexAudioProcessor& processor) :
            preset_manager_panel (processor)
        {
            addAndMakeVisible (preset_manager_panel);
        }

        void paint (Graphics& g) override
        {
            g.fillAll (GUI::Color::BrowserBackground);
        }

        void resized() override
        {
            auto area = getLocalBounds();
            preset_manager_panel.setBounds (area);
        }

        PresetManagerPanel preset_manager_panel;
    };

    struct RightSidePanel : public Component
    {
        RightSidePanel (MorphexAudioProcessor& processor)
        {
            m_sliders.clear();

            m_outputGainSlider = new morphex::Slider (processor.parameters, Morphex::Parameters::OutputGain);
            addAndMakeVisible (m_outputGainSlider);
            m_sliders.add (m_outputGainSlider);
        }

        void paint (Graphics& g) override
        {
            // Draw borders
            GUI::Paint::drawBorders (g, getLocalBounds());
        }

        void resized() override
        {
            FlexBox fb;
            fb.flexDirection  = FlexBox::Direction::column;
            fb.flexWrap       = FlexBox::Wrap::wrap;
            fb.alignContent   = FlexBox::AlignContent::flexEnd;
            fb.alignItems     = FlexBox::AlignItems::flexEnd;
            fb.justifyContent = FlexBox::JustifyContent::flexEnd;

            FlexItem gain_knob (getHeight(), getHeight(), *m_sliders[0]);

            fb.items.addArray ({ gain_knob });
            fb.performLayout (getLocalBounds().toFloat());
        }

        morphex::Slider* m_outputGainSlider;

        OwnedArray<morphex::Slider> m_sliders;
    };

    LeftSidePanel  m_leftPanel;
    CenterPanel    m_centerPanel;
    RightSidePanel m_rightPanel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TopPanel)
};
