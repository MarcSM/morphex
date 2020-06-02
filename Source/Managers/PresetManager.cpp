/*
  ==============================================================================

    PresetManager.cpp
    Created: 7 Jul 2019 12:48:04pm
    Author:  Marc Sanchez Martinez

  ================================¡==============================================
*/

#include "PresetManager.h"

#include "../SMTConstants.h"

#include "../SMTHelperFunctions.h"

PresetManager::PresetManager(AudioProcessor* inProcessor)
:   mCurrentPresetIsSaved(false),
mCurrentPresetName("Untitled"),
mProcessor(inProcessor)
{
    if (!File(PLUGIN_DATA_PRESETS_DIRECTORY).exists()) {
        File(PLUGIN_DATA_PRESETS_DIRECTORY).createDirectory();
    }
    
    storeLocalPreset();
}

PresetManager::~PresetManager()
{
    
}

void PresetManager::getXmlForPreset(XmlElement* inElement)
{
    // Get the plugin parameters
    auto& parameters = mProcessor->getParameters();
    
    for (int i=0; i < parameters.size(); i++) {
        
        AudioProcessorParameterWithID* parameter =
        (AudioProcessorParameterWithID*)parameters.getUnchecked(i);
        
        inElement->setAttribute(parameter->paramID,
                                parameter->getValue());
    }
}

void PresetManager::loadPresetForXml(XmlElement* inElement)
{
    mCurrentPresetXml = inElement;

    auto& parameters = mProcessor->getParameters();
    
    for (int i=0; i < mCurrentPresetXml->getNumAttributes(); i++) {
        
        const String paramId = mCurrentPresetXml->getAttributeName(i);
        const float value = mCurrentPresetXml->getDoubleAttribute(paramId);
        
        for (int j=0; j < parameters.size(); j++) {
            
            AudioProcessorParameterWithID* parameter =
            (AudioProcessorParameterWithID*)parameters.getUnchecked(j);
            
            if (paramId == parameter->paramID) {
                parameter->setValueNotifyingHost(value);
            }
        }
    }
}

int PresetManager::getNumberOfPresets()
{
    return mLocalPresets.size();
}

String PresetManager::getPresetName(int inPresetIndex)
{
    return mLocalPresets[inPresetIndex].getFileNameWithoutExtension();
}

void PresetManager::createNewPreset()
{
    auto& parameters = mProcessor->getParameters();
    
    for (int i=0; i < parameters.size(); i++) {
        
        AudioProcessorParameterWithID* parameter =
        (AudioProcessorParameterWithID*)parameters.getUnchecked(i);
        
        const float defaultValue =
        parameter->getDefaultValue();
        
        parameter->setValueNotifyingHost(defaultValue);
    }
    
    mCurrentPresetIsSaved = false;
    mCurrentPresetName = "Untitled";
}

void PresetManager::savePreset()
{
    MemoryBlock destinationData;
    mProcessor->getStateInformation(destinationData);
    
    mCurrentlyLoadedPreset.deleteFile();
    
    mCurrentlyLoadedPreset.appendData(destinationData.getData(),
                                      destinationData.getSize());
    
    mCurrentPresetIsSaved = true;
}

void PresetManager::saveAsPreset(String inPresetName)
{
    File presetFile = File(PLUGIN_DATA_PRESETS_DIRECTORY + directorySeparator + inPresetName + PRESET_FILE_EXTENSION);
    
    if (!presetFile.exists()) {
        presetFile.create();
    } else {
        presetFile.deleteFile();
    }
    
    MemoryBlock destinationData;
    mProcessor->getStateInformation(destinationData);
    
    presetFile.appendData(destinationData.getData(),
                          destinationData.getSize());
    
    mCurrentPresetIsSaved = true;
    mCurrentPresetName = inPresetName;
    
    storeLocalPreset();
}

void PresetManager::loadPreset(int inPresetIndex)
{
    mCurrentlyLoadedPreset = mLocalPresets[inPresetIndex];
    
    MemoryBlock presetBinary;
    
    if (mCurrentlyLoadedPreset.loadFileAsData(presetBinary)) {
        mCurrentPresetIsSaved = true;
        mCurrentPresetName = getPresetName(inPresetIndex);
        mProcessor->setStateInformation(presetBinary.getData(),
                                        (int)presetBinary.getSize());
    }
}

bool PresetManager::getIsCurrentPresetSaved()
{
    return mCurrentPresetIsSaved;
}

String PresetManager::getCurrentPresetName()
{
    return mCurrentPresetName;
}

void PresetManager::storeLocalPreset()
{
    mLocalPresets.clear();
    
    for (DirectoryIterator di (File(PLUGIN_DATA_PRESETS_DIRECTORY),
                               false,
                               "*" + (String)PRESET_FILE_EXTENSION,
                               File::TypesOfFileToFind::findFiles); di.next();)
    {
        File preset = di.getFile();
        mLocalPresets.add(preset);
    }
}