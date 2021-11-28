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

#include "../components/collection_browser.h"
#include "core_panel.h"
#include "fx_panel.h"
#include "sound_panel.h"

class MorphingPanel : public juce::Component
{
public:
    MorphingPanel (MorphexAudioProcessor& inProcessor) :
        m_leftPanel (inProcessor),
        m_rightPanel (inProcessor),
        m_centerPanel (inProcessor)
    {
        addAndMakeVisible (m_leftPanel);
        addAndMakeVisible (m_rightPanel);
        addAndMakeVisible (m_centerPanel);
    }

    ~MorphingPanel() {}

    void paint (juce::Graphics& g) override {}

    void resized() override
    {
        juce::FlexBox fb;

        // TODO - Keep all the widths and heights
        // in a class and access them via pointers
        float m_centerPanelWidth = getWidth() * 0.75f * 0.45f;
        float soundPanelWidth    = (getWidth() - m_centerPanelWidth) / 2.0f;

        juce::FlexItem left (soundPanelWidth, getHeight(), m_leftPanel);
        juce::FlexItem center (m_centerPanelWidth, getHeight(), m_centerPanel);
        juce::FlexItem right (soundPanelWidth, getHeight(), m_rightPanel);

        fb.items.addArray ({ left, center, right });
        fb.performLayout (getLocalBounds().toFloat());
    }

private:
    struct LeftSidePanel : public juce::Component
    {
        LeftSidePanel (MorphexAudioProcessor& inProcessor) :
            sound1Panel (inProcessor, 1),
            sound3Panel (inProcessor, 3)
        {
            addAndMakeVisible (sound1Panel);
            addAndMakeVisible (sound3Panel);
        }

        void paint (juce::Graphics& g) override {}

        void resized() override
        {
            juce::FlexBox fb;
            fb.flexDirection = juce::FlexBox::Direction::column;

            float soundPanelHeight = getHeight() / 2.0f;

            juce::FlexItem sound1 (getWidth(), soundPanelHeight, sound1Panel);
            juce::FlexItem sound3 (getWidth(), soundPanelHeight, sound3Panel);

            fb.items.addArray ({ sound1, sound3 });
            fb.performLayout (getLocalBounds().toFloat());
        }

        SoundPanel sound1Panel;
        SoundPanel sound3Panel;

        juce::Colour             backgroundColour;
        juce::OwnedArray<juce::Slider> knobs;
    };

    struct RightSidePanel : public juce::Component
    {
        RightSidePanel (MorphexAudioProcessor& inProcessor) :
            sound2Panel (inProcessor, 2),
            sound4Panel (inProcessor, 4)
        {
            addAndMakeVisible (sound2Panel);
            addAndMakeVisible (sound4Panel);
        }

        void paint (juce::Graphics& g) override {}

        void resized() override
        {
            juce::FlexBox fb;
            fb.flexDirection = juce::FlexBox::Direction::column;

            float soundPanelHeight = getHeight() / 2.0f;

            juce::FlexItem sound2 (getWidth(), soundPanelHeight, sound2Panel);
            juce::FlexItem sound4 (getWidth(), soundPanelHeight, sound4Panel);

            fb.items.addArray ({ sound2, sound4 });
            fb.performLayout (getLocalBounds().toFloat());
        }

        SoundPanel sound2Panel;
        SoundPanel sound4Panel;

        juce::Colour                 backgroundColour;
        juce::OwnedArray<juce::TextButton> buttons;
    };

    struct CenterPanel : public juce::Component
    {
        CenterPanel (MorphexAudioProcessor& inProcessor)
        {
            // Core Panel
            m_corePanel = new CorePanel (inProcessor);
            m_fxPanel   = new FxPanel (inProcessor);

            // Browser
            const int browser_margin = 10;

            const int browser_width  = getWidth() - (browser_margin * 2);
            const int browser_height = 200 - (browser_margin * 2);

            const int browser_x = browser_margin;
            const int browser_y = browser_margin;

            m_browser = new CollectionBrowser (inProcessor);

            m_browser->setBounds (browser_x, browser_y, browser_width, browser_height);

            // Tabbed Component
            m_tabbedComponent = new juce::TabbedComponent (juce::TabbedButtonBar::TabsAtTop);
            m_tabbedComponent->setOutline (0);
            m_tabbedComponent->addTab (TRANS ("CORE"), GUI::Color::Transparent, m_corePanel, true);
            m_tabbedComponent->addTab (TRANS ("FX"), GUI::Color::Transparent, m_fxPanel, true);
            m_tabbedComponent->addTab (TRANS ("SOUNDS"), GUI::Color::Transparent, m_browser, true);
            m_tabbedComponent->setCurrentTabIndex (0);
            m_tabbedComponent->setBounds (0, 0, getWidth(), getHeight());
            addAndMakeVisible (m_tabbedComponent);
        }

        void paint (juce::Graphics& g) override
        {
            auto area = getLocalBounds();
            m_tabbedComponent->setBounds (area);
        }

        void resized() override
        {
            m_tabbedComponent->setTabBarDepth (getHeight() / 8.0f);
        }

        juce::OwnedArray<juce::Slider> m_sliders;

        juce::ScopedPointer<juce::TabbedComponent>   m_tabbedComponent;
        juce::ScopedPointer<CorePanel>         m_corePanel;
        juce::ScopedPointer<FxPanel>           m_fxPanel;
        juce::ScopedPointer<CollectionBrowser> m_browser;
    };

    LeftSidePanel  m_leftPanel;
    RightSidePanel m_rightPanel;
    CenterPanel    m_centerPanel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MorphingPanel)
};
