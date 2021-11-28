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

#include <JuceHeader.h>

#include "../themes/morphex_look_and_feel.h"

namespace morphex
{
class Button : public juce::TextButton
{
public:
    Button() :
        juce::TextButton (juce::String()) {}

    ~Button() {}

    void paintButton (juce::Graphics& g,
                      bool      shouldDrawButtonAsHighlighted,
                      bool      shouldDrawButtonAsDown)
    {
        auto& lf = getLookAndFeel();

        lf.drawButtonBackground (g, *this, findColour (getToggleState() ? buttonOnColourId : buttonColourId), shouldDrawButtonAsHighlighted, shouldDrawButtonAsDown);

        lf.drawButtonText (g, *this, shouldDrawButtonAsHighlighted, shouldDrawButtonAsDown);

        if (icon.isNotEmpty())
        {
            g.setColour (findColour (getToggleState() ? TextButton::textColourOnId
                                                      : TextButton::textColourOffId)
                             .withMultipliedAlpha (isEnabled() ? 1.0f : 0.5f));
            const float icon_size = juce::jmin (getWidth(), getHeight()) * 0.6f;
            FontAwesome::drawCenterd (g, icon, icon_size, juce::Colours::white, getLocalBounds());
        }
    }

    void setIcon (Icon newIcon)
    {
        icon = newIcon;
    }

private:
    Icon icon;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Button)
};
}; // namespace morphex
