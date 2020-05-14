/*
  ==============================================================================

    CollectionBrowser.h
    Created: 13 May 2020 1:49:08pm
    Author:  Marc Sanchez Martinez

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include "../SMTConstants.h"

//==============================================================================
/*
*/
class CollectionBrowser    : public Component
{
public:
    CollectionBrowser()
    {
        // In your constructor, you should add any child components, and
        // initialise any special settings that your component needs.
        
        if (!File(PLUGIN_DATA_COLLECTIONS_DIRECTORY).exists())
        {
            File(PLUGIN_DATA_COLLECTIONS_DIRECTORY).createDirectory();
        }
   
        int flags = FileBrowserComponent::openMode |
                    FileBrowserComponent::canSelectFiles |
                    FileBrowserComponent::filenameBoxIsReadOnly;
   
        m_wcFileFilter = new WildcardFileFilter("*.had", "*", "somedescription");

        // Create the browser component
        m_fileBrowser = new FileBrowserComponent(flags, PLUGIN_DATA_COLLECTIONS_DIRECTORY, m_wcFileFilter, NULL);
   
        // Add browser compoenent
        addAndMakeVisible(m_fileBrowser);
    }

    ~CollectionBrowser()
    {
    }

    void paint (Graphics& g) override
    {
        /* This demo code just fills the component's background and
           draws some placeholder text to get you started.

           You should replace everything in this method with your own
           drawing code..
        */
        
        g.fillAll (GUI::Color::BrowserBackground);

        /*
        g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));   // clear the background

        g.setColour (Colours::grey);
        g.drawRect (getLocalBounds(), 1);   // draw an outline around the component
        */
        
        /*
        g.setColour (Colours::white);
        g.setFont (14.0f);
        g.drawText ("CollectionBrowser", getLocalBounds(),
                    Justification::centred, true);   // draw some placeholder text
        */
        auto area = getLocalBounds();
        m_fileBrowser->setBounds(area);
    }

    void resized() override
    {
        // This method is where you should set the bounds of any child
        // components that your component contains..

    }

private:

    Rectangle<float> rectangle;
   
    Point<float> currentMouseXY;
    Point<float> mouseDownXY;
    
    String mCollectionsDirectory;
    String mPluginDirectory;

    ScopedPointer<FileBrowserComponent> m_fileBrowser;
    ScopedPointer<WildcardFileFilter> m_wcFileFilter;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CollectionBrowser)
};
