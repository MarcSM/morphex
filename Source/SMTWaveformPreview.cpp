/*
  ==============================================================================

    SMTWaveformPreview.cpp
    Created: 13 Jul 2019 11:17:08am
    Author:  Marc Sanchez Martinez

  ==============================================================================
*/

#include "SMTWaveformPreview.h"

#include "SMTInterfaceDefines.h"

SMTWaveformPreview::SMTWaveformPreview(std::unique_ptr<Sound>& sound)
:   mSound(sound)
{
    startTimer(5000); // 0.5 seconds
}

SMTWaveformPreview::~SMTWaveformPreview()
{
    
}

void SMTWaveformPreview::drawWaveform(Graphics& g, int x, int y, int width, int height)
{
    int preview_type = 0;
    
    // Waveform Preview
    if (preview_type == 0)
    {
        int waveform_size = (int) mSound->synthesis.harmonic.size();
        
        for (int i = 1; i < waveform_size; ++i)
        {
            g.drawLine({    (float) jmap(i - 1, 0, waveform_size - 1, 0, width) + x,
                            jmap(mSound->synthesis.harmonic[i - 1], -1.0f, 1.0f, (float) height, 0.0f) + y,
                            (float) jmap(i, 0, waveform_size - 1, 0,  width) + x,
                            jmap(mSound->synthesis.harmonic[i], -1.0f, 1.0f, (float) height, 0.0f) + y      },
                       0.3f
            );
        }
    }
    // Harmonics Preview
    else
    {
        // TODO
    }
}

void SMTWaveformPreview::paint(Graphics& g)
{
    // Background
    int borderWidth = 0;
    int cornerSize = 4;
    g.setColour (Colours::black);
    rectangle.setPosition(borderWidth / 2, borderWidth / 2);
    rectangle.setSize(getWidth() - borderWidth, getHeight() - borderWidth);
    g.fillRoundedRectangle (rectangle, cornerSize);
    
    // Write sound name
    int sound_name_height = 30;
    g.setColour (Colours::white);
    g.setFont(font_2);
//    g.drawText (mSound->file.name, 0, 0, getWidth(), sound_name_height, Justification::centred);

    // Draw the Waveform
    g.setColour (Colours::white);
//    drawWaveform(g, 0, sound_name_height, getWidth(), getHeight() - sound_name_height);
}

void SMTWaveformPreview::timerCallback()
{
//    if (currentSoundDisplay != mSound->path)
//    {
//        currentSoundDisplay = mSound->path;
//        repaint();
//    }
}
