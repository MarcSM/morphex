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

#include "../components/button.h"

class PresetManagerPanel : public juce::Component,
public juce::Button::Listener,
public juce::ComboBox::Listener
{
public:
    enum MenuOption
    {
        None = 0,
        Init,
        Save,
        SaveAs,
        NUM_OPTIONS
    };

    PresetManagerPanel (MorphexAudioProcessor& processor) :
        m_presetManager (processor.getPresetManager()),
        m_processor (processor)
    {
        // Save Preset Button
        m_savePreset.setIcon (FontAwesome_Save);
        m_savePreset.addListener (this);
        addAndMakeVisible (m_savePreset);

        // Preset Display Combobox
        m_presetDisplay.addListener (this);
        m_presetDisplay.setTextWhenNothingSelected ("Untitled");
        updatePresetComboBox();
        addAndMakeVisible (m_presetDisplay);

        // Menu Combobox
        m_menu.onChange = [this] { onChangeMenuOption(); };
        //        m_menu->addListener (this);
        updateMenuOptions();
        addAndMakeVisible (m_menu);

        // Information Panel
        addAndMakeVisible (m_informationPanel);
    }

    ~PresetManagerPanel() {}

    void paint (juce::Graphics& g) override {}

    void resized() override
    {
        juce::Grid grid;

        float grid_margin = getWidth() * 0.0f;

        grid.rowGap    = juce::Grid::Px (grid_margin);
        grid.columnGap = juce::Grid::Px (grid_margin);

        using Track = juce::Grid::TrackInfo;
        using juce::operator""_fr;

        grid.templateRows    = { Track (1_fr),
                              Track (1_fr) };
        grid.templateColumns = { Track (1_fr), Track (6_fr), Track (2_fr) };
        grid.templateAreas   = {
            "save preset menu",
            "info info info"
        };

        grid.autoColumns = Track (1_fr);
        grid.autoRows    = Track (1_fr);

        grid.items.addArray ({ juce::GridItem (m_savePreset).withArea ("save"),
            juce::GridItem (m_presetDisplay).withArea ("preset"),
            juce::GridItem (m_menu).withArea ("menu"),
            juce::GridItem (m_informationPanel).withArea ("info") });

        grid.justifyContent = juce::Grid::JustifyContent::spaceBetween;
        grid.justifyItems   = juce::Grid::JustifyItems::stretch;
        grid.alignContent   = juce::Grid::AlignContent::spaceAround;

        juce::Rectangle<int> grid_bounds (grid_margin, grid_margin, getWidth() - (grid_margin * 2.0f), getHeight() - (grid_margin * 2.0f));

        grid.performLayout (grid_bounds);
    }

private:
    struct InformationPanel : public juce::Component
    {
        InformationPanel() {}

        void paint (juce::Graphics& g) override
        {
            // Draw borders
            GUI::Paint::drawBorders (g, getLocalBounds(), GUI::Paint::BorderType::Glass);
        }

        void resized() override {}
    };

    void updateMenuOptions()
    {
        m_menu.clear();
        m_menu.setTextWhenNothingSelected ("Menu");
        m_menu.addItem ("Init", MenuOption::Init);
        m_menu.addSeparator();
        m_menu.addItem ("Save", MenuOption::Save);
        m_menu.addItem ("Save as", MenuOption::SaveAs);
    }

    void onChangeMenuOption()
    {
        int selected_id = m_menu.getSelectedId();

        switch (selected_id)
        {
            case MenuOption::None:
                break;
            case MenuOption::Init:
                displayInitPopup();
                break;
            case MenuOption::Save:
                m_presetManager.savePreset();
                break;
            case MenuOption::SaveAs:
                displaySaveAsPopup();
                break;
            default:
                jassertfalse;
                break;
        }

        if (selected_id != MenuOption::None)
            m_menu.setSelectedId (MenuOption::None);
    }

    void buttonClicked (juce::Button* b) override
    {
        if (b == &m_savePreset)
            m_presetManager.savePreset();

        // TODO
        //        if (b == mNewPreset) {
        //            presetManager->createNewPreset();
        //            updatePresetComboBox();
        //        }
        //        else if (b == m_savePreset) {
        //            presetManager->savePreset();
        //        }
        //        else if (b == mSaveAsPreset) {
        //            displaySaveAsPopup();
        //        }
    }

    void comboBoxChanged (juce::ComboBox* comboBoxThatHasChanged) override
    {
        if (comboBoxThatHasChanged == &m_presetDisplay)
        {
            const int index = m_presetDisplay.getSelectedItemIndex();

            if (index > -1)
            {
                if (m_presetManager.loadPreset (index))
                {
                    this->m_currentPresetIndex = index;
                }
                else
                {
                    m_presetDisplay.setSelectedItemIndex (this->m_currentPresetIndex, juce::NotificationType::dontSendNotification);
                }
            }
        }

        if (comboBoxThatHasChanged == &m_menu)
        {
            // TODO
            //            const int index = m_presetDisplay->getSelectedItemIndex();
            //            presetManager->loadPreset(index);
        }
    }

    void displayInitPopup()
    {
        juce::String currentPresetName = m_presetManager.getCurrentPresetName();

        juce::AlertWindow window ("Init", "Are you sure you want to initialize this preset?", juce::AlertWindow::NoIcon);

        window.centreAroundComponent (this, getWidth(), getHeight());
        window.addButton ("Confirm", 1);
        window.addButton ("Cancel", 0);

        if (window.runModalLoop())
        {
            m_presetManager.createNewPreset();
            updatePresetComboBox();
        }
    }

    void displaySaveAsPopup()
    {
        juce::String currentPresetName = m_presetManager.getCurrentPresetName();

        juce::AlertWindow window ("Save As", "Please enter a name for your preset", juce::AlertWindow::NoIcon);

        window.centreAroundComponent (this, getWidth(), getHeight());
        window.addTextEditor ("presetName", currentPresetName, "Preset Name: ");
        window.addButton ("Confirm", 1);
        window.addButton ("Cancel", 0);

        if (window.runModalLoop())
        {
            juce::String presetName = window.getTextEditor ("presetName")->getText();
            m_presetManager.saveAsPreset (presetName);
            updatePresetComboBox();
        }
    }

    void updatePresetComboBox()
    {
        this->m_currentPresetIndex = m_presetDisplay.getSelectedItemIndex();

        juce::String presetName = m_presetManager.getCurrentPresetName();

        m_presetDisplay.clear (juce::dontSendNotification);

        const int numPresets = m_presetManager.getNumberOfPresets();

        for (int i = 0; i < numPresets; i++)
        {
            m_presetDisplay.addItem (m_presetManager.getPresetName (i), (i + 1));
        }
    }

    morphex::PresetManager& m_presetManager;

    InformationPanel m_informationPanel;

    morphex::Button m_savePreset;
    juce::ComboBox        m_presetDisplay;
    juce::ComboBox        m_menu;

    int m_currentPresetIndex = 0;

    MorphexAudioProcessor& m_processor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PresetManagerPanel)
};
