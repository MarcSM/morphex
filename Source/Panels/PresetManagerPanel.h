/*
  ==============================================================================

    PresetManagerPanel.h
    Created: 2 Jun 2020 9:36:52am
    Author:  Marc Sanchez Martinez

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include "../Components/Button.h"

class PresetManagerPanel
:   public Component,
    public Button::Listener,
    public ComboBox::Listener
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
    
    PresetManagerPanel (SpectralMorphingToolAudioProcessor* inProcessor)
    :   mPresetManager (inProcessor->getPresetManager()),
        mProcessor (inProcessor)
    {
        // Save Preset Button
        mSavePreset.setIcon (FontAwesome_Save);
        mSavePreset.addListener (this);
        addAndMakeVisible (mSavePreset);

        // Preset Display Combobox
        mPresetDisplay.addListener (this);
        mPresetDisplay.setTextWhenNothingSelected ("Untitled");
        updatePresetComboBox();
        addAndMakeVisible (mPresetDisplay);
        
        // Menu Combobox
        mMenu.onChange = [this] { onChangeMenuOption(); };
//        mMenu->addListener (this);
        updateMenuOptions();
        addAndMakeVisible (mMenu);

        // Information Panel
        addAndMakeVisible (informationPanel);
    }

    ~PresetManagerPanel() {}

    void paint (Graphics& g) override {}

    void resized() override
    {
        Grid grid;
        
        float grid_margin = getWidth() * 0.0f;
        
        grid.rowGap    = Grid::Px (grid_margin);
        grid.columnGap = Grid::Px (grid_margin);
        
        using Track = Grid::TrackInfo;
        
        grid.templateRows = {Track (1_fr),
                             Track (1_fr)};
        grid.templateColumns = {Track (1_fr), Track (6_fr), Track (2_fr)};
        grid.templateAreas = {
            "save preset menu",
            "info info info"
        };
        
        grid.autoColumns = Track (1_fr);
        grid.autoRows    = Track (1_fr);

        grid.items.addArray ({
            GridItem (mSavePreset).withArea ("save"),
            GridItem (mPresetDisplay).withArea ("preset"),
            GridItem (mMenu).withArea ("menu"),
            GridItem (informationPanel).withArea ("info")
        });
        
        grid.justifyContent = Grid::JustifyContent::spaceBetween;
        grid.justifyItems = Grid::JustifyItems::stretch;
        grid.alignContent = Grid::AlignContent::spaceAround;
        
        Rectangle<int> grid_bounds (grid_margin, grid_margin,
                                    getWidth() - ( grid_margin * 2.0f ),
                                    getHeight() - ( grid_margin * 2.0f ));
        
        grid.performLayout (grid_bounds);
    }

private:
    
    struct InformationPanel : public Component
    {
        InformationPanel () {}
        
        void paint (Graphics& g) override
        {
            // Draw borders
            GUI::Paint::drawBorders (g, getLocalBounds(), GUI::Paint::BorderType::Glass);
        }
        
        void resized() override {}
    };
    
    void updateMenuOptions()
    {
        mMenu.clear();
        mMenu.setTextWhenNothingSelected ("Menu");
        mMenu.addItem ("Init", MenuOption::Init);
        mMenu.addSeparator();
        mMenu.addItem ("Save", MenuOption::Save);
        mMenu.addItem ("Save as", MenuOption::SaveAs);
    }
    
    void onChangeMenuOption()
    {
        int selected_id = mMenu.getSelectedId();

        switch (selected_id)
        {
            case MenuOption::None:                                 break;
            case MenuOption::Init:   displayInitPopup();           break;
            case MenuOption::Save:   mPresetManager->savePreset(); break;
            case MenuOption::SaveAs: displaySaveAsPopup();         break;
            default:                 jassertfalse;                 break;
        }

        if (selected_id != MenuOption::None) mMenu.setSelectedId (MenuOption::None);
    }
    
    void buttonClicked (Button* b) override
    {
        if (b == &mSavePreset) mPresetManager->savePreset();

        // TODO
//        if (b == mNewPreset) {
//            presetManager->createNewPreset();
//            updatePresetComboBox();
//        }
//        else if (b == mSavePreset) {
//            presetManager->savePreset();
//        }
//        else if (b == mSaveAsPreset) {
//            displaySaveAsPopup();
//        }
    }
    
    void comboBoxChanged (ComboBox* comboBoxThatHasChanged) override
    {
        if (comboBoxThatHasChanged == &mPresetDisplay)
        {
            const int index = mPresetDisplay.getSelectedItemIndex();
            
            if (index > -1)
            {
                if (mPresetManager->loadPreset (index))
                {
                    this->currentPresetIndex = index;
                }
                else
                {
                    mPresetDisplay.setSelectedItemIndex (this->currentPresetIndex, NotificationType::dontSendNotification);
                }
            }
        }

        if (comboBoxThatHasChanged == &mMenu)
        {
            // TODO
//            const int index = mPresetDisplay->getSelectedItemIndex();
//            presetManager->loadPreset(index);
        }
    }
    
    void displayInitPopup()
    {
        String currentPresetName = mPresetManager->getCurrentPresetName();

        AlertWindow window ("Init", "Are you sure you want to initialize this preset?", AlertWindow::NoIcon);

        window.centreAroundComponent (this, getWidth(), getHeight());
        window.addButton ("Confirm", 1);
        window.addButton ("Cancel", 0);

        if (window.runModalLoop())
        {
            mPresetManager->createNewPreset();
            updatePresetComboBox();
        }
    }
    
    void displaySaveAsPopup()
    {
        String currentPresetName = mPresetManager->getCurrentPresetName();

        AlertWindow window ("Save As", "Please enter a name for your preset", AlertWindow::NoIcon);

        window.centreAroundComponent (this, getWidth(), getHeight());
        window.addTextEditor ("presetName", currentPresetName, "Preset Name: ");
        window.addButton ("Confirm", 1);
        window.addButton ("Cancel", 0);

        if (window.runModalLoop())
        {
            String presetName = window.getTextEditor ("presetName")->getText();
            mPresetManager->saveAsPreset (presetName);
            updatePresetComboBox();
        }
    }
    
    void updatePresetComboBox()
    {
        this->currentPresetIndex = mPresetDisplay.getSelectedItemIndex();
        
        String presetName = mPresetManager->getCurrentPresetName();

        mPresetDisplay.clear (dontSendNotification);

        const int numPresets = mPresetManager->getNumberOfPresets();

        for (int i=0; i < numPresets; i++)
        {
            mPresetDisplay.addItem (mPresetManager->getPresetName(i), (i+1));
        }
    }
    
    PresetManager* mPresetManager;
    
    InformationPanel informationPanel;
    
    Morphex::Button mSavePreset;
    ComboBox mPresetDisplay;
    ComboBox mMenu;
    
    int currentPresetIndex = 0;

    SpectralMorphingToolAudioProcessor* mProcessor;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PresetManagerPanel)
};
