/*
  ==============================================================================

    SMTMainPanel.cpp
    Created: 14 Jun 2019 7:19:07pm
    Author:  Marc Sanchez Martinez

  ==============================================================================
*/

#include "SMTMainPanel.h"
#include "SMTParameters.h"

using namespace Core;

SMTMainPanel::SMTMainPanel(SpectralMorphingToolAudioProcessor* inProcessor)
:   SMTPanelBase(inProcessor)
{
    setSize(MAIN_PANEL_WIDTH, MAIN_PANEL_HEIGHT);
    
    mTopPanel = new SMTTopPanel(inProcessor);
    mTopPanel->setTopLeftPosition(0, 0);
    addAndMakeVisible(mTopPanel);
    
    // Sound 1 Panel
    mSound1Panel = new SMTSoundPanel(inProcessor, inProcessor->sound[1]);
    mSound1Panel->setTopLeftPosition(0, TOP_PANEL_HEIGHT);
    addAndMakeVisible(mSound1Panel);
    
    // Sound 2 Panel
    mSound2Panel = new SMTSoundPanel(inProcessor, inProcessor->sound[2]);
    mSound2Panel->setTopLeftPosition(MAIN_PANEL_WIDTH - SOUND_PANEL_WIDTH, TOP_PANEL_HEIGHT);
    addAndMakeVisible(mSound2Panel);
    
    // Center Panel
    mCenterPanel = new SMTCenterPanel(inProcessor);
    mCenterPanel->setTopLeftPosition(SOUND_PANEL_WIDTH, TOP_PANEL_HEIGHT);
    addAndMakeVisible(mCenterPanel);
    
    // Bottom Panel
    mBottomPanel = new SMTBottomPanel(inProcessor);
    mBottomPanel->setTopLeftPosition(0, MAIN_PANEL_HEIGHT - BOTTOM_PANEL_HEIGHT);
    addAndMakeVisible(mBottomPanel);
}

SMTMainPanel::~SMTMainPanel()
{
    LookAndFeel::setDefaultLookAndFeel(nullptr);
    setLookAndFeel(nullptr);
}
