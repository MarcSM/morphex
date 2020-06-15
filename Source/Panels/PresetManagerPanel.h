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

//==============================================================================
/*
*/
class PresetManagerPanel
:   public Component,
    public Button::Listener,
    public ComboBox::Listener
//    public AsyncUpdater
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
    :   mProcessor (inProcessor), mPresetManager (inProcessor->getPresetManager())
    {
        // Save Preset Button
        mSavePreset = new Morphex::Button();
        mSavePreset->setIcon (FontAwesome_Save);
//        mSavePreset->setBounds (button_x, button_y, button_w, button_h);
        mSavePreset->addListener (this);
        addAndMakeVisible (mSavePreset);
        
//        mNewPreset = new ImageButton();
//        mNewPreset = new DrawableButton ("New Preset", DrawableButton::ButtonStyle::ImageFitted);
//        mNewPreset = new DrawableButton ("New Preset", DrawableButton::ButtonStyle::ImageFitted);
//        mNewPreset = new Button ("New Preset");
//        mNewPreset->setFont (FontAwesome::getInstance()->getFont());
//        RenderedIcon aux = FontAwesome().getIcon (FontAwesome_Save, 80, Colours::white, 1.0f);
////        RenderedIcon aux = FontAwesome::getInstance()->getIcon (FontAwesome_Save, 80, Colours::white, 1.0f);
//        new_preset_icon = new DrawableImage();
//        new_preset_icon->setImage (aux);
        
//        svg_test = Drawable::createFromImageData (BinaryData::morphex_long_logo_svg, BinaryData::morphex_long_logo_svgSize);
        
//        mNewPreset->setImages (svg_test.get());

//        RenderedIcon getIcon(Icon icon, float size, juce::Colour colour, float scaleFactor = 1.0f);
//        RenderedIcon getIcon(FontAwesome_Play, 24, Colours::white, 1.0f);
//        FontAwesome::drawAt(g, FontAwesome_Play, 24, Colours::white, 20, 20);
        
//        mNewPreset->setButtonText("N");
//        mNewPreset->setBounds(button_x, button_y, button_w, button_h);
//        mNewPreset->addListener(this);
//        addAndMakeVisible(mNewPreset);
//        button_x += button_w;
        
//        // Save Preset Button
//        mSavePreset = new TextButton();
//        mSavePreset->setButtonText("S");
//        mSavePreset->setBounds(button_x, button_y, button_w, button_h);
//        mSavePreset->addListener(this);
//        addAndMakeVisible(mSavePreset);
//        button_x += button_w;
//
//        // Save Preset As Button
//        mSaveAsPreset = new TextButton();
//        mSaveAsPreset->setButtonText("SA");
//        mSaveAsPreset->setBounds(button_x, button_y, button_w, button_h);
//        mSaveAsPreset->addListener(this);
//        addAndMakeVisible(mSaveAsPreset);
//        button_x += button_w;
        
        // Preset Combo Box
//        const int comboBox_x = button_x + 1;
//        const int comboBox_y = button_y + 1;
//        const int comboBox_w = 140;
//        const int comboBox_h = button_h - 2;
        
        mPresetDisplay = new ComboBox();
//        mPresetDisplay->setBounds(comboBox_x, comboBox_y, comboBox_w, comboBox_h);
        mPresetDisplay->addListener (this);
        addAndMakeVisible (mPresetDisplay);
        
        updatePresetComboBox();
        
        mMenu = new ComboBox();
//        mMenu->setText ("Menu", const NotificationType notification)
        //        mMenu->setBounds(comboBox_x, comboBox_y, comboBox_w, comboBox_h);
        mMenu->onChange = [this] { onChangeMenuOption(); };
//        mMenu->addListener (this);
        addAndMakeVisible (mMenu);
        updateMenuOptions();
        
        // Information Panel
        addAndMakeVisible (informationPanel);
    }

    ~PresetManagerPanel()
    {
    }

    void paint (Graphics& g) override
    {
//        FontAwesome::drawAt (g, FontAwesome_Play, 24, Colours::white, 20, 20);
    }

    void resized() override
    {
//        DrawablePath
        
//        new_preset_icon = FontAwesome::getInstance()->getIcon (FontAwesome_Save, 80, Colours::white, 1.0f);
////        new_preset_icon = FontAwesome().getIcon (FontAwesome_Save, 80, Colours::white, 1.0f);
//        DrawableImage convert;
//        convert.setImage(ImageCache::...);
//
//        mNewPreset->setImages (new_preset_icon);
        
//        mNewPreset->setImages (false, true, true,
//                               save_icon, 0.7f, Colours::transparentBlack,
//                               save_icon, 1.0f, Colours::transparentBlack,
//                               save_icon, 1.0f, Colours::white.withAlpha (0.8f),
//                               0.5f);
        
        
        Grid grid;
        
        float grid_margin = getWidth() * 0.0f;
//        float grid_margin = getWidth() * 0.04f;

        //        Grid::Px row_gap = Grid::Px (grid_margin);
        //        Grid::Px column_gap = Grid::Px (grid_margin);
        
        grid.rowGap    = Grid::Px (grid_margin);;
        grid.columnGap = Grid::Px (grid_margin);;
        
        //        grid.rowGap    = 20_px;
        //        grid.columnGap = 20_px;
        
        using Track = Grid::TrackInfo;
        
//        grid.templateRows = { Track(1_fr), Track(2_fr)};
//        grid.templateColumns = { Track(2_fr), Track(1_fr), Track(3_fr) };
//        grid.templateAreas = { "input control preset",
//            "input control editor" };
//        grid.items = { GridItem(inputPanel).withArea("input"),
//            GridItem(masterControlPanel).withArea("control"),
//            GridItem(presetChooserPanel).withArea("preset"),
//            GridItem(presetEditorPanel).withArea("editor") };
//
        grid.templateRows = {Track (1_fr), Track (1_fr)};
        
        grid.templateColumns = {Track (1_fr), Track (6_fr), Track (2_fr)};
//                                Track (1_fr), Track (1_fr), Track (1_fr), Track (1_fr)};
        grid.templateAreas = {
            "save preset menu",
            "info info info"
        };
        
        grid.autoColumns = Track (1_fr);
        grid.autoRows    = Track (1_fr);
//        grid.autoFlow = Grid::AutoFlow::column;
        
            grid.items.addArray ({
                GridItem (mSavePreset).withArea("save"),
//                GridItem (mSavePreset),
//                GridItem (mSaveAsPreset),
                GridItem (mPresetDisplay).withArea("preset"),
                GridItem (mMenu).withArea("menu"),
                GridItem (informationPanel).withArea("info")
            });
        
        grid.justifyContent = Grid::JustifyContent::spaceBetween;
        grid.justifyItems = Grid::JustifyItems::stretch;
        grid.alignContent = Grid::AlignContent::spaceAround;
//        grid.justifyContent = Grid::JustifyContent::spaceAround;
        
        
        Rectangle<int> grid_bounds(grid_margin, grid_margin,
                                   getWidth() - ( grid_margin * 2.0f ),
                                   getHeight() - ( grid_margin * 2.0f ));
        grid.performLayout (grid_bounds);
        
//        int marign = 8;
//
//        int button_x = marign;
//        int button_y = marign;
//        int button_w = 30;
//        int button_h = getHeight() - (2 * marign);
//
//        // New Preset Button
//        mNewPreset->setBounds(button_x, button_y, button_w, button_h);
//        button_x += button_w;
//
//        // Save Preset Button
//        mSavePreset->setBounds(button_x, button_y, button_w, button_h);
//        button_x += button_w;
//
//        // Save Preset As Button
//        mSaveAsPreset->setBounds(button_x, button_y, button_w, button_h);
//        button_x += button_w;
//
//        // Preset Combo Box
//        const int comboBox_x = button_x + 1;
//        const int comboBox_y = button_y + 1;
//        const int comboBox_w = 140;
//        const int comboBox_h = button_h - 2;
//
//        mPresetDisplay->setBounds(comboBox_x, comboBox_y, comboBox_w, comboBox_h);
    }

private:
    
    struct InformationPanel : public Component
    {
        InformationPanel ()
        {
            
        }
        
        void paint (Graphics& g) override
        {
            // Draw borders
            GUI::Paint::drawBorders (g, getLocalBounds(), GUI::Paint::BorderType::Glass);
        }
        
        void resized() override
        {
            
        }
    };
    
    void updateMenuOptions()
    {
        mMenu->clear();
        mMenu->setTextWhenNothingSelected ("Menu");
        mMenu->addItem ("Init", MenuOption::Init);
        mMenu->addSeparator();
        mMenu->addItem ("Save", MenuOption::Save);
        mMenu->addItem ("Save as", MenuOption::SaveAs);
//        mMenu.addSeparator();
    }
    
    void onChangeMenuOption()
    {
        int selected_id = mMenu->getSelectedId();
        
        switch (selected_id)
        {
            case MenuOption::None:   break;
            case MenuOption::Init:   displayInitPopup();           break;
            case MenuOption::Save:   mPresetManager->savePreset(); break;
            case MenuOption::SaveAs: displaySaveAsPopup();         break;
            default:                 jassertfalse; break;
        }
        
        if (selected_id != MenuOption::None) mMenu->setSelectedId (MenuOption::None);
    }
    
    void buttonClicked (Button* b)
    {
//        PresetManager* presetMana'ger = mProcessor->getPresetManager();
        
        if (b == mSavePreset) {
            mPresetManager->savePreset();
        }

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
    
    void comboBoxChanged (ComboBox* comboBoxThatHasChanged)
    {
        if (comboBoxThatHasChanged == mPresetDisplay)
        {
//            PresetManager* presetManager = mProcessor->getPresetManager();
//
            const int index = mPresetDisplay->getSelectedItemIndex();
            mPresetManager->loadPreset (index);
        }
        
        if (comboBoxThatHasChanged == mMenu) {

            DBG ("HERE");
//            PresetManager* presetManager = mProcessor->getPresetManager();
//
//            const int index = mPresetDisplay->getSelectedItemIndex();
//            presetManager->loadPreset(index);
        }
    }
    
    void displayInitPopup()
    {
//        PresetManager* presetManager = mProcessor->getPresetManager();
        
        String currentPresetName = mPresetManager->getCurrentPresetName();
        
        AlertWindow window ("Init", "Are you sure you want to initialize this preset?", AlertWindow::NoIcon);
        
        window.centreAroundComponent (this, getWidth(), getHeight());
        window.addButton ("Confirm", 1);
        window.addButton ("Cancel", 0);
        
        if (window.runModalLoop())
        {
            mPresetManager->createNewPreset();
            updatePresetComboBox();
            // TODO - Repaint the whole GUI
//            triggerAsyncUpdate();
        }
    }
    
    void displaySaveAsPopup()
    {
//        PresetManager* presetManager = mProcessor->getPresetManager();
        
        String currentPresetName = mPresetManager->getCurrentPresetName();
        
        AlertWindow window ("Save As", "Please enter a name for your preset", AlertWindow::NoIcon);
        
        window.centreAroundComponent(this, getWidth(), getHeight());
        window.addTextEditor("presetName", currentPresetName, "Preset Name: ");
        window.addButton("Confirm", 1);
        window.addButton("Cancel", 0);
        
        if (window.runModalLoop()) {
            String presetName = window.getTextEditor("presetName")->getText();
            mPresetManager->saveAsPreset(presetName);
            updatePresetComboBox();
        }
    }
    
    void updatePresetComboBox()
    {
//        PresetManager* presetManager = mProcessor->getPresetManager();
        String presetName = mPresetManager->getCurrentPresetName();
        
        mPresetDisplay->clear(dontSendNotification);
        
        const int numPresets = mPresetManager->getNumberOfPresets();
        
        for (int i=0; i < numPresets; i++) {
            mPresetDisplay->addItem(mPresetManager->getPresetName(i), (i+1));
        }
        
        mPresetDisplay->setText(mPresetManager->getCurrentPresetName());
    }
    
    PresetManager* mPresetManager;
    
    InformationPanel informationPanel;
    
    ScopedPointer<Morphex::Button> mSavePreset;
    
//    ScopedPointer<ImageButton> mNewPreset;
//    ScopedPointer<FontAwesome> fontAwesome;
//    ScopedPointer<DrawableButton> mNewPreset;
//    DrawableImage* new_preset_icon;
//    RenderedIcon* new_preset_icon;
    
    ScopedPointer<ComboBox> mPresetDisplay;
    ScopedPointer<ComboBox> mMenu;

//    ScopedPointer<TextButton> mSavePreset, mSaveAsPreset;

    SpectralMorphingToolAudioProcessor* mProcessor;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PresetManagerPanel)
};
