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

#include "../Components/PadXY.h"

//==============================================================================
/*
*/
class CorePanel    : public Component
{
public:
    CorePanel(SpectralMorphingToolAudioProcessor* inProcessor)
    :   instrument (&inProcessor->mMorphexSynth.instrument)
    {
        // Make button toggleable
        harmonicButton  .setClickingTogglesState (true);
        sinusoidalButton.setClickingTogglesState (true);
        stochasticButton.setClickingTogglesState (true);
        attackButton    .setClickingTogglesState (true);
        residualButton  .setClickingTogglesState (true);
        
        // Core parameters default values
        harmonicButton  .setToggleState (instrument->generate.harmonic,   NotificationType::dontSendNotification);
        sinusoidalButton.setToggleState (instrument->generate.sinusoidal, NotificationType::dontSendNotification);
        stochasticButton.setToggleState (instrument->generate.stochastic, NotificationType::dontSendNotification);
        attackButton    .setToggleState (instrument->generate.attack,     NotificationType::dontSendNotification);
        residualButton  .setToggleState (instrument->generate.residual,   NotificationType::dontSendNotification);

        // Pad XY
        Morphex::Parameter freqs_interp_factor_parameter = Morphex::PARAMETERS<float>[Morphex::Parameters::freqs_interp_factor];
        Morphex::Parameter mags_interp_factor_parameter = Morphex::PARAMETERS<float>[Morphex::Parameters::mags_interp_factor];

        mPadXY = new PadXY(inProcessor, inProcessor->parameters,
                           freqs_interp_factor_parameter,
                           mags_interp_factor_parameter);
        
        addAndMakeVisible(mPadXY);
    }

    ~CorePanel()
    {
    }

    void paint (Graphics& g) override
    {
        
        g.fillAll (Colours::black);
//        g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
//        getWidth() * 0.75f , getHeight() * 0.75f, true);
        // Background gradient
        ColourGradient cg (GUI::Color::Accent.withAlpha(0.5f),
                           getWidth() * 0.5f, getHeight() * 0.5f,
                           GUI::Color::Accent.withAlpha(0.0f),
                           getWidth() * 0.65f , getHeight() * 1.0f, true);
        g.setFillType (cg);
        g.fillRect (0, 0, getWidth(), getHeight());
        
        // Core controls
        addAndMakeVisible (harmonicButton);
        addAndMakeVisible (sinusoidalButton);
        addAndMakeVisible (stochasticButton);
        addAndMakeVisible (attackButton);
        addAndMakeVisible (residualButton);
        harmonicButton  .onClick = [this] { updateParameter (harmonicButton.getToggleState(),   instrument->generate.harmonic);   };
        sinusoidalButton.onClick = [this] { updateParameter (sinusoidalButton.getToggleState(), instrument->generate.sinusoidal); };
        stochasticButton.onClick = [this] { updateParameter (stochasticButton.getToggleState(), instrument->generate.stochastic); };
        attackButton    .onClick = [this] { updateParameter (attackButton.getToggleState(),     instrument->generate.attack);     };
        residualButton  .onClick = [this] { updateParameter (residualButton.getToggleState(),   instrument->generate.residual);   };
        
        // Pad XY
        
        // Circle glow
        PadXY::PadCircle* circle = mPadXY->getCircle();
        float glow_radius = circle->getWidth() * 2.0f;
        float glow_x = circle->getX() + ( circle->getWidth() / 2.0f ) + mPadXY->getX();
        float glow_y = circle->getY() + ( circle->getHeight() / 2.0f ) + mPadXY->getY();
        float glow_w = glow_radius * 2.0f;
        float glow_h = glow_radius * 2.0f;
        
        ColourGradient cg_glow (GUI::Color::Accent.withAlpha(0.3f),
                                glow_x, glow_y,
                                GUI::Color::Accent.withAlpha(0.0f),
                                glow_x + (glow_radius * 0.4f),
                                glow_y + (glow_radius * 0.4f), true);
        
        g.setFillType (cg_glow);
        g.fillRect (0, 0, getWidth(), getHeight());
//        g.fillRect (glow_x - glow_radius, glow_y - glow_radius, glow_w, glow_h);
    }

    void resized() override
    {
        // Sizes
        const float pad_width = getWidth();
        const float pad_height = getWidth();
        
        const float pad_x = 0;
        const float pad_y = 0;
//        const int pad_y = ( getHeight() - pad_height ) / 2;
        
        // Pad XY
        mPadXY->setBounds (pad_x, pad_y, pad_width, pad_height);
        
        const int num_buttons = 5;
        const float button_width = getWidth() / num_buttons;
        const float button_height = ( getHeight() - pad_height ) / 2;

        // Core controls
        const int core_controls_y = pad_y + pad_height;
        
        harmonicButton  .setBounds (0,                core_controls_y, button_width, button_height);
        sinusoidalButton.setBounds (button_width,     core_controls_y, button_width, button_height);
        stochasticButton.setBounds (2 * button_width, core_controls_y, button_width, button_height);
        attackButton    .setBounds (3 * button_width, core_controls_y, button_width, button_height);
        residualButton  .setBounds (4 * button_width, core_controls_y, button_width, button_height);
    }
    
    void updateToggleState (Button* button, String name)
    {
        auto state = button->getToggleState();
        String stateString    = state ? "ON" : "OFF";
        String selectedString = state ? " (selected)" : "";
        
        Logger::outputDebugString (name + " Button changed to " + stateString);
        button->setButtonText (name + selectedString);
    }
    
    template<typename T>
    void updateParameter (T new_value, T &parameter)
    {
        parameter = new_value;
    }
    
    void updateToggleCoreParameter (Button* button, bool &core_parameter)
    {
        core_parameter = button->getToggleState();
    }

private:
    
    Instrument* instrument;
    
    TextButton harmonicButton   {"Harmonics"},
               sinusoidalButton {"Sinusoidal"},
               stochasticButton {"Stochastic"},
               attackButton     {"Attack"},
               residualButton   {"Residual"};

    PadXY* mPadXY;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CorePanel)
};
