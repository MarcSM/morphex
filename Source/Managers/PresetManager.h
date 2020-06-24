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
