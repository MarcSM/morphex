/*
  ==============================================================================

    SMTBottomPanel.h
    Created: 20 Jul 2019 4:54:01pm
    Author:  Marc Sanchez Martinez

  ==============================================================================
*/

#pragma once

#include "SMTPanelBase.h"

#include "SMTMidiKeyboard.h"

class SMTBottomPanel
:   public SMTPanelBase

{
public:
    
    SMTBottomPanel(SpectralMorphingToolAudioProcessor* inProcessor);
    ~SMTBottomPanel();
    
    void paint(Graphics& g) override;
    
private:
    
    SMTMidiKeyboard mMidiKeyboard;
};
