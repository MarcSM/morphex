/* Copyright (C) 2020 Marc Sanchez Martinez
 *
 * https://github.com/MarcSM/morphex
 *
 * Morphex is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Morphex is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Morphex. If not, see <http://www.gnu.org/licenses/>.
 */

#include "PluginProcessor.h"
#include "PluginEditor.h"

#include "Panels/MorphexLookAndFeel.h"

//==============================================================================
SpectralMorphingToolAudioProcessorEditor::SpectralMorphingToolAudioProcessorEditor (SpectralMorphingToolAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p), morphexPanel (&p)
{
    setLookAndFeel (new MorphexLookAndFeel());

    addAndMakeVisible (morphexPanel);

#if JUCE_ANDROID || JUCE_IOS
    setFullScreen (true);
#else
//    setUsingNativeTitleBar (true);
    this->resizeCorner = new ResizableCornerComponent (this, getConstrainer());
    addAndMakeVisible (this->resizeCorner);
    this->resizeCorner->setBounds (getWidth()-20, getHeight()-20, 20, 20);

    setResizable (true, false);
    setResizeLimits (MORPHEX_PANEL_WIDTH,
                     MORPHEX_PANEL_HEIGHT,
                     MORPHEX_PANEL_WIDTH * 4,
                     MORPHEX_PANEL_HEIGHT * 4);
    
    getConstrainer()->setFixedAspectRatio (MORPHEX_RATIO);

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
