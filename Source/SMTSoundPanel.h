/*
  ==============================================================================

    SMTSoundPanel.h
    Created: 19 Jul 2019 11:52:52am
    Author:  Marc Sanchez Martinez

  ==============================================================================
*/

#pragma once

#include "SMTPanelBase.h"
#include "SMTWaveformPreview.h"

class SMTSoundPanel
:   public SMTPanelBase
{
public:
    
    SMTSoundPanel(SpectralMorphingToolAudioProcessor* inProcessor, std::unique_ptr<Sound>& sound);
    ~SMTSoundPanel();
    
    void paint(Graphics& g) override;
    
private:
    
    SpectralMorphingToolAudioProcessor* mProcessor;
    
    ScopedPointer<SMTWaveformPreview> mWaveformPreview;
};
