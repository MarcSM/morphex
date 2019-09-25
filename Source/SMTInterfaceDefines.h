/*
  ==============================================================================

    SMTInterfaceDefines.h
    Created: 14 Jun 2019 7:42:29pm
    Author:  Marc Sanchez Martinez

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"

#define MAIN_PANEL_WIDTH    900
#define MAIN_PANEL_HEIGHT   700

#define TOP_PANEL_WIDTH     MAIN_PANEL_WIDTH
#define TOP_PANEL_HEIGHT    45

#define BOTTOM_PANEL_WIDTH  MAIN_PANEL_WIDTH
#define BOTTOM_PANEL_HEIGHT 65

#define SOUND_PANEL_WIDTH   250
#define SOUND_PANEL_HEIGHT  MAIN_PANEL_HEIGHT - TOP_PANEL_HEIGHT - BOTTOM_PANEL_HEIGHT

#define CENTER_PANEL_WIDTH  MAIN_PANEL_WIDTH - (2 * SOUND_PANEL_WIDTH)
#define CENTER_PANEL_HEIGHT SOUND_PANEL_HEIGHT

// General
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
