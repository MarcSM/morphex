/*
 ==============================================================================
 
 InterfaceDefines.h
 Created: 12 May 2020 11:40:08am
 Author:  Marc Sanchez Martinez
 
 ==============================================================================
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
        const Colour Transparent = Colour(0, 0, 0).withAlpha(0.0f);
        const Colour Accent = Colour(0, 168, 204);
        const Colour Background = Colour(34, 34, 34);
        const Colour BackgroundGradientStart = Colour(70, 70, 70).withAlpha(0.75f);
        const Colour BackgroundGradientEnd = BackgroundGradientStart.withAlpha(0.0f);
        const Colour BackgroundDark = Background.overlaidWith (Colour(0, 0, 0).withAlpha(0.25f));
        const Colour Logo = Colour(250, 250, 250);
        const Colour BrowserBackground = Colour(0, 0, 0);
        const Colour KeyDown = Colour(180, 180, 180);
    }
    
    namespace Paint
    {
        enum BorderType
        {
            Normal = 0,
            Glass
        };
        
        inline void drawBorders (Graphics& g,
                                 Rectangle<int> componentBounds,
                                 BorderType border_type = BorderType::Normal)
        {
            int line_thickness = 3;
            
            // Get borders points
            Point<int> topLeft = componentBounds.getTopLeft();
            Point<int> topRight = componentBounds.getTopRight();
            Point<int> bottomLeft = componentBounds.getBottomLeft();
            Point<int> bottomRight = componentBounds.getBottomRight();
            
            // Light color
            switch (border_type)
            {
                case BorderType::Normal: g.setColour (Colour(255, 255, 255).withAlpha(0.10f));  break;
                case BorderType::Glass:  g.setColour (Colour(255, 255, 255).withAlpha(0.15f));  break;
                default:                 jassertfalse; break;
            }
            
            // Left Border
            g.drawLine (bottomLeft.getX(), bottomLeft.getY(),
                        topLeft.getX(), topLeft.getY(), line_thickness);
            
            // Top Border
            g.drawLine (topLeft.getX(), topLeft.getY(),
                        topRight.getX(), topRight.getY(), line_thickness);
            
            // Shadow color
            g.setColour (Colour(0, 0, 0).withAlpha(0.75f));
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
        
        inline void drawDisabled (Graphics& g, Rectangle<int> componentBounds)
        {
            g.fillAll (Colours::black.withAlpha (0.25f));
        }
    }
}

const Colour SMTColour_bg = Colour(34, 34, 34);

const Colour SMTBlackColour = Colour(0, 0, 0);
const Colour SMTWhiteColour = Colour(255, 255, 255);
const Colour SMTTransparentColour = Colour(0, 0, 0).withAlpha(0.0f);

const Colour SMTColour_Logo = SMTWhiteColour;
const Colour SMTColour_Text = SMTWhiteColour;

const Colour SMTColour_1 = Colour(105, 105, 105);
const Colour SMTColour_2 = Colour(0, 0, 0).withAlpha(0.0f);
const Colour SMTColour_3 = Colour(0, 0, 0).withAlpha(0.3f);
const Colour SMTColour_4 = Colour(0, 0, 0).withAlpha(0.6f);
const Colour SMTColour_5 = Colour(105, 105, 105).withAlpha(0.8f);
const Colour SMTColour_6 = Colour(0, 0, 0).withAlpha(0.8f);
const Colour SMTColour_7 = Colour(125, 125, 125).withAlpha(0.3f);

// Button Colours
const Colour SMTHyperlinkButtonColour = SMTWhiteColour;
const Colour SMTButtonColour = SMTBlackColour;
const Colour SMTButtonOnColour = SMTColour_2;
const Colour SMTButtonTextOffColour = SMTWhiteColour;
const Colour SMTButtonTextOnColour = SMTWhiteColour;

// ComboBox Colours
const Colour SMTComboBoxBackgroundColour = SMTBlackColour;
const Colour SMTComboBoxOutlineColour = SMTColour_2;
const Colour SMTComboBoxArrowColour = SMTWhiteColour;
const Colour SMTComboBoxTextColour = SMTWhiteColour;

// Fonts
const Font font_1 ("Helvetica Neue", 12.00f, Font::bold);
const Font font_2 ("Helvetica Neue", 18.00f, Font::bold);
const Font font_3 ("Helvetica Neue", 22.00f, Font::bold);
const Font font_4 ("Helvetica Neue", 26.00f, Font::bold);
const Font font_5 ("Helvetica Neue", 28.00f, Font::bold);
const Font font_6 ("Helvetica Neue", 30.00f, Font::bold);
const Font font_7 ("Helvetica Neue", 36.00f, Font::bold);
const Font font_8 ("Helvetica Neue", 48.00f, Font::bold);
