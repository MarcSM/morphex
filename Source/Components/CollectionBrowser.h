/*
  ==============================================================================

    CollectionBrowser.h
    Created: 13 May 2020 1:49:08pm
    Author:  Marc Sanchez Martinez

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include "../Helpers/SMTConstants.h"

class CollectionBrowser : public FileBrowserComponent
{
public:
    
    CollectionBrowser (int flags = FileBrowserComponent::openMode |
                                   FileBrowserComponent::canSelectFiles |
                                   FileBrowserComponent::useTreeView |
                                   FileBrowserComponent::filenameBoxIsReadOnly,
                       const File& initialFileOrDirectory = PLUGIN_DATA_COLLECTIONS_DIRECTORY,
                       const juce::FileFilter* fileFilter = new WildcardFileFilter ("*.had", "*", "somedescription"),
                       FilePreviewComponent* previewComp = NULL)
    :   FileBrowserComponent (flags, initialFileOrDirectory, fileFilter, previewComp)
    {
        pTreeComponent = static_cast<FileTreeComponent*> (this->getDisplayComponent());
        if (pTreeComponent) pTreeComponent->setDragAndDropDescription ("DragAndDrop");
    }
    
    ~CollectionBrowser() {}

    void paint (Graphics& g) override
    {
        g.fillAll (GUI::Color::BrowserBackground);
        
        // Draw borders
        GUI::Paint::drawBorders (g, getLocalBounds(), GUI::Paint::BorderType::Glass);
    }

private:
    
    void fileClicked (const File& f, const MouseEvent& e) override
    {
        FileBrowserComponent::fileClicked (f, e);
        if (f.isDirectory())
        {
            pTreeComponent->setDragAndDropDescription ("directory");
        }
        else
        {
            pTreeComponent->setDragAndDropDescription (pTreeComponent->getSelectedFile().getFullPathName());
        }
    }
    
    FileTreeComponent* pTreeComponent;

    Rectangle<float> rectangle;
   
    Point<float> currentMouseXY;
    Point<float> mouseDownXY;
    
    String mCollectionsDirectory;
    String mPluginDirectory;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CollectionBrowser)
};
