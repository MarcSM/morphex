/*
  ==============================================================================

    SMTBottomPanel.cpp
    Created: 20 Jul 2019 4:54:01pm
    Author:  Marc Sanchez Martinez

  ==============================================================================
*/

#include "SMTBottomPanel.h"

#include "SMTConstants.h"

SMTBottomPanel::SMTBottomPanel(SpectralMorphingToolAudioProcessor* inProcessor)
:   SMTPanelBase(inProcessor),
    mMidiKeyboard(inProcessor->getMidiState())
{
    setSize(BOTTOM_PANEL_WIDTH, BOTTOM_PANEL_HEIGHT);

    int side_margin = 1;
    int midi_keyboard_x = 0 + side_margin;
    int midi_keyboard_y = 0;
    int midi_keyboard_width = getWidth() - (side_margin * 2);
    int midi_keyboard_height = getHeight();

    mMidiKeyboard.setBounds(midi_keyboard_x, midi_keyboard_y,
                       midi_keyboard_width, midi_keyboard_height);

    addAndMakeVisible(mMidiKeyboard);
    
    repaint();
}

SMTBottomPanel::~SMTBottomPanel()
{
}

void SMTBottomPanel::paint (Graphics& g)
{
    SMTPanelBase::paint(g);
}
