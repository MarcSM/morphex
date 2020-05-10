/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 5.4.7

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) 2017 - ROLI Ltd.

  ==============================================================================
*/

#pragma once

//[Headers]     -- You can add your own extra header files here --
#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "SMTInterfaceDefines.h"
#include "SMTPanelBase.h"
#include "SMTTopPanel.h"
#include "SMTSoundPanel.h"
#include "SMTCenterPanel.h"
#include "SMTBottomPanel.h"
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class MainPanel  : public Component
{
public:
    //==============================================================================
    MainPanel (SpectralMorphingToolAudioProcessor* inProcessor);
    ~MainPanel() override;

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    //[/UserMethods]

    void paint (Graphics& g) override;
    void resized() override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    SMTTopPanel mTopPanel;
//    SMTSoundPanel mSound1Panel;
//    SMTSoundPanel mSound2Panel;
    SMTCenterPanel mCenterPanel;
    SMTBottomPanel mBottomPanel;
    //[/UserVariables]

    //==============================================================================


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainPanel)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

