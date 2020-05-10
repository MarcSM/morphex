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
    : AudioProcessorEditor (&p), processor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    
    setResizable (true, false);
    
    setSize(MAIN_PANEL_WIDTH, MAIN_PANEL_HEIGHT);
    
    // TODO TEST - Disable UI
    mMainPanel = new SMTMainPanel(&processor);
    addAndMakeVisible(mMainPanel);
    // TODO TEST - Disable UI
    
//    mLookAndFeel = new SMTLookAndFeel();
//    setLookAndFeel(mLookAndFeel);
//    LookAndFeel::setDefaultLookAndFeel(mLookAndFeel);
}

SpectralMorphingToolAudioProcessorEditor::~SpectralMorphingToolAudioProcessorEditor()
{
    SMTLookAndFeel::setDefaultLookAndFeel(nullptr);
    setLookAndFeel(nullptr);
    LookAndFeel::setDefaultLookAndFeel(nullptr);
}

//==============================================================================
void SpectralMorphingToolAudioProcessorEditor::paint (Graphics& g)
{
    g.fillAll (SMTColour_bg);
}

void SpectralMorphingToolAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}
