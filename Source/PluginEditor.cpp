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
    : AudioProcessorEditor (&p), processor (p), morphexPanel(&p)
{
    addAndMakeVisible (morphexPanel);

#if JUCE_ANDROID || JUCE_IOS
    setFullScreen (true);
#else
//    setUsingNativeTitleBar (true);
    this->resizeCorner = new ResizableCornerComponent(this, getConstrainer());
    addAndMakeVisible(this->resizeCorner);
    this->resizeCorner->setBounds (getWidth()-20, getHeight()-20, 20, 20);

    setResizable (true, false);
    setResizeLimits (MORPHEX_PANEL_WIDTH,
                     MORPHEX_PANEL_HEIGHT,
                     MORPHEX_PANEL_WIDTH * 4,
                     MORPHEX_PANEL_HEIGHT * 4);
    
    getConstrainer()->setFixedAspectRatio(MORPHEX_RATIO);

#endif
}

SpectralMorphingToolAudioProcessorEditor::~SpectralMorphingToolAudioProcessorEditor() {}

//==============================================================================
void SpectralMorphingToolAudioProcessorEditor::paint (Graphics& g) {}

void SpectralMorphingToolAudioProcessorEditor::resized()
{
    morphexPanel.setBounds (0, 0, getWidth(), getHeight());
    resizeCorner->setBounds (getWidth()-20, getHeight()-20, 20, 20);
}
