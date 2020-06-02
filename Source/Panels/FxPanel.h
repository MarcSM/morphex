/*
  ==============================================================================

    FxPanel.h
    Created: 1 Jun 2020 12:24:39pm
    Author:  Marc Sanchez Martinez

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class FxPanel : public Component
{
public:
    FxPanel (SpectralMorphingToolAudioProcessor* inProcessor)
    {
        // In your constructor, you should add any child components, and
        // initialise any special settings that your component needs.
        
//        FxRow* distortion_panel = new FxRow (inProcessor, "Distortion");
//        addAndMakeVisible (mFxRows.add (distortion_panel));
//        
//        FxRow* delay_panel = new FxRow (inProcessor, "Delay");
//        addAndMakeVisible (mFxRows.add (delay_panel));
        
        // Reverb FX
        FxRow* reverb_panel = new FxRow (inProcessor, "Reverb");
        addAndMakeVisible (mFxRows.add (reverb_panel));
        
        // Reverb FX - Dry / Wet
        Morphex::Parameter reverb_drywet_parameter = Morphex::PARAMETERS<float>[Morphex::Parameters::ReverbDryWet];
        Slider* reverb_drywet_slider = new Slider (reverb_drywet_parameter.parameter_label);
        new AudioProcessorValueTreeState::SliderAttachment (inProcessor->parameters, reverb_drywet_parameter.parameter_ID, *reverb_drywet_slider);
        reverb_drywet_slider->setSliderStyle (Slider::SliderStyle::RotaryHorizontalVerticalDrag);
        reverb_drywet_slider->setTextBoxStyle (Slider::NoTextBox, true, 0, 0);
//            reverb_drywet_slider->setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxBelow, false, 100, 10);
        reverb_drywet_slider->setRange(reverb_drywet_parameter.min_value, reverb_drywet_parameter.max_value, 0.001f);
        reverb_drywet_slider->setValue(reverb_drywet_parameter.default_value);
        addAndMakeVisible (reverb_panel->mSliders.add (reverb_drywet_slider));
    }

    ~FxPanel()
    {
    }

    void paint (Graphics& g) override
    {
        g.fillAll (Colours::black);
        
        // Draw borders
        GUI::Paint::drawBorders(g, getLocalBounds());
    }

    void resized() override
    {
        auto bounds = getLocalBounds();
        auto row_height = bounds.getHeight() * 0.18;
        
        for (int i = 0; i < mFxRows.size(); ++i)
        {
//            mFxRows[i]->setBounds (row_height * (i % 3),
//            bounds.getHeight() / 2 * (i / 3),
            mFxRows[i]->setBounds (0, row_height * i, getWidth(), row_height);
        }
    }

private:
    
    struct FxRow : public Component
    {
        FxRow (SpectralMorphingToolAudioProcessor* inProcessor, String name)
        :   name (name)
        {
            mSliders.clear();
            
//            const int margin = 10;
//
//            const int slider_w = 80;
//            const int slider_h = 80;
//
//            int slider_x = 30;
//            int slider_y = margin + 25;
//
//            Morphex::Parameter output_gain_parameter = Morphex::PARAMETERS<float>[Morphex::Parameters::OutputGain];
//            //            Morphex::Parameter output_gain_parameter *mParameters->getRawParameterValue (Morphex::PARAMETERS<float>[Morphex::Parameters::OutputGain]);
//
//            //            auto* slider = new Slider();
//            //            slider->setSliderStyle (Slider::SliderStyle::Rotary);
//            //            slider->setTextBoxStyle (Slider::NoTextBox, true, 0, 0);
//            //
//            //            addAndMakeVisible (knobs.add (slider));
//
//            Slider* output_gain_slider = new Slider (output_gain_parameter.parameter_label);
//
//            new AudioProcessorValueTreeState::SliderAttachment (inProcessor->parameters, output_gain_parameter.parameter_ID, *output_gain_slider);
//
//            // Add ADSR attack slider
//            //            asdr_attack = new SMTParameterSlider(mProcessor->parameters,
//            //                                                 SMTParameterID[SMTParameter::kParameter_asdr_attack],
//            //                                                 SMTParameterLabel[SMTParameter::kParameter_asdr_attack]);
//            //            output_gain_slider->setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxBelow, false, 200, 20);
//            //            output_gain_slider->setSliderStyle(Slider::SliderStyle::RotaryHorizontalVerticalDrag);
//            //            output_gain_slider->setBounds(slider_x, slider_y, slider_w, slider_h);
//
//            output_gain_slider->setSliderStyle (Slider::SliderStyle::RotaryHorizontalVerticalDrag);
//            output_gain_slider->setTextBoxStyle (Slider::NoTextBox, true, 0, 0);
//            //            output_gain_slider->setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxBelow, false, 100, 10);
//            output_gain_slider->setRange(output_gain_parameter.min_value, output_gain_parameter.max_value, 0.001f);
//            output_gain_slider->setValue(output_gain_parameter.default_value);
//
//            mSliders.add (output_gain_slider);
//
//            addAndMakeVisible (output_gain_slider);
        }
        
        void paint (Graphics& g) override
        {
            g.fillAll (GUI::Color::BackgroundDark);
            
            /*
             g.setColour (Colours::grey);
             g.drawRect (getLocalBounds(), 1);   // draw an outline around the component
             
             g.setColour (Colours::white);
             g.setFont (14.0f);
             g.drawText ("Options", getLocalBounds(),
             Justification::centred, true);   // draw some placeholder
             */
            
            g.setColour (Colours::white);
            g.setFont (14.0f);
            g.drawText (name, getLocalBounds(), Justification::topLeft, true);
//            g.drawText (name, getLocalBounds(), Justification::topLeft, false);
            
            // Draw borders
            GUI::Paint::drawBorders(g, getLocalBounds());
        }
        
        void resized() override
        {
//            float row_height = getHeight() * 0.2;
            
            FlexBox fb;
            fb.flexDirection = FlexBox::Direction::column;
            fb.flexWrap = FlexBox::Wrap::wrap;
            fb.justifyContent = FlexBox::JustifyContent::flexEnd;
            //            fb.justifyContent = FlexBox::JustifyContent::spaceBetween;
            
            //            float gain_knob_width = ( getWidth() / 3.0f ) * 2.0f;
            //
            //            FlexItem gain_knob (gain_knob_width, getHeight(), *mSliders[0]);
            //            FlexItem vu_meter (getWidth() - gain_knob_width, getHeight(), *new Component());
            
//            FlexItem gain_knob (getWidth(), row_height, *mSliders[0]);
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
        
        String name;
        
        OwnedArray<Slider> mSliders;
    };
    
    OwnedArray<FxRow> mFxRows;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FxPanel)
};
