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
class CollectionBrowser    : public FileBrowserComponent
{
public:
    
//    CollectionBrowser()
//    {
//        if (!File(PLUGIN_DATA_COLLECTIONS_DIRECTORY).exists())
//        {
//            File(PLUGIN_DATA_COLLECTIONS_DIRECTORY).createDirectory();
//        }
//
//        int flags = FileBrowserComponent::openMode |
//        FileBrowserComponent::canSelectFiles |
//        FileBrowserComponent::useTreeView |
//        FileBrowserComponent::filenameBoxIsReadOnly;
//
//        m_wcFileFilter = new WildcardFileFilter("*.had", "*", "somedescription");
//
//        // Create the browser component
//        m_fileBrowser = new FileBrowserComponent(flags, PLUGIN_DATA_COLLECTIONS_DIRECTORY, m_wcFileFilter, NULL);
//
//        pTreeComponent = static_cast<FileTreeComponent*> (m_fileBrowser->getDisplayComponent());
//        if (pTreeComponent) pTreeComponent->setDragAndDropDescription("DragAndDrop");
//
//        // Add browser compoenent
//        addAndMakeVisible(m_fileBrowser);
//    }
    
//    CollectionBrowser (int flags,
//                       const File& initialFileOrDirectory,
//                       const juce::FileFilter* fileFilter,
//                       FilePreviewComponent* previewComp)
    
    CollectionBrowser (int flags = FileBrowserComponent::openMode |
                       FileBrowserComponent::canSelectFiles |
                       FileBrowserComponent::useTreeView |
                       FileBrowserComponent::filenameBoxIsReadOnly,
                       const File& initialFileOrDirectory = PLUGIN_DATA_COLLECTIONS_DIRECTORY,
                       const juce::FileFilter* fileFilter = new WildcardFileFilter("*.had", "*", "somedescription"),
                       FilePreviewComponent* previewComp = NULL)
    : FileBrowserComponent(flags, initialFileOrDirectory, fileFilter, previewComp)
    {
        pTreeComponent = static_cast<FileTreeComponent*> (this->getDisplayComponent());
        if (pTreeComponent) pTreeComponent->setDragAndDropDescription("DragAndDrop");
    }
    
//    CollectionBrowser (int flags = FileBrowserComponent::openMode |
//                       FileBrowserComponent::canSelectFiles |
//                       FileBrowserComponent::useTreeView |
//                       FileBrowserComponent::filenameBoxIsReadOnly,
//                       const File& initialFileOrDirectory = PLUGIN_DATA_COLLECTIONS_DIRECTORY,
//                       const FileFilter* fileFilter = new WildcardFileFilter("*.had", "*", "somedescription"),
//                       FilePreviewComponent* previewComp = NULL)
    
//    CollectionBrowser (int flags,
//                       const File& initialFileOrDirectory,
//                       const juce::FileFilter* fileFilter,
//                       FilePreviewComponent* previewComp)
//    {
    
        
//        FileTreeComponent* pTreeComponent = static_cast<FileTreeComponent*> (mFileBrowserComp->getDisplayComponent());
//
//        if(pTreeComponent)
//        {
//            pTreeComponent->setDragAndDropDescription("DragAndDrop");
//        }
        
        // In your constructor, you should add any child components, and
        // initialise any special settings that your component needs.
        
//        if (!File(PLUGIN_DATA_COLLECTIONS_DIRECTORY).exists())
//        {
//            File(PLUGIN_DATA_COLLECTIONS_DIRECTORY).createDirectory();
//        }
//
//        int flags = FileBrowserComponent::openMode |
//                    FileBrowserComponent::canSelectFiles |
//                    FileBrowserComponent::useTreeView |
//                    FileBrowserComponent::filenameBoxIsReadOnly;
//
//        m_wcFileFilter = new WildcardFileFilter("*.had", "*", "somedescription");
//
//        // Create the browser component
//        m_fileBrowser = new FileBrowserComponent(flags, PLUGIN_DATA_COLLECTIONS_DIRECTORY, m_wcFileFilter, NULL);
//
//        pTreeComponent = static_cast<FileTreeComponent*> (m_fileBrowser->getDisplayComponent());
//        if (pTreeComponent) pTreeComponent->setDragAndDropDescription("DragAndDrop");
//
//        // Add browser compoenent
//        addAndMakeVisible(m_fileBrowser);
//    }

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
        
        g.fillAll (GUI::Color::BrowserBackground);
        
        // Draw borders
        GUI::Paint::drawBorders(g, getLocalBounds());
    }

//    void resized() override
//    {
//        auto area = getLocalBounds();
//        m_fileBrowser->setBounds(area);
//    }

private:
    
//    void fileClicked (const File& f, const MouseEvent& e) override
    void fileClicked (const File& f, const MouseEvent& e) override
    {
        FileBrowserComponent::fileClicked (f, e);
        pTreeComponent->setDragAndDropDescription (pTreeComponent->getSelectedFile().getFullPathName());
    }
    
//    void selectionChanged() override
//    {
//        // we're just going to update the drag description of out tree so that rows can be dragged onto the file players
//        pTreeComponent->setDragAndDropDescription (pTreeComponent->getSelectedFile().getFullPathName());
//    }
    
    FileTreeComponent* pTreeComponent;

    Rectangle<float> rectangle;
   
    Point<float> currentMouseXY;
    Point<float> mouseDownXY;
    
    String mCollectionsDirectory;
    String mPluginDirectory;

//    ScopedPointer<FileBrowserComponent> m_fileBrowser;
//    ScopedPointer<WildcardFileFilter> m_wcFileFilter;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CollectionBrowser)
};
