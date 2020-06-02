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
{
public:
    PresetManagerPanel (SpectralMorphingToolAudioProcessor* inProcessor)
    :   mProcessor (inProcessor)
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
        mMenu->setText ("Menu");
//        mMenu->setText ("Menu", const NotificationType notification)
        //        mMenu->setBounds(comboBox_x, comboBox_y, comboBox_w, comboBox_h);
        mMenu->addListener (this);
        addAndMakeVisible (mMenu);
        
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
//        grid.templateAreas = { "button button button input control editor" };
        
        grid.autoColumns = Track (1_fr);
        grid.autoRows    = Track (1_fr);
//        grid.autoFlow = Grid::AutoFlow::column;
        
            grid.items.addArray ({
                GridItem (mSavePreset),
//                GridItem (mSavePreset),
//                GridItem (mSaveAsPreset),
                GridItem (mPresetDisplay),
                GridItem (mMenu),
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
    
    void buttonClicked (Button* b)
    {
        PresetManager* presetManager = mProcessor->getPresetManager();
        
        if (b == mSavePreset) {
            presetManager->savePreset();
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
        if (comboBoxThatHasChanged == mPresetDisplay) {
            
            PresetManager* presetManager = mProcessor->getPresetManager();
            
            const int index = mPresetDisplay->getSelectedItemIndex();
            presetManager->loadPreset(index);
        }
    }
    
    void displaySaveAsPopup()
    {
        PresetManager* presetManager = mProcessor->getPresetManager();
        
        String currentPresetName = presetManager->getCurrentPresetName();
        
        AlertWindow window ("Save As", "Please enter a name for your preset", AlertWindow::NoIcon);
        
        window.centreAroundComponent(this, getWidth(), getHeight());
        window.addTextEditor("presetName", currentPresetName, "Preset Name: ");
        window.addButton("Confirm", 1);
        window.addButton("Cancel", 0);
        
        if (window.runModalLoop()) {
            String presetName = window.getTextEditor("presetName")->getText();
            presetManager->saveAsPreset(presetName);
            updatePresetComboBox();
        }
    }
    
    void updatePresetComboBox()
    {
        PresetManager* presetManager = mProcessor->getPresetManager();
        String presetName = presetManager->getCurrentPresetName();
        
        mPresetDisplay->clear(dontSendNotification);
        
        const int numPresets = presetManager->getNumberOfPresets();
        
        for (int i=0; i < numPresets; i++) {
            mPresetDisplay->addItem(presetManager->getPresetName(i), (i+1));
        }
        
        mPresetDisplay->setText(presetManager->getCurrentPresetName());
    }
    
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
