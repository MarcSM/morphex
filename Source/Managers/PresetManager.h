/*
  ==============================================================================

    PresetManager.h
    Created: 7 Jul 2019 12:48:04pm
    Author:  Marc Sanchez Martinez

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"

#include "../Entities/MorphexSynth.h"

#include "../Core/Sound.h"

#define PRESET_FILE_EXTENSION ".mpf"

class PresetManager
{
public:
    
    PresetManager (AudioProcessor* inProcessor, MorphexSynth* inMorphexSynth);
    ~PresetManager();
    
    void getXmlForPreset (XmlElement* inElement);
    bool loadPresetFromXml (XmlElement* inElement);
    
    int getNumberOfPresets();
    String getPresetName (int inPresetIndex);
    
    void createNewPreset();
    void savePreset();
    void saveAsPreset (String inPresetName);
    bool loadPreset (int inPresetIndex);
    
    void getSoundInformation (XmlElement* inElement);
    bool setSoundInformation (XmlElement* presetBody);

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
    MorphexSynth* mMorphexSynth;
};
