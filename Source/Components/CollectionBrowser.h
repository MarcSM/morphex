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

#include "../Panels/MorphexLookAndFeel.h"

class CollectionBrowser
:   public FileBrowserComponent,
    public Button::Listener
{
public:
    
    CollectionBrowser (SpectralMorphingToolAudioProcessor* in_processor,
                       int flags = FileBrowserComponent::openMode |
                                   FileBrowserComponent::canSelectFiles |
                                   FileBrowserComponent::useTreeView |
                                   FileBrowserComponent::filenameBoxIsReadOnly,
                       const File& initialFileOrDirectory = PLUGIN_DATA_COLLECTIONS_DIRECTORY,
                       const juce::FileFilter* fileFilter = new WildcardFileFilter ("*.had", "*", "somedescription"),
                       FilePreviewComponent* previewComp = NULL)
    :   FileBrowserComponent (flags, initialFileOrDirectory, fileFilter, previewComp),
        processor (in_processor)
    {
        setLookAndFeel (new MorphexLookAndFeel());
        
        pTreeComponent = static_cast<FileTreeComponent*> (this->getDisplayComponent());
        if (pTreeComponent) pTreeComponent->setDragAndDropDescription ("DragAndDrop");
        
        loadAllButton.addListener (this);
        addAndMakeVisible (loadAllButton);
        
        resized();
    }
    
    ~CollectionBrowser() {}

    void paint (Graphics& g) override
    {
        g.fillAll (GUI::Color::BrowserBackground);
        
        // Draw borders
        GUI::Paint::drawBorders (g, file_browser_area, GUI::Paint::BorderType::Glass);
    }
    
    void resized() override
    {
        FileBrowserComponent::resized();
        
        const float button_height = getHeight() * 0.075;
        
        loadAllButton.setBounds (0, getHeight() - button_height, getWidth(), button_height);
        
        file_browser_area.setBounds (0, button_height, getWidth(), getHeight() - (2 * button_height));
        
        this->getLocalBounds().removeFromBottom (button_height);
        
//        auto b = this->getLocalBounds().reduced (80, 35);
//        b.removeFromBottom (button_height);
        
//        filenameBox.setPosition (0, 0);
        
//        if (getPreviewComponent() != nullptr)
//        {
//            auto b = getLocalBounds();
//            getPreviewComponent()->setBounds (0,0, 50, 100);
////            getPreviewComponent()->setBounds (b.removeFromBottom (button_height));
//        }
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
    
    void buttonClicked (Button* b) override
    {
        if (b == &loadAllButton)
        {
            const File f = getHighlightedFile();
//            const File f = getSelectedFile (0);

            processor->mMorphexSynth.instrument.reset();
            
            if (f.isDirectory())
            {
                // Loading selected folder
                processor->mMorphexSynth.instrument.loadAllSoundsFromFolder (f.getFullPathName().toStdString());
            }
            else
            {
                // Loading current folder
                processor->mMorphexSynth.instrument.loadAllSoundsFromFolder (getRoot().getFullPathName().toStdString());
            }
        }
    }
    
    SpectralMorphingToolAudioProcessor* processor;
    
    FileTreeComponent* pTreeComponent;
    
    Rectangle<int> file_browser_area;

    Rectangle<float> rectangle;
   
    Point<float> currentMouseXY;
    Point<float> mouseDownXY;
    
    String mCollectionsDirectory;
    String mPluginDirectory;
    
    TextButton loadAllButton {"Load All Sounds"};
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CollectionBrowser);
};
