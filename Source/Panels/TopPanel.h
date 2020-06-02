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
        addAndMakeVisible (leftPanel);
        addAndMakeVisible (centerPanel);
    }
    
    ~TopPanel()
    {
    }
    
    void paint (Graphics& g) override
    {
        
    }
    
    void resized() override
    {
        // This method is where you should set the bounds of any child
        // components that your component contains..
        
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
            //morphex_long_logo = ImageCache::getFromMemory(BinaryData::morphex_long_logo_svg, BinaryData::morphex_long_logo_svgSize);
            morphex_long_logo = Drawable::createFromImageData (BinaryData::morphex_long_logo_svg, BinaryData::morphex_long_logo_svgSize);
            //addAndMakeVisible (morphex_long_logo.get());
            //morphex_long_logo->setBounds (0,0,100,100);
            
            morphex_long_logo->replaceColour (Colours::white, GUI::Color::Logo);
            
            //morphex_long_logo_bounds = new Rectangle<float>(10, 10, getWidth(), getHeight());
            //morphex_long_logo_bounds->setBounds(10, 10, 100, 100);

            
        }
        
        void paint (Graphics& g) override
        {
            // g.drawImageAt(morphex_long_logo, morphex_long_logo_margin, morphex_long_logo_margin);
            
            
//            auto tr = morphex_long_logo->getTransformToScaleToFit(getLocalBounds().toFloat().reduced(3.5), false, Justification::centred);
//            morphex_long_logo.applyTransform(tr);
            
//            auto area = getLocalBounds();
//            morphex_long_logo->setBounds(area);
            //morphex_long_logo->draw(g, 1.f, AffineTransform());
            
//            juce::Rectangle<float> morphex_long_logo_bounds(10, 10, 100, 100);
            
//            g.drawRect(morphex_long_logo_bounds.expanded(1.f), 1.f);
            // svg file:
//            morphex_long_logo->drawWithin(g, morphex_long_logo_bounds, RectanglePlacement::stretchToFit, 1.f);
            
//            juce::Rectangle<float> picBounds(10, 10, getWidth(), getHeight());
            
//            juce::Rectangle<float> picBounds(morphex_long_logo_margin, morphex_long_logo_margin,
//                                             getWidth() - 2 * morphex_long_logo_margin,
//                                             getHeight() - 2 * morphex_long_logo_margin);
            
            
            
            
//            juce::Rectangle<float> svgBounds =
//            ((DrawableComposite*) morphex_long_logo.get())->getContentArea();
//            juce::RectanglePlacement ourPlacement(RectanglePlacement::xLeft);
//            juce::AffineTransform xform = ourPlacement.getTransformToFit (svgBounds, *picBounds);
//            morphex_long_logo->draw(g, 1.f, xform);
            
            
            morphex_long_logo->drawWithin(g, picBounds, RectanglePlacement::xLeft, 1.0f);
//            morphex_long_logo->drawWithin(g, *picBounds, RectanglePlacement::centred + RectanglePlacement::doNotResize, 1.0f);

            // drawToFit(g, morphex_long_logo, morphex_long_logo_bounds);
            
            // Plugin Version
            g.setColour (Colours::white);
            g.setFont (14.0f);
            g.drawText("v" + PLUGIN_VERSION + "-beta", -morphex_long_logo_margin, morphex_long_logo_margin, getWidth(), getHeight(), Justification::right);
//            g.drawText("v" + PLUGIN_VERSION, -15, getHeight() -30, getWidth(), getHeight(), Justification::right);
//            g.drawText("v" + PLUGIN_VERSION, -12, getHeight() * 0.2, getWidth(), getHeight(), Justification::right);
            
            // Draw borders
            GUI::Paint::drawBorders (g, getLocalBounds());
        }
        
//        static void drawToFit (juce::Graphics& g, juce::Drawable& svg, juce::Rectangle<float> targetArea)
//        {
//            svg.draw (g, 1.0f,
//                      juce::RectanglePlacement (juce::RectanglePlacement::stretchToFit)
//                      .getTransformToFit ((std::unique_ptr<DrawableComposite>) svg).getContentArea(), targetArea));
//        }
        
//        static void drawToFit (juce::Graphics& g, juce::DrawableComposite& svg, juce::Rectangle<float> targetArea)
//        {
//            svg.draw (g, 1.0f,
//                      juce::RectanglePlacement (juce::RectanglePlacement::stretchToFit)
//                      .getTransformToFit (svg.getContentArea(), targetArea));
//        }
        
        void resized() override
        {
            morphex_long_logo_margin = getWidth() * 0.04f;
//            morphex_long_logo_margin = 15.0f;
            picBounds.setBounds(morphex_long_logo_margin, morphex_long_logo_margin,
                                getWidth() - 2 * morphex_long_logo_margin,
                                getHeight() - 2 * morphex_long_logo_margin);
            
            //morphex_long_logo_bounds->setBounds(10, 10, 100, 100);
            
            //auto area = getLocalBounds();
            //morphex_long_logo->setBounds(0,0,100,100);
            
            /*
            morphex_long_logo = morphex_long_logo.rescaled (getWidth() - 2 * morphex_long_logo_margin - 130,
                                                            getHeight() - 2 * morphex_long_logo_margin,
                                                            Graphics::mediumResamplingQuality);
            */
        }
        
        std::unique_ptr<Drawable> morphex_long_logo;
        Rectangle<float> picBounds;
        //Rectangle<float>* morphex_long_logo_bounds;
//        std::unique_ptr<Rectangle<float>> morphex_long_logo_bounds;
//        Image morphex_long_logo;
        float morphex_long_logo_margin;
    };
    
    struct RightSidePanel : public Component
    {
        RightSidePanel (SpectralMorphingToolAudioProcessor* inProcessor)
        {
            mSliders.clear();
            
            const int margin = 10;
            
            const int slider_w = 80;
            const int slider_h = 80;
            
            int slider_x = 30;
            int slider_y = margin + 25;
            
            Morphex::Parameter output_gain_parameter = Morphex::PARAMETERS<float>[Morphex::Parameters::OutputGain];
//            Morphex::Parameter output_gain_parameter *mParameters->getRawParameterValue (Morphex::PARAMETERS<float>[Morphex::Parameters::OutputGain]);
            
//            auto* slider = new Slider();
//            slider->setSliderStyle (Slider::SliderStyle::Rotary);
//            slider->setTextBoxStyle (Slider::NoTextBox, true, 0, 0);
//
//            addAndMakeVisible (knobs.add (slider));
            
            Slider* output_gain_slider = new Slider (output_gain_parameter.parameter_label);
            
            new AudioProcessorValueTreeState::SliderAttachment (inProcessor->parameters, output_gain_parameter.parameter_ID, *output_gain_slider);
            
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
            output_gain_slider->setRange(output_gain_parameter.min_value, output_gain_parameter.max_value, 0.001f);
            output_gain_slider->setValue(output_gain_parameter.default_value);
            
            mSliders.add (output_gain_slider);
            
            addAndMakeVisible (output_gain_slider);
        }
        
        void paint (Graphics& g) override
        {
            /*
             g.setColour (Colours::grey);
             g.drawRect (getLocalBounds(), 1);   // draw an outline around the component
             
             g.setColour (Colours::white);
             g.setFont (14.0f);
             g.drawText ("Options", getLocalBounds(),
             Justification::centred, true);   // draw some placeholder
             */
            
            // Draw borders
            GUI::Paint::drawBorders(g, getLocalBounds());
        }
        
        void resized() override
        {
            FlexBox fb;
            fb.flexDirection = FlexBox::Direction::column;
            fb.flexWrap = FlexBox::Wrap::wrap;
            fb.justifyContent = FlexBox::JustifyContent::flexEnd;
//            fb.justifyContent = FlexBox::JustifyContent::spaceBetween;

//            float gain_knob_width = ( getWidth() / 3.0f ) * 2.0f;
//
//            FlexItem gain_knob (gain_knob_width, getHeight(), *mSliders[0]);
//            FlexItem vu_meter (getWidth() - gain_knob_width, getHeight(), *new Component());
            
            FlexItem gain_knob (getWidth(), getHeight(), *mSliders[0]);
            
//            fb.items.addArray ( { gain_knob, vu_meter } );
            fb.items.addArray ( { gain_knob } );
            fb.performLayout (getLocalBounds().toFloat());
            
//            Grid grid;
//
//            float grid_margin = getWidth() * 0.04f;
//
//            grid.rowGap    = Grid::Px (grid_margin);;
//            grid.columnGap = Grid::Px (grid_margin);;
//
//            using Track = Grid::TrackInfo;
//
//            grid.templateRows    = { Track (1_fr), Track (1_fr) };
//            grid.templateColumns = { Track (1_fr), Track (1_fr), Track (1_fr) };
//
//            grid.autoColumns = Track (1_fr);
//            grid.autoRows    = Track (1_fr);
//
//            grid.items.addArray ({
//                GridItem (mSliders[0]).withArea (1, 1, 2, 2),
//                GridItem (new Component()).withArea (1, 3, 2, 3),
//            });
//
////            grid.items = { GridItem (mSliders), GridItem (new Component()) };
//
//            grid.justifyContent = Grid::JustifyContent::spaceBetween;
//            grid.justifyItems = Grid::JustifyItems::stretch;
//            grid.alignContent = Grid::AlignContent::spaceAround;
//
////            Rectangle<int> grid_bounds(grid_margin, grid_margin,
////                                       getWidth() - ( grid_margin * 2.0f ),
////                                       getHeight() - ( grid_margin * 2.0f ));
////            grid.performLayout (grid_bounds);
//
//            grid.performLayout (getLocalBounds());
        }
        
        OwnedArray<Slider> mSliders;
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
            
            //g.setColour (Colours::grey);
            //g.drawRect (getLocalBounds(), 1);   // draw an outline around the component
            
//            g.setColour (Colours::white);
//            g.setFont (14.0f);
//            g.drawText ("Preset Manager", getLocalBounds(),
//                        Justification::centred, true);   // draw some placeholder text
            
//            // Draw borders
//            GUI::Paint::drawBorders (g, getLocalBounds());
        }
        
        void resized() override
        {
            auto area = getLocalBounds();
            preset_manager_panel.setBounds (area);
        }
        
        PresetManagerPanel preset_manager_panel;
    };
    
    LeftSidePanel leftPanel;
    RightSidePanel rightPanel;
    CenterPanel centerPanel;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TopPanel)
};
