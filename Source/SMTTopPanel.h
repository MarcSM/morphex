/*
  ==============================================================================

    SMTTopPanel.h
    Created: 14 Jun 2019 7:45:45pm
    Author:  Marc Sanchez Martinez

  ==============================================================================
*/

#pragma once

#include "SMTPanelBase.h"

class SMTTopPanel
:   public SMTPanelBase,
    public Button::Listener,
    public ComboBox::Listener

{
public:
    
    SMTTopPanel(SpectralMorphingToolAudioProcessor* inProcessor);
    ~SMTTopPanel();
    
    void paint(Graphics& g) override;
    
    void buttonClicked(Button*) override;
    
    void comboBoxChanged(ComboBox* comboBoxThatHasChanged) override;
    
private:
    
    void displaySaveAsPopup();
    
    void updatePresetComboBox();
    
    ScopedPointer<ComboBox> mPresetDisplay;

    ScopedPointer<TextButton> mNewPreset, mSavePreset, mSaveAsPreset;
};
