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

#include "editor.h"
#include "processor.h"

#include "gui/themes/morphex_look_and_feel.h"

//==============================================================================
MorphexAudioProcessorEditor::MorphexAudioProcessorEditor (MorphexAudioProcessor& processor) :
    AudioProcessorEditor (&processor), m_processor (processor), m_morphexPanel (processor)
{
    setLookAndFeel (new MorphexLookAndFeel());

    addAndMakeVisible (m_morphexPanel);

#if JUCE_ANDROID || JUCE_IOS
    initialiseJuce_GUI();
    setFullScreen (true);
#else
    //    setUsingNativeTitleBar (true);
    this->m_resizeCorner = new juce::ResizableCornerComponent (this, getConstrainer());
    addAndMakeVisible (this->m_resizeCorner);
    this->m_resizeCorner->setBounds (getWidth() - 20, getHeight() - 20, 20, 20);

    setResizable (true, false);
    setResizeLimits (MORPHEX_PANEL_WIDTH,
                     MORPHEX_PANEL_HEIGHT,
                     MORPHEX_PANEL_WIDTH * 4,
                     MORPHEX_PANEL_HEIGHT * 4);

    getConstrainer()->setFixedAspectRatio (MORPHEX_RATIO);

#endif
}

MorphexAudioProcessorEditor::~MorphexAudioProcessorEditor() {}

//==============================================================================
void MorphexAudioProcessorEditor::paint (juce::Graphics& g) {}

void MorphexAudioProcessorEditor::resized()
{
    m_morphexPanel.setBounds (0, 0, getWidth(), getHeight());
    m_resizeCorner->setBounds (getWidth() - 20, getHeight() - 20, 20, 20);
}
