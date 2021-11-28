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
 
#include "../themes/morphex_look_and_feel.h"

namespace Constants
{
const auto PluginDataDirectory = (juce::File::getSpecialLocation (juce::File::userDesktopDirectory)).getFullPathName() + juce::File::getSeparatorChar() + ProjectInfo::projectName;
const auto CollectionDirectory = Constants::PluginDataDirectory + juce::File::getSeparatorChar() + "Collections";
const auto FactoryDirectory = CollectionDirectory + juce::File::getSeparatorChar() + "Factory";
const auto FileBrowserComponentFlags = juce::FileBrowserComponent::openMode
                                        | juce::FileBrowserComponent::canSelectFiles
                                        | juce::FileBrowserComponent::useTreeView
                                        | juce::FileBrowserComponent::filenameBoxIsReadOnly;
}

class CollectionBrowser : public juce::FileBrowserComponent,
public juce::Button::Listener
{
public:
    CollectionBrowser (MorphexAudioProcessor& processor) :
        FileBrowserComponent (Constants::FileBrowserComponentFlags,
                              Constants::CollectionDirectory,
                              new juce::WildcardFileFilter ("*.had", "*", "somedescription"),
                              nullptr),
        m_fileTreeComponent(static_cast<juce::FileTreeComponent*> (getDisplayComponent())),
        m_processor (processor)
    {
        setLookAndFeel (new MorphexLookAndFeel());
//
//        m_fileTreeComponent(getDisplayComponent()) = static_cast<FileTreeComponent*> (getDisplayComponent());
//        if (m_fileTreeComponent)
//        {
//            m_fileTreeComponent->setDragAndDropDescription ("DragAndDrop");
//        }

        m_loadAllButton.addListener (this);
        addAndMakeVisible (m_loadAllButton);

        resized();
    }

    ~CollectionBrowser() {}

    void paint (juce::Graphics& g) override
    {
        g.fillAll (GUI::Color::BrowserBackground);

        // Draw borders
        GUI::Paint::drawBorders (g, m_fileBrowserArea, GUI::Paint::BorderType::Glass);
    }

    void resized() override
    {
        FileBrowserComponent::resized();

        const float button_height = getHeight() * 0.075;

        m_loadAllButton.setBounds (0, getHeight() - button_height, getWidth(), button_height);

        m_fileBrowserArea.setBounds (0, button_height, getWidth(), getHeight() - (2 * button_height));

        getLocalBounds().removeFromBottom (button_height);

        //        auto b = getLocalBounds().reduced (80, 35);
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
    void fileClicked (const juce::File& f, const juce::MouseEvent& e) override
    {
        FileBrowserComponent::fileClicked (f, e);
        if (f.isDirectory())
        {
            m_fileTreeComponent->setDragAndDropDescription ("directory");
        }
        else
        {
            m_fileTreeComponent->setDragAndDropDescription (m_fileTreeComponent->getSelectedFile().getFullPathName());
        }
    }

    void buttonClicked (juce::Button* b) override
    {
        if (b == &m_loadAllButton)
        {
            const juce::File f = getHighlightedFile();
            //            const File f = getSelectedFile (0);

            m_processor.m_synth.m_instrument.reset();

            if (f.isDirectory())
            {
                // Loading selected folder
                m_processor.m_synth.m_instrument.loadAllSoundsFromFolder (f.getFullPathName().toStdString());
            }
            else
            {
                // Loading current folder
                m_processor.m_synth.m_instrument.loadAllSoundsFromFolder (getRoot().getFullPathName().toStdString());
            }
        }
    }
    
    juce::FileTreeComponent* m_fileTreeComponent;
    MorphexAudioProcessor& m_processor;

    juce::Rectangle<int>   m_fileBrowserArea;
    juce::Rectangle<float> m_rectangle;

    juce::Point<float> currentMouseXY;
    juce::Point<float> mouseDownXY;

    juce::String m_collectionsDirectory;
    juce::String m_pluginDirectory;

    juce::TextButton m_loadAllButton { "Load All Sounds" };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CollectionBrowser);
};
