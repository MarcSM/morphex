/*
 ==============================================================================
 
 SMTTopPanel.cpp
 Created: 7 May 2019 12:45:49pm
 Author:  Marc Sanchez Martinez
 
 ==============================================================================
 */

#include "SMTTopPanel.h"

#include "SMTConstants.h"

SMTTopPanel::SMTTopPanel(SpectralMorphingToolAudioProcessor* inProcessor)
:   SMTPanelBase()
//:   SMTPanelBase(inProcessor)
{
    setSize(TOP_PANEL_WIDTH, TOP_PANEL_HEIGHT);
    
    int marign = 8;
    
    int button_x = marign;
    int button_y = marign;
    int button_w = 30;
    int button_h = getHeight() - (2 * marign);
    
    // New Preset Button
    mNewPreset = new TextButton();
    mNewPreset->setButtonText("N");
    mNewPreset->setBounds(button_x, button_y, button_w, button_h);
    mNewPreset->addListener(this);
    addAndMakeVisible(mNewPreset);
    button_x += button_w;
    
    // Save Preset Button
    mSavePreset = new TextButton();
    mSavePreset->setButtonText("S");
    mSavePreset->setBounds(button_x, button_y, button_w, button_h);
    mSavePreset->addListener(this);
    addAndMakeVisible(mSavePreset);
    button_x += button_w;
    
    // Save Preset As Button
    mSaveAsPreset = new TextButton();
    mSaveAsPreset->setButtonText("SA");
    mSaveAsPreset->setBounds(button_x, button_y, button_w, button_h);
    mSaveAsPreset->addListener(this);
    addAndMakeVisible(mSaveAsPreset);
    button_x += button_w;
    
    // Preset Combo Box
    const int comboBox_x = button_x + 1;
    const int comboBox_y = button_y + 1;
    const int comboBox_w = 140;
    const int comboBox_h = button_h - 2;
    
    mPresetDisplay = new ComboBox();
    mPresetDisplay->setBounds(comboBox_x, comboBox_y, comboBox_w, comboBox_h);
    mPresetDisplay->addListener(this);
    addAndMakeVisible(mPresetDisplay);
    
    updatePresetComboBox();
    
    // Morphex GitHub Hyperlink
    HyperlinkButton* morphex_github_hyperlink
    = new HyperlinkButton ("github.com/MarcSM/Morphex",
                           URL ("https://github.com/MarcSM/Morphex"));
    
    morphex_github_hyperlink->setBounds (getWidth()-200-10, 4, 200, 25);
    morphex_github_hyperlink->setFont (16.00f, false, Justification::right);
    addAndMakeVisible (morphex_github_hyperlink);
}

SMTTopPanel::SMTTopPanel()
:   SMTPanelBase()
{
    
}

SMTTopPanel::~SMTTopPanel()
{
    
}

void SMTTopPanel::paint(Graphics& g)
{
//    SMTPanelBase::paint(g);
//
//    // Morphex Logo
//    g.setColour(SMTColour_Logo);
//    g.setFont(font_5);
//    g.drawText("M  O  R  P  H  E  X", 0, 0, getWidth(), getHeight(), Justification::centred);
//
//    // GitHub Logo
//    Image github_logo_small = ImageCache::getFromMemory(BinaryData::github_logo_small_png, BinaryData::github_logo_small_pngSize);
//    github_logo_small = github_logo_small.rescaled (25, 25, Graphics::mediumResamplingQuality);
//    g.drawImageAt(github_logo_small, getWidth()-240, 10);
//
//    // Plugin Version
//    g.setFont(font_1);
//    g.drawText("v " + PLUGIN_VERSION, 0, 9, getWidth()-11, getHeight(), Justification::right);
}

void SMTTopPanel::buttonClicked (Button* b)
{
//    SMTPresetManager* presetManager = mProcessor->getPresetManager();
//
//    if (b == mNewPreset) {
//        presetManager->createNewPreset();
//        updatePresetComboBox();
//    }
//    else if (b == mSavePreset) {
//        presetManager->savePreset();
//    }
//    else if (b == mSaveAsPreset) {
//        displaySaveAsPopup();
//    }
}

void SMTTopPanel::comboBoxChanged (ComboBox* comboBoxThatHasChanged)
{
//    if (comboBoxThatHasChanged == mPresetDisplay) {
//
//        SMTPresetManager* presetManager = mProcessor->getPresetManager();
//
//        const int index = mPresetDisplay->getSelectedItemIndex();
//        presetManager->loadPreset(index);
//    }
}

void SMTTopPanel::displaySaveAsPopup()
{
//    SMTPresetManager* presetManager = mProcessor->getPresetManager();
//
//    String currentPresetName = presetManager->getCurrentPresetName();
//
//    AlertWindow window ("Save As", "Please enter a name for your preset", AlertWindow::NoIcon);
//
//    window.centreAroundComponent(this, getWidth(), getHeight());
//    window.addTextEditor("presetName", currentPresetName, "Preset Name: ");
//    window.addButton("Confirm", 1);
//    window.addButton("Cancel", 0);
//
//    if (window.runModalLoop()) {
//        String presetName = window.getTextEditor("presetName")->getText();
//        presetManager->saveAsPreset(presetName);
//        updatePresetComboBox();
//    }
}

void SMTTopPanel::updatePresetComboBox()
{
//    SMTPresetManager* presetManager = mProcessor->getPresetManager();
//    String presetName = presetManager->getCurrentPresetName();
//
//    mPresetDisplay->clear(dontSendNotification);
//
//    const int numPresets = presetManager->getNumberOfPresets();
//
//    for (int i=0; i < numPresets; i++) {
//        mPresetDisplay->addItem(presetManager->getPresetName(i), (i+1));
//    }
//
//    mPresetDisplay->setText(presetManager->getCurrentPresetName());
}
