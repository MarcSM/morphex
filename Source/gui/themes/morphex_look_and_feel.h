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

#include "../../helpers/interface_defines.h"

class MorphexLookAndFeel : public juce::LookAndFeel_V4
{
public:
    MorphexLookAndFeel()
    {
        // General
        setColour (juce::ResizableWindow::backgroundColourId, GUI::Color::Background);

        // Tabbed Component
        setColour (juce::TabbedButtonBar::tabOutlineColourId, GUI::Color::Transparent);

        // Midi Keyboard Component
        setColour (juce::MidiKeyboardComponent::mouseOverKeyOverlayColourId, GUI::Color::KeyDown.withAlpha (0.75f));
        setColour (juce::MidiKeyboardComponent::keyDownOverlayColourId, GUI::Color::KeyDown);
        setColour (juce::MidiKeyboardComponent::textLabelColourId, GUI::Color::Transparent);

        // TextButton
        setColour (juce::TextButton::buttonColourId, GUI::Color::Transparent);
        setColour (juce::TextButton::textColourOffId, juce::Colours::white);
        setColour (juce::TextButton::buttonOnColourId, GUI::Color::Accent);
        setColour (juce::TextButton::textColourOnId, GUI::Color::Accent);

        // ComboBox
        setColour (juce::ComboBox::backgroundColourId, GUI::Color::Transparent);
        setColour (juce::ComboBox::textColourId, juce::Colours::white);
        setColour (juce::ComboBox::outlineColourId, GUI::Color::Transparent);
        setColour (juce::ComboBox::arrowColourId, juce::Colours::white);
    }

    juce::Font getTabButtonFont (juce::TabBarButton&, float height) override
    {
        return { height * 0.2f };
    }

    void layoutFileBrowserComponent (juce::FileBrowserComponent&              browserComp,
                                     juce::DirectoryContentsDisplayComponent* fileListComponent,
                                     juce::FilePreviewComponent*              previewComp,
                                     juce::ComboBox*                          currentPathBox,
                                     juce::TextEditor*                        filenameBox,
                                     juce::Button*                            goUpButton) override
    {
        auto sectionHeight = 22;
        auto buttonWidth   = 50;

        //        auto b = browserComp.getLocalBounds().reduced (20, 5);
        auto b = browserComp.getLocalBounds();

        const float button_height = b.getHeight() * 0.075;

        //        b = browserComp.getLocalBounds().reduced (200, 5);

        //        auto topSlice    = b.removeFromTop (sectionHeight);
        //        auto bottomSlice = b.removeFromBottom (sectionHeight);

        auto topSlice    = b.removeFromTop (button_height);
        auto bottomSlice = b.removeFromBottom (button_height);

        currentPathBox->setBounds (topSlice.removeFromLeft (topSlice.getWidth() - buttonWidth));

        //        topSlice.removeFromLeft (6);
        goUpButton->setBounds (topSlice);

        //        bottomSlice.removeFromLeft (20);
        bottomSlice.removeFromBottom (button_height);
        filenameBox->setBounds (bottomSlice);

        if (previewComp != nullptr)
            previewComp->setBounds (b.removeFromRight (b.getWidth() / 3));

        if (auto* listAsComp = dynamic_cast<juce::Component*> (fileListComponent))
            listAsComp->setBounds (b.reduced (0, 10));
    }

    void drawComboBox (juce::Graphics& g, int width, int height, bool isButtonDown, int buttonX, int buttonY, int buttonW, int buttonH, juce::ComboBox& comboBox) override
    {
        LookAndFeel_V4::drawComboBox (g, width, height, isButtonDown, buttonX, buttonY, buttonW, buttonH, comboBox);

        // Draw borders
        GUI::Paint::drawBorders (g, comboBox.getLocalBounds(), GUI::Paint::BorderType::Glass);
    }

    // TODO - Tab button text
    void drawTabButtonText (juce::TabBarButton& button, juce::Graphics& g, bool isMouseOver, bool isMouseDown) override
    {
        auto area = button.getTextArea().toFloat();

        auto length = area.getWidth();
        auto depth  = area.getHeight();

        if (button.getTabbedButtonBar().isVertical())
            std::swap (length, depth);

        juce::Font font (getTabButtonFont (button, depth));
        font.setUnderline (button.hasKeyboardFocus (false));

        juce::AffineTransform t;

        switch (button.getTabbedButtonBar().getOrientation())
        {
            case juce::TabbedButtonBar::TabsAtLeft:
                t = t.rotated (juce::MathConstants<float>::pi * -0.5f).translated (area.getX(), area.getBottom());
                break;
            case juce::TabbedButtonBar::TabsAtRight:
                t = t.rotated (juce::MathConstants<float>::pi * 0.5f).translated (area.getRight(), area.getY());
                break;
            case juce::TabbedButtonBar::TabsAtTop:
            case juce::TabbedButtonBar::TabsAtBottom:
                t = t.translated (area.getX(), area.getY());
                break;
            default:
                jassertfalse;
                break;
        }

        juce::Colour col;

        if (button.isFrontTab() && (button.isColourSpecified (juce::TabbedButtonBar::frontTextColourId) || isColourSpecified (juce::TabbedButtonBar::frontTextColourId)))
        {
            col = findColour (juce::TabbedButtonBar::frontTextColourId);
        }
        else if (button.isColourSpecified (juce::TabbedButtonBar::tabTextColourId)
                 || isColourSpecified (juce::TabbedButtonBar::tabTextColourId))
        {
            col = findColour (juce::TabbedButtonBar::tabTextColourId);
        }
        else
        {
            col = button.getTabBackgroundColour().contrasting();
        }

        auto alpha = button.isEnabled() ? ((isMouseOver || isMouseDown) ? 1.0f : 0.8f) : 0.3f;

        g.setColour (col.withMultipliedAlpha (alpha));
        g.setFont (font);
        g.addTransform (t);

        g.drawFittedText (button.getButtonText().trim(),
                          0,
                          0,
                          (int) length,
                          (int) depth,
                          juce::Justification::centred,
                          juce::jmax (1, ((int) depth) / 12));
    }

    void drawButtonText (juce::Graphics& g, juce::TextButton& button, bool /*shouldDrawButtonAsHighlighted*/, bool /*shouldDrawButtonAsDown*/) override
    {
        juce::Font font (getTextButtonFont (button, button.getHeight()));
        g.setFont (font);
        g.setColour (button.findColour (button.getToggleState() ? juce::TextButton::textColourOnId
                                                                : juce::TextButton::textColourOffId)
                         .withMultipliedAlpha (button.isEnabled() ? 1.0f : 0.5f));

        const int yIndent = juce::jmin (4, button.proportionOfHeight (0.3f));
        //        const int cornerSize = jmin (button.getHeight(), button.getWidth()) / 2;
        const int cornerSize = 0;

        const int fontHeight  = juce::roundToInt (font.getHeight() * 0.6f);
        const int leftIndent  = juce::jmin (fontHeight, 2 + cornerSize / (button.isConnectedOnLeft() ? 4 : 2));
        const int rightIndent = juce::jmin (fontHeight, 2 + cornerSize / (button.isConnectedOnRight() ? 4 : 2));
        const int textWidth   = button.getWidth() - leftIndent - rightIndent;

        if (textWidth > 0)
        {
            g.drawFittedText (button.getButtonText(),
                              leftIndent,
                              yIndent,
                              textWidth,
                              button.getHeight() - yIndent * 2,
                              juce::Justification::centred,
                              2);
        }

        // Draw borders
        GUI::Paint::drawBorders (g, button.getLocalBounds(), GUI::Paint::BorderType::Glass);
    }

    // TabbedComponent
    int getTabButtonBestWidth (juce::TabBarButton& button, int tabDepth) override
    {
        return button.getTabbedButtonBar().getWidth() / button.getTabbedButtonBar().getNumTabs();
    }

    void drawTabAreaBehindFrontButton (juce::TabbedButtonBar& bar, juce::Graphics& g, const int w, const int h) override
    {
        LookAndFeel_V3::drawTabAreaBehindFrontButton (bar, g, w, h);

        // Draw borders
        GUI::Paint::drawBorders (g, bar.getLocalBounds());
    }

    void createTabTextLayout (const juce::TabBarButton& button, float length, float depth, juce::Colour colour, juce::TextLayout& textLayout)
    {
        juce::Font font (depth * 0.4f);
        font.setUnderline (button.hasKeyboardFocus (false));

        juce::AttributedString s;
        s.setJustification (juce::Justification::centred);
        s.append (button.getButtonText().trim(), font, colour);

        textLayout.createLayout (s, length);
    }

    void drawTabButton (juce::TabBarButton& button, juce::Graphics& g, bool isMouseOver, bool isMouseDown) override
    {
        const juce::Rectangle<int> activeArea (button.getActiveArea());

        const juce::TabbedButtonBar::Orientation o = button.getTabbedButtonBar().getOrientation();

        const juce::Colour bkg (button.getTabBackgroundColour());

        if (button.getToggleState())
        {
            g.setColour (bkg);
        }
        else
        {
            juce::Point<int> p1, p2;

            switch (o)
            {
                case juce::TabbedButtonBar::TabsAtBottom:
                    p1 = activeArea.getBottomLeft();
                    p2 = activeArea.getTopLeft();
                    break;
                case juce::TabbedButtonBar::TabsAtTop:
                    p1 = activeArea.getTopLeft();
                    p2 = activeArea.getBottomLeft();
                    break;
                case juce::TabbedButtonBar::TabsAtRight:
                    p1 = activeArea.getTopRight();
                    p2 = activeArea.getTopLeft();
                    break;
                case juce::TabbedButtonBar::TabsAtLeft:
                    p1 = activeArea.getTopLeft();
                    p2 = activeArea.getTopRight();
                    break;
                default:
                    jassertfalse;
                    break;
            }

            g.setGradientFill (juce::ColourGradient (bkg.brighter (0.2f), p1.toFloat(), bkg.darker (0.1f), p2.toFloat(), false));
        }

        g.fillRect (activeArea);

        g.setColour (button.findColour (juce::TabbedButtonBar::tabOutlineColourId));

        juce::Rectangle<int> r (activeArea);

        if (o != juce::TabbedButtonBar::TabsAtBottom)
            g.fillRect (r.removeFromTop (1));
        if (o != juce::TabbedButtonBar::TabsAtTop)
            g.fillRect (r.removeFromBottom (1));
        if (o != juce::TabbedButtonBar::TabsAtRight)
            g.fillRect (r.removeFromLeft (1));
        if (o != juce::TabbedButtonBar::TabsAtLeft)
            g.fillRect (r.removeFromRight (1));

        const float alpha = button.isEnabled() ? ((isMouseOver || isMouseDown) ? 1.0f : 0.8f) : 0.3f;

        juce::Colour col (bkg.contrasting().withMultipliedAlpha (alpha));

        if (juce::TabbedButtonBar* bar = button.findParentComponentOfClass<juce::TabbedButtonBar>())
        {
            juce::TabbedButtonBar::ColourIds colID = button.isFrontTab() ? juce::TabbedButtonBar::frontTextColourId
                                                                   : juce::TabbedButtonBar::tabTextColourId;

            if (bar->isColourSpecified (colID))
                col = bar->findColour (colID);
            else if (isColourSpecified (colID))
                col = findColour (colID);
        }

        const juce::Rectangle<float> area (button.getTextArea().toFloat());

        float length = area.getWidth();
        float depth  = area.getHeight();

        if (button.getTabbedButtonBar().isVertical())
            std::swap (length, depth);

        juce::TextLayout textLayout;
        createTabTextLayout (button, length, depth, col, textLayout);

        juce::AffineTransform t;

        switch (o)
        {
            case juce::TabbedButtonBar::TabsAtLeft:
                t = t.rotated (juce::MathConstants<float>::pi * -0.5f).translated (area.getX(), area.getBottom());
                break;
            case juce::TabbedButtonBar::TabsAtRight:
                t = t.rotated (juce::MathConstants<float>::pi * 0.5f).translated (area.getRight(), area.getY());
                break;
            case juce::TabbedButtonBar::TabsAtTop:
            case juce::TabbedButtonBar::TabsAtBottom:
                t = t.translated (area.getX(), area.getY());
                break;
            default:
                jassertfalse;
                break;
        }

        g.addTransform (t);
        textLayout.draw (g, juce::Rectangle<float> (length, depth));
    }

    void drawTabbedButtonBarBackground (juce::TabbedButtonBar&, juce::Graphics&) override
    {
        // TODO - Now empty it's fine
    }

    void fillTabButtonShape (juce::TabBarButton&, juce::Graphics&, const juce::Path&, bool isMouseOver, bool isMouseDown) override
    {
        // TODO - Now empty it's fine
    }

    // Buttons
    juce::Font getTextButtonFont (juce::TextButton&, int buttonHeight) override
    {
        return font_1;
    }

    void drawButtonBackground (juce::Graphics& g, juce::Button& button, const juce::Colour& backgroundColour, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override
    {
        juce::Colour fillColour;

        if (shouldDrawButtonAsDown)
        {
            fillColour = SMTColour_5;
        }
        else if (shouldDrawButtonAsHighlighted)
        {
            fillColour = SMTColour_4;
        }
        else
        {
            fillColour = SMTColour_3;
        }

        const float            cornerSize = 3.0f;
        const juce::Rectangle<float> bounds     = button.getLocalBounds().toFloat().reduced (0.5f, 0.5f);

        g.setColour (fillColour);
        g.fillRoundedRectangle (bounds.reduced (1), cornerSize);
    }

    // Combo Boxes
    juce::Font getLabelFont (juce::Label& label) override
    {
        return font_1;
    }

    void drawPopupMenuItem (juce::Graphics& g, const juce::Rectangle<int>& area, bool isSeparator, bool isActive, bool isHighlighted, bool isTicked, bool hasSubMenu, const juce::String& text, const juce::String& shortcutKeyText, const juce::Drawable* icon, const juce::Colour* textColour) override
    {
        juce::Rectangle<int> r (area);

        juce::Colour fillColour = isHighlighted ? SMTColour_5 : SMTColour_4;

        g.setColour (fillColour);
        g.fillRect (r.getX(), r.getY(), r.getWidth(), r.getHeight() - 1);

        juce::Colour customTextColour = isTicked ? SMTColour_7 : SMTColour_1;
        g.setColour (customTextColour);
        g.setFont (font_1);

        r.setLeft (10);
        r.setY (1);
        g.drawFittedText (text, r, juce::Justification::left, 1);
    }

private:
    juce::Image m_sliderImage;
};
