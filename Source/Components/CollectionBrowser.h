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
