/*
  ==============================================================================

    SMTMainPanel.h
    Created: 14 Jun 2019 7:19:07pm
    Author:  Marc Sanchez Martinez

  ==============================================================================
*/

#pragma once

#include "SMTPanelBase.h"
#include "SMTTopPanel.h"
#include "SMTSoundPanel.h"
#include "SMTCenterPanel.h"
#include "SMTBottomPanel.h"

class SMTMainPanel
:   public SMTPanelBase
{
public:
    
    SMTMainPanel(SpectralMorphingToolAudioProcessor* inProcessor);
    ~SMTMainPanel();
    
private:
    
    ScopedPointer<SMTTopPanel> mTopPanel;
    ScopedPointer<SMTSoundPanel> mSound1Panel;
    ScopedPointer<SMTSoundPanel> mSound2Panel;
    ScopedPointer<SMTCenterPanel> mCenterPanel;
    ScopedPointer<SMTBottomPanel> mBottomPanel;
};
