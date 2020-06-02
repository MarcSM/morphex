/*
  ==============================================================================

    MorphexLookAndFeel.h
    Created: 12 May 2020 3:02:19pm
    Author:  Marc Sanchez Martinez

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"

#include "../InterfaceDefines.h"

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
        //// OLD STYLE
        
//
//        // TextButton Colours
//        setColour(TextButton::buttonColourId, SMTButtonColour);
//        setColour(TextButton::textColourOnId, SMTButtonTextOnColour);
//        setColour(TextButton::textColourOffId, SMTButtonTextOffColour);
        
        // HyperlinkButton Colours
        setColour(HyperlinkButton::textColourId, SMTHyperlinkButtonColour);
    }
    
//    void drawButtonBackground(Graphics& g, Button& button)
//    {
//        // Draw borders
//        GUI::Paint::drawBorders(g, button.getLocalBounds());
//    }
    
    Font getTabButtonFont (TabBarButton&, float height) override
    {
        return { height * 0.2f };
    }
    
//    void drawComboBox (Graphics& g, int width, int height, bool isButtonDown,
//                       int buttonX, int buttonY, int buttonW, int buttonH,
//                       ComboBox& box) override
//    {
//        const float cornerSize = 3.0f;
//        const Rectangle<int> boxBounds (0, 0, width, height);
//
//        g.setColour(SMTColour_3);
//
//        Rectangle<int> arrow (width - 30, 0, 20, height);
//        g.fillRoundedRectangle(boxBounds.toFloat(), cornerSize);
//
//        Path path;
//
//        path.startNewSubPath(arrow.getX() + 3.0f, arrow.getCentreY() - 2.0f);
//        path.lineTo(arrow.toFloat().getCentreX(), arrow.toFloat().getCentreY() + 3.0f);
//        path.lineTo(arrow.toFloat().getRight() - 3.0f, arrow.toFloat().getCentreY() - 2.0f);
//
//        Colour arrowColour = box.findColour(ComboBox::arrowColourId).withAlpha(box.isEnabled() ? 0.9f : 0.2f);
//        g.setColour(arrowColour);
//        g.strokePath(path, PathStrokeType(2.0f));
//    }
    
    void drawComboBox (Graphics& g, int width, int height, bool isButtonDown,
                       int buttonX, int buttonY, int buttonW, int buttonH,
                       ComboBox& comboBox) override
    {
        LookAndFeel_V4::drawComboBox (g, width, height, isButtonDown,
                                      buttonX, buttonY, buttonW, buttonH,
                                      comboBox);
        
//        auto cornerSize = box.findParentComponentOfClass<ChoicePropertyComponent>() != nullptr ? 0.0f : 3.0f;
//        Rectangle<int> boxBounds (0, 0, width, height);
//
//        g.setColour (box.findColour (ComboBox::backgroundColourId));
//        g.fillRoundedRectangle (boxBounds.toFloat(), cornerSize);
//
//        g.setColour (box.findColour (ComboBox::outlineColourId));
//        g.drawRoundedRectangle (boxBounds.toFloat().reduced (0.5f, 0.5f), cornerSize, 1.0f);
//
//        Path path;
//
//        path.startNewSubPath(arrow.getX() + 3.0f, arrow.getCentreY() - 2.0f);
//        path.lineTo(arrow.toFloat().getCentreX(), arrow.toFloat().getCentreY() + 3.0f);
//        path.lineTo(arrow.toFloat().getRight() - 3.0f, arrow.toFloat().getCentreY() - 2.0f);
//
//        Colour arrowColour = box.findColour(ComboBox::arrowColourId).withAlpha(box.isEnabled() ? 0.9f : 0.2f);
//        g.setColour(arrowColour);
//        g.strokePath(path, PathStrokeType(2.0f));
        
        // Draw borders
        GUI::Paint::drawBorders(g, comboBox.getLocalBounds());
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
            case TabbedButtonBar::TabsAtRight:  t = t.rotated (MathConstants<float>::pi *  0.5f).translated (area.getRight(), area.getY()); break;
            case TabbedButtonBar::TabsAtTop:
            case TabbedButtonBar::TabsAtBottom: t = t.translated (area.getX(), area.getY()); break;
            default:                            jassertfalse; break;
        }
        
        Colour col;
        
        if (button.isFrontTab() && (button.isColourSpecified (TabbedButtonBar::frontTextColourId)
                                    || isColourSpecified (TabbedButtonBar::frontTextColourId)))
            col = findColour (TabbedButtonBar::frontTextColourId);
        else if (button.isColourSpecified (TabbedButtonBar::tabTextColourId)
                 || isColourSpecified (TabbedButtonBar::tabTextColourId))
            col = findColour (TabbedButtonBar::tabTextColourId);
        else
            col = button.getTabBackgroundColour().contrasting();
        
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
                                         bool /*shouldDrawButtonAsHighlighted*/, bool /*shouldDrawButtonAsDown*/)
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
            g.drawFittedText (button.getButtonText(),
                              leftIndent, yIndent, textWidth, button.getHeight() - yIndent * 2,
                              Justification::centred, 2);
        
        // Draw borders
        GUI::Paint::drawBorders(g, button.getLocalBounds());
    }
    
//    void drawButtonText(Graphics& g, TextButton& button, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown)
//    {
//        LookAndFeel_V4::drawButtonText (g, button, shouldDrawButtonAsHighlighted, shouldDrawButtonAsDown);
//
////        TextButton::paintButton(g);
//
//        //        // Background
//        //        rectangle.setPosition(borderWidth / 2, borderWidth / 2);
//        //        rectangle.setSize(getWidth() - borderWidth, getHeight() - borderWidth);
//        //        g.setColour (Colours::transparentBlack);
//        //        g.fillRoundedRectangle (rectangle, cornerSize);
//
////        int w = getWidth();
//
//        // Draw borders
//        GUI::Paint::drawBorders(g, button.getLocalBounds());
////        GUI::Paint::drawBorders(g, getLocalBounds());
//    }
    
//    void TextButton::paintButton(Graphics& g)
//    {
//        TextButton::paintButton(g);
//
//        //        // Background
//        //        rectangle.setPosition(borderWidth / 2, borderWidth / 2);
//        //        rectangle.setSize(getWidth() - borderWidth, getHeight() - borderWidth);
//        //        g.setColour (Colours::transparentBlack);
//        //        g.fillRoundedRectangle (rectangle, cornerSize);
//
//        // Draw borders
//        GUI::Paint::drawBorders(g, getLocalBounds());
//    }

    // TabbedComponent
    int getTabButtonBestWidth (TabBarButton& button, int tabDepth) override
    {
        return button.getTabbedButtonBar().getWidth() / button.getTabbedButtonBar().getNumTabs();
    }
    
//    void drawTabAreaBehindFrontButton (TabbedButtonBar& bar, Graphics& g, const int w, const int h) override
//    {
//        /*
//        const float shadowSize = 0.f;
//
//        Rectangle<int> shadowRect, line;
//        ColourGradient gradient (Colours::black.withAlpha (bar.isEnabled() ? 0.08f : 0.04f), 0, 0,
//                                 Colours::transparentBlack, 0, 0, false);
//        
//        switch (bar.getOrientation())
//        {
//            case TabbedButtonBar::TabsAtLeft:
//                gradient.point1.x = (float) w;
//                gradient.point2.x = w * (1.0f - shadowSize);
//                shadowRect.setBounds ((int) gradient.point2.x, 0, w - (int) gradient.point2.x, h);
//                line.setBounds (w - 1, 0, 1, h);
//                break;
//                
//            case TabbedButtonBar::TabsAtRight:
//                gradient.point2.x = w * shadowSize;
//                shadowRect.setBounds (0, 0, (int) gradient.point2.x, h);
//                line.setBounds (0, 0, 1, h);
//                break;
//                
//            case TabbedButtonBar::TabsAtTop:
//                gradient.point1.y = (float) h;
//                gradient.point2.y = h * (1.0f - shadowSize);
//                shadowRect.setBounds (0, (int) gradient.point2.y, w, h - (int) gradient.point2.y);
//                line.setBounds (0, h - 1, w, 1);
//                break;
//                
//            case TabbedButtonBar::TabsAtBottom:
//                gradient.point2.y = h * shadowSize;
//                shadowRect.setBounds (0, 0, w, (int) gradient.point2.y);
//                line.setBounds (0, 0, w, 1);
//                break;
//                
//            default: break;
//        }
//        
//        g.setGradientFill (gradient);
//        g.fillRect (shadowRect.expanded (2, 2));
//        
//        g.setColour (bar.findColour (TabbedButtonBar::tabOutlineColourId));
//        g.fillRect (line);
//        */
//    }
    
    /*
    void drawTabButton(TabBarButton& button, Graphics& g, bool isMouseOver, bool isMouseDown) override
    {
        g.setColour(Colours::red);
        
        g.drawFittedText(button.getButtonText(), button.getBounds(), Justification::centred, 1);
    }
    */
    
//    void TabbedComponent::paint (Graphics& g) override
//    {
//        TabbedComponent::paint (Graphics& g);
//        
//        // Draw borders
//        GUI::Paint::drawBorders(g, getLocalBounds());
//    }
    
//    void drawTabButtonText (TabBarButton& button, Graphics& g, bool isMouseOver, bool isMouseDown) override
//    {
//        LookAndFeel_V2::drawTabButtonText (button, g, isMouseOver, isMouseDown);
//    }
    
    void drawTabAreaBehindFrontButton (TabbedButtonBar& bar, Graphics& g, const int w, const int h) override
    {
        LookAndFeel_V3::drawTabAreaBehindFrontButton (bar, g, w, h);
        
        // Draw borders
        GUI::Paint::drawBorders(g, bar.getLocalBounds());
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
                default:                              jassertfalse; break;
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
            
            if (bar->isColourSpecified (colID))
                col = bar->findColour (colID);
            else if (isColourSpecified (colID))
                col = findColour (colID);
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
            case TabbedButtonBar::TabsAtRight:  t = t.rotated (MathConstants<float>::pi *  0.5f).translated (area.getRight(), area.getY()); break;
            case TabbedButtonBar::TabsAtTop:
            case TabbedButtonBar::TabsAtBottom: t = t.translated (area.getX(), area.getY()); break;
            default:                            jassertfalse; break;
        }
        
        g.addTransform (t);
        textLayout.draw (g, Rectangle<float> (length, depth));
    }
    
    /*
    void drawTabButton (TabBarButton& button, Graphics& g, bool isMouseOver, bool isMouseDown) override
    {
        Rectangle<int> activeArea (button.getActiveArea());
        
        const TabbedButtonBar::Orientation o = button.getTabbedButtonBar().getOrientation();

        const Colour bkg (button.getTabBackgroundColour());

        if (button.getToggleState())
        {
            //g.setColour (bkg);
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
                default:                              jassertfalse; break;
            }
        }

        g.fillRect (activeArea);

        g.setColour (button.findColour (TabbedButtonBar::tabOutlineColourId));
        
        Rectangle<int> r (activeArea);
        
//        r.removeFromTop (1);
//        r.removeFromBottom (1);
//        r.removeFromLeft (1);
//        r.removeFromRight (1);
        
        g.fillRect (r);
        
//        if (o != TabbedButtonBar::TabsAtBottom)   g.fillRect (r.removeFromTop (1));
//        if (o != TabbedButtonBar::TabsAtTop)      g.fillRect (r.removeFromBottom (1));
//        if (o != TabbedButtonBar::TabsAtRight)    g.fillRect (r.removeFromLeft (1));
//        if (o != TabbedButtonBar::TabsAtLeft)     g.fillRect (r.removeFromRight (1));
        
        const float alpha = button.isEnabled() ? ((isMouseOver || isMouseDown) ? 1.0f : 0.8f) : 0.3f;

        Colour col (bkg.contrasting().withMultipliedAlpha (alpha));

        if (TabbedButtonBar* bar = button.findParentComponentOfClass<TabbedButtonBar>())
        {
            TabbedButtonBar::ColourIds colID = button.isFrontTab() ? TabbedButtonBar::frontTextColourId
            : TabbedButtonBar::tabTextColourId;

            if (bar->isColourSpecified (colID))
                col = bar->findColour (colID);
            else if (isColourSpecified (colID))
                col = findColour (colID);
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
            case TabbedButtonBar::TabsAtRight:  t = t.rotated (MathConstants<float>::pi *  0.5f).translated (area.getRight(), area.getY()); break;
            case TabbedButtonBar::TabsAtTop:
            case TabbedButtonBar::TabsAtBottom: t = t.translated (area.getX(), area.getY()); break;
            default:                            jassertfalse; break;
        }

        g.addTransform (t);
        textLayout.draw (g, Rectangle<float> (length, depth));
    }*/
    
    void drawTabbedButtonBarBackground (TabbedButtonBar&, Graphics&) override
    {
        
    }
    
    void fillTabButtonShape (TabBarButton&, Graphics&, const Path&, bool isMouseOver, bool isMouseDown) override
    {
        
    }
    
    //~MorphexLookAndFeel(){};
    
    // Buttons
    Font getTextButtonFont (TextButton&, int buttonHeight) override
    {
        return font_1;
    }
    
    void drawButtonBackground (Graphics& g, Button& button, const Colour& backgroundColour,
                               bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override
    {
        Colour fillColour;
        
        if (shouldDrawButtonAsDown) {
            fillColour = SMTColour_5;
        }
        else if (shouldDrawButtonAsHighlighted) {
            fillColour = SMTColour_4;
        } else {
            fillColour = SMTColour_3;
        }
        
        const float cornerSize = 3.0f;
        const Rectangle<float> bounds = button.getLocalBounds().toFloat().reduced(0.5f, 0.5f);
        
        g.setColour(fillColour);
        g.fillRoundedRectangle(bounds.reduced(1), cornerSize);
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
