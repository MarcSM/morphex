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

#include "JuceHeader.h"

#include "../Helpers/InterfaceDefines.h"

class MorphexLookAndFeel
:   public LookAndFeel_V4
{
public:
    
    MorphexLookAndFeel()
    {
        // General
        setColour (ResizableWindow::backgroundColourId, GUI::Color::Background);
        
        // Tabbed Component
        setColour (TabbedButtonBar::tabOutlineColourId, GUI::Color::Transparent);
        
        // Midi Keyboard Component
        setColour (MidiKeyboardComponent::mouseOverKeyOverlayColourId, GUI::Color::KeyDown.withAlpha(0.75f));
        setColour (MidiKeyboardComponent::keyDownOverlayColourId, GUI::Color::KeyDown);
        setColour (MidiKeyboardComponent::textLabelColourId, GUI::Color::Transparent);

        // TextButton
        setColour (TextButton::buttonColourId, GUI::Color::Transparent);
        setColour (TextButton::textColourOffId, Colours::white);
        setColour (TextButton::buttonOnColourId, GUI::Color::Accent);
        setColour (TextButton::textColourOnId, GUI::Color::Accent);
        
        // ComboBox
        setColour (ComboBox::backgroundColourId, GUI::Color::Transparent);
        setColour (ComboBox::textColourId, Colours::white);
        setColour (ComboBox::outlineColourId, GUI::Color::Transparent);
        setColour (ComboBox::arrowColourId, Colours::white);
    }
    
    Font getTabButtonFont (TabBarButton&, float height) override
    {
        return { height * 0.2f };
    }
    
    void layoutFileBrowserComponent (FileBrowserComponent& browserComp,
                                                     DirectoryContentsDisplayComponent* fileListComponent,
                                                     FilePreviewComponent* previewComp,
                                                     ComboBox* currentPathBox,
                                                     TextEditor* filenameBox,
                                                     Button* goUpButton) override
    {
        auto sectionHeight = 22;
        auto buttonWidth = 50;
        
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
        
        if (auto* listAsComp = dynamic_cast<Component*> (fileListComponent))
            listAsComp->setBounds (b.reduced (0, 10));
    }
    
    void drawComboBox (Graphics& g, int width, int height, bool isButtonDown,
                       int buttonX, int buttonY, int buttonW, int buttonH,
                       ComboBox& comboBox) override
    {
        LookAndFeel_V4::drawComboBox (g, width, height, isButtonDown,
                                      buttonX, buttonY, buttonW, buttonH,
                                      comboBox);
        
        // Draw borders
        GUI::Paint::drawBorders(g, comboBox.getLocalBounds(), GUI::Paint::BorderType::Glass);
    }
    
    // TODO - Tab button text
    void drawTabButtonText (TabBarButton& button, Graphics& g, bool isMouseOver, bool isMouseDown) override
    {
        auto area = button.getTextArea().toFloat();
        
        auto length = area.getWidth();
        auto depth  = area.getHeight();
        
        if (button.getTabbedButtonBar().isVertical())
            std::swap (length, depth);
        
        Font font (getTabButtonFont (button, depth));
        font.setUnderline (button.hasKeyboardFocus (false));
        
        AffineTransform t;
        
        switch (button.getTabbedButtonBar().getOrientation())
        {
            case TabbedButtonBar::TabsAtLeft:   t = t.rotated (MathConstants<float>::pi * -0.5f).translated (area.getX(), area.getBottom()); break;
            case TabbedButtonBar::TabsAtRight:  t = t.rotated (MathConstants<float>::pi *  0.5f).translated (area.getRight(), area.getY());  break;
            case TabbedButtonBar::TabsAtTop:
            case TabbedButtonBar::TabsAtBottom: t = t.translated (area.getX(), area.getY());                                                 break;
            default:                            jassertfalse;                                                                                break;
        }
        
        Colour col;
        
        if (button.isFrontTab() && (button.isColourSpecified (TabbedButtonBar::frontTextColourId)
                                    || isColourSpecified (TabbedButtonBar::frontTextColourId)))
        {
            col = findColour (TabbedButtonBar::frontTextColourId);
        }
        else if (button.isColourSpecified (TabbedButtonBar::tabTextColourId)
                 || isColourSpecified (TabbedButtonBar::tabTextColourId))
        {
            col = findColour (TabbedButtonBar::tabTextColourId);
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
                          0, 0, (int) length, (int) depth,
                          Justification::centred,
                          jmax (1, ((int) depth) / 12));
    }
    
    void drawButtonText (Graphics& g, TextButton& button,
                         bool /*shouldDrawButtonAsHighlighted*/,
                         bool /*shouldDrawButtonAsDown*/) override
    {
        Font font (getTextButtonFont (button, button.getHeight()));
        g.setFont (font);
        g.setColour (button.findColour (button.getToggleState() ? TextButton::textColourOnId
                                        : TextButton::textColourOffId)
                     .withMultipliedAlpha (button.isEnabled() ? 1.0f : 0.5f));
        
        const int yIndent = jmin (4, button.proportionOfHeight (0.3f));
//        const int cornerSize = jmin (button.getHeight(), button.getWidth()) / 2;
        const int cornerSize = 0;

        const int fontHeight = roundToInt (font.getHeight() * 0.6f);
        const int leftIndent  = jmin (fontHeight, 2 + cornerSize / (button.isConnectedOnLeft() ? 4 : 2));
        const int rightIndent = jmin (fontHeight, 2 + cornerSize / (button.isConnectedOnRight() ? 4 : 2));
        const int textWidth = button.getWidth() - leftIndent - rightIndent;
        
        if (textWidth > 0)
        {
            g.drawFittedText (button.getButtonText(),
                              leftIndent, yIndent, textWidth, button.getHeight() - yIndent * 2,
                              Justification::centred, 2);
        }
        
        // Draw borders
        GUI::Paint::drawBorders (g, button.getLocalBounds(), GUI::Paint::BorderType::Glass);
    }

    // TabbedComponent
    int getTabButtonBestWidth (TabBarButton& button, int tabDepth) override
    {
        return button.getTabbedButtonBar().getWidth() / button.getTabbedButtonBar().getNumTabs();
    }
    
    void drawTabAreaBehindFrontButton (TabbedButtonBar& bar, Graphics& g, const int w, const int h) override
    {
        LookAndFeel_V3::drawTabAreaBehindFrontButton (bar, g, w, h);
        
        // Draw borders
        GUI::Paint::drawBorders (g, bar.getLocalBounds());
    }
    
    void createTabTextLayout (const TabBarButton& button, float length, float depth,
                                              Colour colour, TextLayout& textLayout)
    {
        Font font (depth * 0.4f);
        font.setUnderline (button.hasKeyboardFocus (false));
        
        AttributedString s;
        s.setJustification (Justification::centred);
        s.append (button.getButtonText().trim(), font, colour);
        
        textLayout.createLayout (s, length);
    }
    
    void drawTabButton (TabBarButton& button, Graphics& g, bool isMouseOver, bool isMouseDown) override
    {
        const Rectangle<int> activeArea (button.getActiveArea());
        
        const TabbedButtonBar::Orientation o = button.getTabbedButtonBar().getOrientation();
        
        const Colour bkg (button.getTabBackgroundColour());
        
        if (button.getToggleState())
        {
            g.setColour (bkg);
        }
        else
        {
            Point<int> p1, p2;
            
            switch (o)
            {
                case TabbedButtonBar::TabsAtBottom:   p1 = activeArea.getBottomLeft(); p2 = activeArea.getTopLeft();    break;
                case TabbedButtonBar::TabsAtTop:      p1 = activeArea.getTopLeft();    p2 = activeArea.getBottomLeft(); break;
                case TabbedButtonBar::TabsAtRight:    p1 = activeArea.getTopRight();   p2 = activeArea.getTopLeft();    break;
                case TabbedButtonBar::TabsAtLeft:     p1 = activeArea.getTopLeft();    p2 = activeArea.getTopRight();   break;
                default:                              jassertfalse;                                                     break;
            }
            
            g.setGradientFill (ColourGradient (bkg.brighter (0.2f), p1.toFloat(),
                                               bkg.darker (0.1f),   p2.toFloat(), false));
        }
        
        g.fillRect (activeArea);
        
        g.setColour (button.findColour (TabbedButtonBar::tabOutlineColourId));
        
        Rectangle<int> r (activeArea);
        
        if (o != TabbedButtonBar::TabsAtBottom)   g.fillRect (r.removeFromTop (1));
        if (o != TabbedButtonBar::TabsAtTop)      g.fillRect (r.removeFromBottom (1));
        if (o != TabbedButtonBar::TabsAtRight)    g.fillRect (r.removeFromLeft (1));
        if (o != TabbedButtonBar::TabsAtLeft)     g.fillRect (r.removeFromRight (1));
        
        const float alpha = button.isEnabled() ? ((isMouseOver || isMouseDown) ? 1.0f : 0.8f) : 0.3f;
        
        Colour col (bkg.contrasting().withMultipliedAlpha (alpha));
        
        if (TabbedButtonBar* bar = button.findParentComponentOfClass<TabbedButtonBar>())
        {
            TabbedButtonBar::ColourIds colID = button.isFrontTab() ? TabbedButtonBar::frontTextColourId
            : TabbedButtonBar::tabTextColourId;
            
            if (bar->isColourSpecified (colID)) col = bar->findColour (colID);
            else if (isColourSpecified (colID)) col = findColour (colID);
        }
        
        const Rectangle<float> area (button.getTextArea().toFloat());
        
        float length = area.getWidth();
        float depth  = area.getHeight();
        
        if (button.getTabbedButtonBar().isVertical())
            std::swap (length, depth);
        
        TextLayout textLayout;
        createTabTextLayout (button, length, depth, col, textLayout);
        
        AffineTransform t;
        
        switch (o)
        {
            case TabbedButtonBar::TabsAtLeft:   t = t.rotated (MathConstants<float>::pi * -0.5f).translated (area.getX(), area.getBottom()); break;
            case TabbedButtonBar::TabsAtRight:  t = t.rotated (MathConstants<float>::pi *  0.5f).translated (area.getRight(), area.getY());  break;
            case TabbedButtonBar::TabsAtTop:
            case TabbedButtonBar::TabsAtBottom: t = t.translated (area.getX(), area.getY());                                                 break;
            default:                            jassertfalse;                                                                                break;
        }
        
        g.addTransform (t);
        textLayout.draw (g, Rectangle<float> (length, depth));
    }
    
    void drawTabbedButtonBarBackground (TabbedButtonBar&, Graphics&) override
    {
        // TODO - Now empty it's fine
    }
    
    void fillTabButtonShape (TabBarButton&, Graphics&, const Path&, bool isMouseOver, bool isMouseDown) override
    {
        // TODO - Now empty it's fine
    }
    
    // Buttons
    Font getTextButtonFont (TextButton&, int buttonHeight) override
    {
        return font_1;
    }
    
    void drawButtonBackground (Graphics& g, Button& button, const Colour& backgroundColour,
                               bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override
    {
        Colour fillColour;
        
        if (shouldDrawButtonAsDown)
        {
            fillColour = SMTColour_5;
        }
        else if (shouldDrawButtonAsHighlighted)
        {
            fillColour = SMTColour_4;
        } else
        {
            fillColour = SMTColour_3;
        }
        
        const float cornerSize = 3.0f;
        const Rectangle<float> bounds = button.getLocalBounds().toFloat().reduced (0.5f, 0.5f);
        
        g.setColour (fillColour);
        g.fillRoundedRectangle (bounds.reduced (1), cornerSize);
    }
    
    // Combo Boxes
    Font getLabelFont (Label& label) override
    {
        return font_1;
    }
    
    void drawPopupMenuItem (Graphics& g, const Rectangle<int>& area,
                            bool isSeparator, bool isActive, bool isHighlighted, bool isTicked, bool hasSubMenu,
                            const String& text, const String& shortcutKeyText,
                            const Drawable* icon, const Colour* textColour) override
    {
        Rectangle<int> r (area);
        
        Colour fillColour = isHighlighted ? SMTColour_5 : SMTColour_4;
        
        g.setColour(fillColour);
        g.fillRect(r.getX(), r.getY(), r.getWidth(), r.getHeight() - 1);
        
        Colour customTextColour = isTicked ? SMTColour_7 : SMTColour_1;
        g.setColour(customTextColour);
        g.setFont(font_1);
        
        r.setLeft(10);
        r.setY(1);
        g.drawFittedText(text, r, Justification::left, 1);
    }
    
private:
    
    Image mSliderImage;
};
