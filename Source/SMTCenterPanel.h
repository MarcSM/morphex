/*
  ==============================================================================

    SMTCenterPanel.h
    Created: 14 Jun 2019 7:38:52pm
    Author:  Marc Sanchez Martinez

  ==============================================================================
*/

#pragma once

#include "SMTPanelBase.h"
#include "SMTSoundPanel.h"

#include "SMTParameterSlider.h"

#include "SMTBrowser.h"
#include "SMTAdditionalParameters.h"
#include "SMTADSRParameters.h"
#include "SMTTabbedComponent.h"

#include "SMTXYPad.h"

class SMTCenterPanel
:   public SMTPanelBase
{
public:
    
    SMTCenterPanel();
    SMTCenterPanel(SpectralMorphingToolAudioProcessor* inProcessor);
    ~SMTCenterPanel();
    
    void paint(Graphics& g) override;
    
private:
    
    OwnedArray<SMTParameterSlider> mSliders;
    
    ScopedPointer<SMTBrowser> mBrowser;
    ScopedPointer<SMTADSRParameters> mADSRParameters;
    ScopedPointer<SMTAdditionalParameters> mAdditionalParameters;

    ScopedPointer<SMTTabbedComponent> mTabbedComponent;
    
    ScopedPointer<SMTXYPad> mXYPad;
};
