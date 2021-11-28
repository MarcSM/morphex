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

#include "JuceHeader.h"

#define MORPHEX_PANEL_WIDTH   1000
#define MORPHEX_PANEL_HEIGHT  600
#define MORPHEX_RATIO         double(MORPHEX_PANEL_WIDTH) / double(MORPHEX_PANEL_HEIGHT)

#define TOP_PANEL_WIDTH     MORPHEX_PANEL_WIDTH
#define TOP_PANEL_HEIGHT    50

#define BOTTOM_PANEL_WIDTH  MORPHEX_PANEL_WIDTH
#define BOTTOM_PANEL_HEIGHT 50

#define SOUND_PANEL_WIDTH   250
#define SOUND_PANEL_HEIGHT  MORPHEX_PANEL_HEIGHT - TOP_PANEL_HEIGHT - BOTTOM_PANEL_HEIGHT

#define CENTER_PANEL_WIDTH  MORPHEX_PANEL_WIDTH - (2 * SOUND_PANEL_WIDTH)
#define CENTER_PANEL_HEIGHT SOUND_PANEL_HEIGHT

// General
namespace GUI
{
    namespace Color
    {
        const juce::Colour Transparent = juce::Colour(0, 0, 0).withAlpha(0.0f);
        const juce::Colour Accent = juce::Colour(0, 168, 204);
        const juce::Colour Background = juce::Colour(34, 34, 34);
        const juce::Colour BackgroundGradientStart = juce::Colour(70, 70, 70).withAlpha(0.75f);
        const juce::Colour BackgroundGradientEnd = BackgroundGradientStart.withAlpha(0.0f);
        const juce::Colour BackgroundDark = Background.overlaidWith (juce::Colour(0, 0, 0).withAlpha(0.25f));
        const juce::Colour Logo = juce::Colour(250, 250, 250);
        const juce::Colour BrowserBackground = juce::Colour(0, 0, 0);
        const juce::Colour KeyDown = juce::Colour(180, 180, 180);
    }
    
    namespace Paint
    {
        enum BorderType
        {
            Normal = 0,
            Glass
        };
        
        inline void drawBorders (juce::Graphics& g,
                                 juce::Rectangle<int> componentBounds,
                                 BorderType border_type = BorderType::Normal)
        {
            int line_thickness = 3;
            
            // Get borders points
            juce::Point<int> topLeft = componentBounds.getTopLeft();
            juce::Point<int> topRight = componentBounds.getTopRight();
            juce::Point<int> bottomLeft = componentBounds.getBottomLeft();
            juce::Point<int> bottomRight = componentBounds.getBottomRight();
            
            // Light color
            switch (border_type)
            {
                case BorderType::Normal: g.setColour (juce::Colour(255, 255, 255).withAlpha(0.10f));  break;
                case BorderType::Glass:  g.setColour (juce::Colour(255, 255, 255).withAlpha(0.15f));  break;
                default:                 jassertfalse; break;
            }
            
            // Left Border
            g.drawLine (bottomLeft.getX(), bottomLeft.getY(),
                        topLeft.getX(), topLeft.getY(), line_thickness);
            
            // Top Border
            g.drawLine (topLeft.getX(), topLeft.getY(),
                        topRight.getX(), topRight.getY(), line_thickness);
            
            // Shadow color
            g.setColour (juce::Colour(0, 0, 0).withAlpha(0.75f));
//            switch (border_type)
//            {
//                case BorderType::Normal: g.setColour (Colour(0, 0, 0).withAlpha(0.75f));        break;
//                case BorderType::Glass:  g.setColour (Colour(255, 255, 255).withAlpha(0.10f));  break;
//                default:                 jassertfalse; break;
//            }
            
            // Right Border
            g.drawLine (topRight.getX(), topRight.getY(),
                        bottomRight.getX(), bottomRight.getY(), line_thickness);
            
            // Bottom Border
            g.drawLine (bottomRight.getX(), bottomRight.getY(),
                        bottomLeft.getX(), bottomLeft.getY(), line_thickness);
            
            //g.drawRect (componentBounds, 1);   // draw an outline around the component
        }
        
        inline void drawDisabled (juce::Graphics& g, juce::Rectangle<int> componentBounds)
        {
            g.fillAll (juce::Colours::black.withAlpha (0.25f));
        }
    }
}

const juce::Colour SMTColour_bg = juce::Colour(34, 34, 34);

const juce::Colour SMTBlackColour = juce::Colour(0, 0, 0);
const juce::Colour SMTWhiteColour = juce::Colour(255, 255, 255);
const juce::Colour SMTTransparentColour = juce::Colour(0, 0, 0).withAlpha(0.0f);

const juce::Colour SMTColour_Logo = SMTWhiteColour;
const juce::Colour SMTColour_Text = SMTWhiteColour;

const juce::Colour SMTColour_1 = juce::Colour(105, 105, 105);
const juce::Colour SMTColour_2 = juce::Colour(0, 0, 0).withAlpha(0.0f);
const juce::Colour SMTColour_3 = juce::Colour(0, 0, 0).withAlpha(0.3f);
const juce::Colour SMTColour_4 = juce::Colour(0, 0, 0).withAlpha(0.6f);
const juce::Colour SMTColour_5 = juce::Colour(105, 105, 105).withAlpha(0.8f);
const juce::Colour SMTColour_6 = juce::Colour(0, 0, 0).withAlpha(0.8f);
const juce::Colour SMTColour_7 = juce::Colour(125, 125, 125).withAlpha(0.3f);

// Button Colours
const juce::Colour SMTHyperlinkButtonColour = SMTWhiteColour;
const juce::Colour SMTButtonColour = SMTBlackColour;
const juce::Colour SMTButtonOnColour = SMTColour_2;
const juce::Colour SMTButtonTextOffColour = SMTWhiteColour;
const juce::Colour SMTButtonTextOnColour = SMTWhiteColour;

// ComboBox Colours
const juce::Colour SMTComboBoxBackgroundColour = SMTBlackColour;
const juce::Colour SMTComboBoxOutlineColour = SMTColour_2;
const juce::Colour SMTComboBoxArrowColour = SMTWhiteColour;
const juce::Colour SMTComboBoxTextColour = SMTWhiteColour;

// Fonts
const juce::Font font_1 ("Helvetica Neue", 12.00f, juce::Font::bold);
const juce::Font font_2 ("Helvetica Neue", 18.00f, juce::Font::bold);
const juce::Font font_3 ("Helvetica Neue", 22.00f, juce::Font::bold);
const juce::Font font_4 ("Helvetica Neue", 26.00f, juce::Font::bold);
const juce::Font font_5 ("Helvetica Neue", 28.00f, juce::Font::bold);
const juce::Font font_6 ("Helvetica Neue", 30.00f, juce::Font::bold);
const juce::Font font_7 ("Helvetica Neue", 36.00f, juce::Font::bold);
const juce::Font font_8 ("Helvetica Neue", 48.00f, juce::Font::bold);
