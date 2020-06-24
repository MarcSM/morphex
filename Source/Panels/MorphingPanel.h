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

#include "SoundPanel.h"
#include "CorePanel.h"
#include "FxPanel.h"

#include "../Components/CollectionBrowser.h"

class MorphingPanel :   public Component
{
public:
    MorphingPanel(SpectralMorphingToolAudioProcessor* inProcessor)
    :   leftPanel (inProcessor),
        rightPanel (inProcessor),
        centerPanel (inProcessor)
    {
        addAndMakeVisible (leftPanel);
        addAndMakeVisible (rightPanel);
        addAndMakeVisible (centerPanel);
    }
    
    ~MorphingPanel() {}
    
    void paint (Graphics& g) override {}
    
    void resized() override
    {
        FlexBox fb;
        
        // TODO - Keep all the widths and heights
        // in a class and access them via pointers
        float centerPanelWidth = getWidth() * 0.75f * 0.45f;
        float soundPanelWidth = (getWidth() - centerPanelWidth) / 2.0f;
        
        FlexItem left (soundPanelWidth, getHeight(), leftPanel);
        FlexItem center (centerPanelWidth, getHeight(), centerPanel);
        FlexItem right (soundPanelWidth, getHeight(), rightPanel);
        
        fb.items.addArray ({left, center, right});
        fb.performLayout (getLocalBounds().toFloat());
    }
    
private:
    
    struct LeftSidePanel : public Component
    {
        LeftSidePanel (SpectralMorphingToolAudioProcessor* inProcessor)
        :   sound1Panel(inProcessor, 1),
            sound3Panel(inProcessor, 3)
        {
            addAndMakeVisible (sound1Panel);
            addAndMakeVisible (sound3Panel);
        }
        
        void paint (Graphics& g) override {}
        
        void resized() override
        {
            FlexBox fb;
            fb.flexDirection = FlexBox::Direction::column;
            
            float soundPanelHeight = getHeight() / 2.0f;
            
            FlexItem sound1 (getWidth(), soundPanelHeight, sound1Panel);
            FlexItem sound3 (getWidth(), soundPanelHeight, sound3Panel);
            
            fb.items.addArray ({sound1, sound3});
            fb.performLayout (getLocalBounds().toFloat());
        }

        SoundPanel sound1Panel;
        SoundPanel sound3Panel;
        
        Colour backgroundColour;
        OwnedArray<Slider> knobs;
    };
    
    struct RightSidePanel    : public Component
    {
        RightSidePanel (SpectralMorphingToolAudioProcessor* inProcessor)
        :   sound2Panel (inProcessor, 2),
            sound4Panel (inProcessor, 4)
        {
            addAndMakeVisible (sound2Panel);
            addAndMakeVisible (sound4Panel);
        }
        
        void paint (Graphics& g) override {}
        
        void resized() override
        {
            FlexBox fb;
            fb.flexDirection = FlexBox::Direction::column;
            
            float soundPanelHeight = getHeight() / 2.0f;
            
            FlexItem sound2 (getWidth(), soundPanelHeight, sound2Panel);
            FlexItem sound4 (getWidth(), soundPanelHeight, sound4Panel);
            
            fb.items.addArray ( { sound2, sound4 } );
            fb.performLayout (getLocalBounds().toFloat());
        }
        
        SoundPanel sound2Panel;
        SoundPanel sound4Panel;
        
        Colour backgroundColour;
        OwnedArray<TextButton> buttons;
    };
    
    struct CenterPanel : public Component
    {
        CenterPanel (SpectralMorphingToolAudioProcessor* inProcessor)
        {
            // Core Panel
            mCorePanel = new CorePanel (inProcessor);
            mFxPanel = new FxPanel (inProcessor);

            // Browser
            const int browser_margin = 10;
            
            const int browser_width = getWidth() - (browser_margin * 2);
            const int browser_height = 200 - (browser_margin * 2);
            
            const int browser_x = browser_margin;
            const int browser_y = browser_margin;
            
            mBrowser = new CollectionBrowser();

            mBrowser->setBounds (browser_x, browser_y, browser_width, browser_height);
            
            // Tabbed Component
            mTabbedComponent = new TabbedComponent (TabbedButtonBar::TabsAtTop);
            mTabbedComponent->setOutline (0);
            mTabbedComponent->addTab (TRANS ("CORE"), GUI::Color::Transparent, mCorePanel, true);
            mTabbedComponent->addTab (TRANS ("FX"), GUI::Color::Transparent, mFxPanel, true);
            mTabbedComponent->addTab (TRANS ("SOUNDS"), GUI::Color::Transparent, mBrowser, true);
            mTabbedComponent->setCurrentTabIndex (0);
            mTabbedComponent->setBounds (0, 0, getWidth(), getHeight());
            addAndMakeVisible (mTabbedComponent);
        }
        
        void paint (Graphics& g) override
        {
            auto area = getLocalBounds();
            mTabbedComponent->setBounds (area);
        }
        
        void resized() override
        {
            mTabbedComponent->setTabBarDepth (getHeight() / 8.0f);
        }
        
        OwnedArray<Slider> sliders;
        
        ScopedPointer<TabbedComponent> mTabbedComponent;
        ScopedPointer<CorePanel> mCorePanel;
        ScopedPointer<FxPanel> mFxPanel;
        ScopedPointer<CollectionBrowser> mBrowser;
    };
    
    LeftSidePanel leftPanel;
    RightSidePanel rightPanel;
    CenterPanel centerPanel;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MorphingPanel)
};
