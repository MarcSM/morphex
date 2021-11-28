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

#pragma once

#include "morphing_panel.h"

class MainPanel : public Component
{
public:
    MainPanel (MorphexAudioProcessor& processor) :
        m_morphingPanel (processor)
    {
        addAndMakeVisible (m_morphingPanel);
    }

    ~MainPanel() {}

    void paint (Graphics& g) override {}

    void resized() override
    {
        FlexBox fb;

        FlexItem morphing (getWidth(), getHeight(), m_morphingPanel);

        fb.items.addArray ({ morphing });
        fb.performLayout (getLocalBounds().toFloat());
    }

private:
    MorphingPanel m_morphingPanel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainPanel)
};
