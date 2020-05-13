/*
 ==============================================================================
 
 MorphingPanel.h
 Created: 12 May 2020 12:45:09pm
 Author:  Marc Sanchez Martinez
 
 ==============================================================================
 */

#pragma once

#include <JuceHeader.h>

#include "SoundPanel.h"

#include "../Components/CollectionBrowser.h"

//==============================================================================
/*
 */
class MorphingPanel    : public Component
{
public:
    MorphingPanel()
    {
        // In your constructor, you should add any child components, and
        // initialise any special settings that your component needs.
        
        //        for (int i = 0; i < 4; i++)
        //        {
        //            soundPanel[i] = new SoundPanel(i);
        //        }
        
        //leftPanel = new LeftSidePanel ();
        //rightPanel = new RightSidePanel ();
        
        //        leftPanel = new LeftSidePanel (*soundPanel);
        //        rightPanel = new RightSidePanel (*soundPanel);
        
        addAndMakeVisible (rightPanel);
        addAndMakeVisible (leftPanel);
        addAndMakeVisible (centerPanel);
    }
    
    ~MorphingPanel()
    {
    }
    
    void paint (Graphics& g) override
    {
        /* This demo code just fills the component's background and
         draws some placeholder text to get you started.
         
         You should replace everything in this method with your own
         drawing code..
         */
        
        //g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));   // clear the background
        
        g.setColour (Colours::grey);
        g.drawRect (getLocalBounds(), 1);   // draw an outline around the component
        
        g.setColour (Colours::white);
        g.setFont (14.0f);
        g.drawText ("MorphingPanel", getLocalBounds(),
                    Justification::centred, true);   // draw some placeholder text
    }
    
    void resized() override
    {
        // This method is where you should set the bounds of any child
        // components that your component contains..
        
        FlexBox fb;
        
        // TODO - It's possible to keep all the widths and heights
        // in a class and access them via pointers?
        float centerPanelWidth = getWidth() / 3.0f;
        float soundPanelWidth = ( getWidth() - centerPanelWidth ) / 2.0f;
        //float soundPanelHeight = getHeight() / 6.0f;
        
        FlexItem left  (soundPanelWidth, getHeight(), leftPanel);
        FlexItem center  (centerPanelWidth, getHeight(), centerPanel);
        FlexItem right (soundPanelWidth, getHeight(), rightPanel);
        
        fb.items.addArray ( { left, center, right } );
        fb.performLayout (getLocalBounds().toFloat());
    }
    
private:
    
    //==============================================================================
    struct LeftSidePanel    : public Component
    {
        //        LeftSidePanel (SoundPanel* soundPanel) : soundPanel (soundPanel)
        LeftSidePanel ()
        {
            sound1Panel = std::make_unique<SoundPanel>(1);
            addAndMakeVisible (sound1Panel.get());
            
            sound3Panel = std::make_unique<SoundPanel>(3);
            addAndMakeVisible (sound3Panel.get());
            
            /*
             for (int i = 0; i < 6; ++i)
             {
             auto* slider = new Slider();
             slider->setSliderStyle (Slider::SliderStyle::Rotary);
             slider->setTextBoxStyle (Slider::NoTextBox, true, 0, 0);
             
             addAndMakeVisible (knobs.add (slider));
             }
             */
        }
        
        void paint (Graphics& g) override
        {
            //g.fillAll (backgroundColour);
        }
        
        void resized() override
        {
            FlexBox fb;
            fb.flexDirection = FlexBox::Direction::column;
            
            float soundPanelHeight = getHeight() / 2.0f;
            
            FlexItem sound1 (getWidth(), soundPanelHeight, *sound1Panel);
            FlexItem sound3 (getWidth(), soundPanelHeight, *sound3Panel);
            
            fb.items.addArray ( { sound1, sound3 } );
            fb.performLayout (getLocalBounds().toFloat());
            
            /*
             //==============================================================================
             FlexBox knobBox;
             knobBox.flexWrap = FlexBox::Wrap::wrap;
             knobBox.justifyContent = FlexBox::JustifyContent::spaceBetween;
             
             for (auto* k : knobs)
             knobBox.items.add (FlexItem (*k).withMinHeight (50.0f).withMinWidth (50.0f).withFlex (1));
             
             //==============================================================================
             FlexBox fb;
             fb.flexDirection = FlexBox::Direction::column;
             
             fb.items.add (FlexItem (knobBox).withFlex (2.5));
             
             fb.performLayout (getLocalBounds().toFloat());
             */
        }
        
        std::unique_ptr<SoundPanel> sound1Panel;
        std::unique_ptr<SoundPanel> sound3Panel;
        Colour backgroundColour;
        OwnedArray<Slider> knobs;
    };
    
    struct RightSidePanel    : public Component
    {
        //        RightSidePanel (SoundPanel* soundPanel) : soundPanel (soundPanel)
        RightSidePanel ()
        {
            sound2Panel = std::make_unique<SoundPanel>(2);
            addAndMakeVisible (sound2Panel.get());
            
            sound4Panel = std::make_unique<SoundPanel>(4);
            addAndMakeVisible (sound4Panel.get());
            
            /*
             for (int i = 0; i < 10; ++i)
             addAndMakeVisible (buttons.add (new TextButton (String (i))));
             */
        }
        
        void paint (Graphics& g) override
        {
            //g.fillAll (backgroundColour);
        }
        
        void resized() override
        {
            FlexBox fb;
            fb.flexDirection = FlexBox::Direction::column;
            
            float soundPanelHeight = getHeight() / 2.0f;
            
            FlexItem sound2 (getWidth(), soundPanelHeight, *sound2Panel);
            FlexItem sound4 (getWidth(), soundPanelHeight, *sound4Panel);
            
            fb.items.addArray ( { sound2, sound4 } );
            fb.performLayout (getLocalBounds().toFloat());
            
            /*
             FlexBox fb;
             fb.flexWrap = FlexBox::Wrap::wrap;
             fb.justifyContent = FlexBox::JustifyContent::center;
             fb.alignContent = FlexBox::AlignContent::center;
             
             for (auto* b : buttons)
             fb.items.add (FlexItem (*b).withMinWidth (50.0f).withMinHeight (50.0f));
             
             fb.performLayout (getLocalBounds().toFloat());
             */
        }
        
        //        SoundPanel* soundPanel;
        std::unique_ptr<SoundPanel> sound2Panel;
        std::unique_ptr<SoundPanel> sound4Panel;
        Colour backgroundColour;
        OwnedArray<TextButton> buttons;
    };
    
    
    
    struct CenterPanel    : public Component
    {
        CenterPanel()
        {
            // Browser
            const int browser_margin = 10;
            
            const int browser_width = getWidth() - (browser_margin * 2);
            const int browser_height = 200 - (browser_margin * 2);
            
            const int browser_x = browser_margin;
            const int browser_y = browser_margin;
            
            
            mBrowser = new CollectionBrowser();
            
            mBrowser->setBounds(browser_x, browser_y,
                                browser_width, browser_height);
            
            // Tabbed Component
            mTabbedComponent = new TabbedComponent (TabbedButtonBar::TabsAtTop);
            
            mTabbedComponent->setTabBarDepth(50);
            mTabbedComponent->addTab(TRANS("CORE"), GUI::Color::Transparent, new Component(), true);
            mTabbedComponent->addTab(TRANS("FX"), GUI::Color::Transparent, new Component(), true);
            mTabbedComponent->addTab(TRANS("SOUNDS"), GUI::Color::Transparent, mBrowser, true);
            //mTabbedComponent->addTab(TRANS("OPTIONS"), GUI::Color::Transparent, mBrowser, true);
            mTabbedComponent->setCurrentTabIndex(0);
            
            mTabbedComponent->setBounds (0, 0, getWidth(), getHeight());
            
            addAndMakeVisible(mTabbedComponent);
        }
        
        void paint (Graphics& g) override
        {
            auto area = getLocalBounds();
            mTabbedComponent->setBounds(area);
            
            //g.fillAll (Colours::hotpink);
        }
        
        void resized() override
        {
            
            
            /*
             auto isPortrait = getLocalBounds().getHeight() > getLocalBounds().getWidth();
             
             FlexBox fb;
             fb.flexDirection = isPortrait ? FlexBox::Direction::column
             : FlexBox::Direction::row;
             
             for (auto* s : sliders)
             {
             s->setSliderStyle (isPortrait ? Slider::SliderStyle::LinearHorizontal
             : Slider::SliderStyle::LinearVertical);
             
             fb.items.add (FlexItem (*s).withFlex (0, 1, isPortrait ? getHeight() / 5.0f
             : getWidth()  / 5.0f));
             }
             
             fb.performLayout (getLocalBounds().toFloat());
             */
        }
        
        OwnedArray<Slider> sliders;
        ScopedPointer<CollectionBrowser> mBrowser;
        ScopedPointer<TabbedComponent> mTabbedComponent;
    };
    
    //    SoundPanel* soundPanel[4];
    
    LeftSidePanel leftPanel;
    RightSidePanel rightPanel;
    CenterPanel centerPanel;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MorphingPanel)
};
