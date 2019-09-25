/*
  ==============================================================================

    SMTPresetManager.h
    Created: 7 Jul 2019 12:48:04pm
    Author:  Marc Sanchez Martinez

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"

#include "Sound.h"

#define PRESET_FILE_EXTENSION ".mpf"

class SMTPresetManager
{
public:
    
    SMTPresetManager(AudioProcessor* inProcessor);
    ~SMTPresetManager();
    
    void getXmlForPreset(XmlElement* inElement);
    void loadPresetForXml(XmlElement* inElement);
    
    int getNumberOfPresets();
    String getPresetName(int inPresetIndex);
    
    void createNewPreset();
    void savePreset();
    void saveAsPreset(String inPresetName);
    void loadPreset(int inPresetIndex);
    
    bool getIsCurrentPresetSaved();
    String getCurrentPresetName();
    
private:
    
    void storeLocalPreset();
    
    bool mCurrentPresetIsSaved;
    
    File mCurrentlyLoadedPreset;
    
    Array<File> mLocalPresets;
    
    String mCurrentPresetName;
    String mPresetDirectory;
    
    XmlElement* mCurrentPresetXml;
    
    AudioProcessor* mProcessor;
};
