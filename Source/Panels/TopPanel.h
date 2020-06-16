/*
 ==============================================================================
 
 TopPanel.h
 Created: 12 May 2020 11:18:41am
 Author:  Marc Sanchez Martinez
 
 ==============================================================================
 */

#pragma once

#include <JuceHeader.h>

#include "PresetManagerPanel.h"

//==============================================================================
/*
 */
class TopPanel    : public Component
{
public:
    TopPanel(SpectralMorphingToolAudioProcessor* inProcessor)
    :   rightPanel (inProcessor),
        centerPanel (inProcessor)
    {
        addAndMakeVisible (rightPanel);
        addAndMakeVisible (centerPanel);
        addAndMakeVisible (leftPanel);
    }
    
    ~TopPanel()
    {
    }
    
    void paint (Graphics& g) override
    {
        
    }
    
    void resized() override
    {
        FlexBox fb;
        
        float centerPanelWidth = getWidth() * 0.75f * 0.45f;
        float soundPanelWidth = ( getWidth() - centerPanelWidth ) / 2.0f;
        
        FlexItem left  (soundPanelWidth, getHeight(), leftPanel);
        FlexItem center  (centerPanelWidth, getHeight(), centerPanel);
        FlexItem right (soundPanelWidth, getHeight(), rightPanel);
        
        fb.items.addArray ( { left, center, right } );
        fb.performLayout (getLocalBounds().toFloat());
    }
    
private:
    
    struct LeftSidePanel    : public Component
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
            g.drawText ("v" + PLUGIN_VERSION + "-beta", -morphex_long_logo_margin, morphex_long_logo_margin, getWidth(), getHeight(), Justification::right);
            
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
    
    struct CenterPanel    : public Component
    {
        CenterPanel (SpectralMorphingToolAudioProcessor* inProcessor)
        :   preset_manager_panel (inProcessor)
        {
            //            preset_manager_panel.setBounds (0, 0, getWidth(), getHeight());
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
            
            Morphex::Parameter output_gain_parameter = Morphex::PARAMETERS<float>[Morphex::Parameters::OutputGain];
            
            Slider* output_gain_slider = new Slider (output_gain_parameter.parameter_label);
            
            new AudioProcessorValueTreeState::SliderAttachment (inProcessor->parameters, output_gain_parameter.parameter_ID, *output_gain_slider);
            
            // TODO - USe custom slider class
            // Add ADSR attack slider
//            asdr_attack = new SMTParameterSlider(mProcessor->parameters,
//                                                 SMTParameterID[SMTParameter::kParameter_asdr_attack],
//                                                 SMTParameterLabel[SMTParameter::kParameter_asdr_attack]);
//            output_gain_slider->setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxBelow, false, 200, 20);
//            output_gain_slider->setSliderStyle(Slider::SliderStyle::RotaryHorizontalVerticalDrag);
//            output_gain_slider->setBounds(slider_x, slider_y, slider_w, slider_h);
            
            output_gain_slider->setSliderStyle (Slider::SliderStyle::RotaryHorizontalVerticalDrag);
            output_gain_slider->setTextBoxStyle (Slider::NoTextBox, true, 0, 0);
//            output_gain_slider->setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxBelow, false, 100, 10);
            output_gain_slider->setRange (output_gain_parameter.min_value, output_gain_parameter.max_value, 0.001f);
            output_gain_slider->setValue (output_gain_parameter.default_value);
            
            mSliders.add (output_gain_slider);
            
            addAndMakeVisible (output_gain_slider);
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
        
        OwnedArray<Slider> mSliders;
    };
    
    LeftSidePanel leftPanel;
    CenterPanel centerPanel;
    RightSidePanel rightPanel;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TopPanel)
};
