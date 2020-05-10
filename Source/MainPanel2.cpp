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

//[Headers] You can add your own extra header files here...
//[/Headers]

#include "MainPanel2.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
MainPanel::MainPanel (SpectralMorphingToolAudioProcessor* inProcessor)
    : mTopPanel (inProcessor),
      mCenterPanel (inProcessor),
      mBottomPanel (inProcessor)
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]


    //[UserPreSize]
    //[/UserPreSize]

    setSize (1000, 700);


    //[Constructor] You can add your own custom stuff here..

    // Top Panel
//    mTopPanel = new SMTTopPanel(inProcessor);
    mTopPanel.setTopLeftPosition(0, 0);
    addAndMakeVisible(mTopPanel);

    // Center Panel
//    mCenterPanel = new SMTCenterPanel(inProcessor);
    mCenterPanel.setTopLeftPosition(SOUND_PANEL_WIDTH, TOP_PANEL_HEIGHT);
    addAndMakeVisible(mCenterPanel);

    // Bottom Panel
//    mBottomPanel = new SMTBottomPanel(inProcessor);
    mBottomPanel.setTopLeftPosition(0, MAIN_PANEL_HEIGHT - BOTTOM_PANEL_HEIGHT);
    addAndMakeVisible(mBottomPanel);

    //[/Constructor]
}

MainPanel::~MainPanel()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]



    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void MainPanel::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colour (0xff323e44));

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void MainPanel::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="MainPanel" componentName=""
                 parentClasses="public Component" constructorParams="SpectralMorphingToolAudioProcessor* inProcessor"
                 variableInitialisers="mTopPanel (inProcessor)&#10;mCenterPanel (inProcessor)&#10;mBottomPanel (inProcessor)"
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="0" initialWidth="1000" initialHeight="700">
  <BACKGROUND backgroundColour="ff323e44"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

