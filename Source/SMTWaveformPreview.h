/*
  ==============================================================================

    SMTWaveformPreview.h
    Created: 13 Jul 2019 11:17:08am
    Author:  Marc Sanchez Martinez

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"

#include "Sound.h"

class SMTWaveformPreview:
    public Component,
    public Timer
{
    Rectangle<float> rectangle;
    
public:
    
    SMTWaveformPreview(std::unique_ptr<Sound>& sound);

    ~SMTWaveformPreview();
    
    void drawWaveform (Graphics& g, int x, int y, int width, int height);
    void paint(Graphics& g) override;
    
    void timerCallback() override;
    
private:
    
    std::unique_ptr<Sound>& mSound;
    
    std::string currentSoundDisplay;
};
