/*
  ==============================================================================

    SMTSoundPanel.cpp
    Created: 19 Jul 2019 11:52:52am
    Author:  Marc Sanchez Martinez

  ==============================================================================
*/

#include "SMTSoundPanel.h"
#include "SMTParameters.h"

SMTSoundPanel::SMTSoundPanel(SpectralMorphingToolAudioProcessor* inProcessor, std::unique_ptr<Sound>& sound):
    SMTPanelBase(inProcessor),
    mProcessor(inProcessor)
{
    setSize(SOUND_PANEL_WIDTH, SOUND_PANEL_HEIGHT);
    
    // Browser
    const int waveform_margin = 10;
    
    const int waveform_width = getWidth() - (waveform_margin * 2);
    const int waveform_height = 200 - (waveform_margin * 2);

    const int waveform_x = waveform_margin;
    const int waveform_y = waveform_margin;

    mWaveformPreview = new SMTWaveformPreview(sound);
    
    mWaveformPreview->setBounds(waveform_x, waveform_y,
                                waveform_width, waveform_height);

    addAndMakeVisible(mWaveformPreview);
    
    repaint();
}

SMTSoundPanel::~SMTSoundPanel()
{
}

void SMTSoundPanel::paint (Graphics& g)
{
    SMTPanelBase::paint(g);
}
