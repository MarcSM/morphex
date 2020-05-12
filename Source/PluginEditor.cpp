/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SpectralMorphingToolAudioProcessorEditor::SpectralMorphingToolAudioProcessorEditor (SpectralMorphingToolAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p), morphexPanel()
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.

    addAndMakeVisible (morphexPanel);
    
#if JUCE_ANDROID || JUCE_IOS
    setFullScreen (true);
#else
    this->resizeCorner = new ResizableCornerComponent(this, getConstrainer());
    addAndMakeVisible(this->resizeCorner);
    this->resizeCorner->setBounds (getWidth()-20, getHeight()-20, 20, 20);
    
    double ratio = double(MORPHEX_PANEL_WIDTH) / double(MORPHEX_PANEL_HEIGHT);

    // setResizeLimits(400, 400/ratio, 1200, 1200/ratio);
    // getConstrainer()->setFixedAspectRatio(ratio);
    // setSize(800.0,800.0/ratio);
    
    setResizable (true, false);
    setResizeLimits (MORPHEX_PANEL_WIDTH,
                     MORPHEX_PANEL_HEIGHT,
                     MORPHEX_PANEL_WIDTH * 4,
                     MORPHEX_PANEL_HEIGHT * 4);
    //            setResizeLimits (MORPHEX_PANEL_WIDTH,
    //                            MORPHEX_PANEL_HEIGHT,
    //                            MORPHEX_PANEL_WIDTH / ratio,
    //                            MORPHEX_PANEL_HEIGHT / ratio);
    getConstrainer()->setFixedAspectRatio(ratio);
    centreWithSize (getWidth(), getHeight());

#endif
    
    
    

    
    // TODO TEST - Disable UI
//    mMainPanel = new MainPanel();
////    mMainPanel = new MainPanel(&processor);
//    addAndMakeVisible(mMainPanel);
    // TODO TEST - Disable UI
    
//    mLookAndFeel = new SMTLookAndFeel();
//    setLookAndFeel(mLookAndFeel);
//    LookAndFeel::setDefaultLookAndFeel(mLookAndFeel);
}

SpectralMorphingToolAudioProcessorEditor::~SpectralMorphingToolAudioProcessorEditor()
{
//    SMTLookAndFeel::setDefaultLookAndFeel(nullptr);
//    setLookAndFeel(nullptr);
//    LookAndFeel::setDefaultLookAndFeel(nullptr);
}

//==============================================================================
void SpectralMorphingToolAudioProcessorEditor::paint (Graphics& g)
{
    // g.fillAll (SMTColour_bg);
}

void SpectralMorphingToolAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    
    resizeCorner->setBounds (getWidth()-20, getHeight()-20, 20, 20);
    morphexPanel.setBounds(0, 0, getWidth(), getHeight());
}
