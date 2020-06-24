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

#include "../Components/Slider.h"

#include "PresetManagerPanel.h"

class TopPanel : public Component
{
public:
    
    TopPanel (SpectralMorphingToolAudioProcessor* inProcessor)
    :   centerPanel (inProcessor),
        rightPanel (inProcessor)
    {
        addAndMakeVisible (leftPanel);
        addAndMakeVisible (centerPanel);
        addAndMakeVisible (rightPanel);
    }
    
    ~TopPanel() {}
    
    void paint (Graphics& g) override {}
    
    void resized() override
    {
        FlexBox fb;
        
        float centerPanelWidth = getWidth() * 0.75f * 0.45f;
        float soundPanelWidth = (getWidth() - centerPanelWidth) / 2.0f;
        
        FlexItem left (soundPanelWidth, getHeight(), leftPanel);
        FlexItem center (centerPanelWidth, getHeight(), centerPanel);
        FlexItem right (soundPanelWidth, getHeight(), rightPanel);
        
        fb.items.addArray ({left, center, right});
        fb.performLayout (getLocalBounds().toFloat());
    }
    
private:
    
    struct LeftSidePanel : public Component
    {
        LeftSidePanel ()
        {
            // Morphex Logo
            morphex_long_logo = Drawable::createFromImageData (BinaryData::morphex_long_logo_svg, BinaryData::morphex_long_logo_svgSize);
            morphex_long_logo->replaceColour (Colours::white, GUI::Color::Logo);
        }
        
        void paint (Graphics& g) override
        {
            morphex_long_logo->drawWithin (g, morphex_long_logo_bounds, RectanglePlacement::xLeft, 1.0f);
            
            // Plugin Version
            g.setColour (Colours::white);
            g.setFont (14.0f);
            g.drawText ("v" + PLUGIN_VERSION, -morphex_long_logo_margin, morphex_long_logo_margin, getWidth(), getHeight(), Justification::right);
            
            // Draw borders
            GUI::Paint::drawBorders (g, getLocalBounds());
        }
        
        void resized() override
        {
            morphex_long_logo_margin = getWidth() * 0.04f;
            
            morphex_long_logo_bounds.setBounds (morphex_long_logo_margin, morphex_long_logo_margin,
                                                getWidth() - 2 * morphex_long_logo_margin,
                                                getHeight() - 2 * morphex_long_logo_margin);
        }
        
        std::unique_ptr<Drawable> morphex_long_logo;
        Rectangle<float> morphex_long_logo_bounds;
        float morphex_long_logo_margin;
    };
    
    struct CenterPanel : public Component
    {
        CenterPanel (SpectralMorphingToolAudioProcessor* inProcessor)
        :   preset_manager_panel (inProcessor)
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
        RightSidePanel (SpectralMorphingToolAudioProcessor* inProcessor)
        {
            mSliders.clear();
            
            output_gain_slider = new Morphex::Slider (inProcessor->parameters, Morphex::Parameters::OutputGain);
            addAndMakeVisible (output_gain_slider);
            mSliders.add (output_gain_slider);
        }
        
        void paint (Graphics& g) override
        {
            // Draw borders
            GUI::Paint::drawBorders (g, getLocalBounds());
        }
        
        void resized() override
        {
            FlexBox fb;
            fb.flexDirection = FlexBox::Direction::column;
            fb.flexWrap = FlexBox::Wrap::wrap;
            fb.alignContent = FlexBox::AlignContent::flexEnd;
            fb.alignItems = FlexBox::AlignItems::flexEnd;
            fb.justifyContent = FlexBox::JustifyContent::flexEnd;
            
            FlexItem gain_knob (getHeight(), getHeight(), *mSliders[0]);
            
            fb.items.addArray ( { gain_knob } );
            fb.performLayout (getLocalBounds().toFloat());
        }
        
        Morphex::Slider* output_gain_slider;
        
        OwnedArray<Morphex::Slider> mSliders;
    };
    
    LeftSidePanel leftPanel;
    CenterPanel centerPanel;
    RightSidePanel rightPanel;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TopPanel)
};
