/*
  ==============================================================================

    SMTPresetManager.cpp
    Created: 7 Jul 2019 12:48:04pm
    Author:  Marc Sanchez Martinez

  ================================¡==============================================
*/

#include "SMTPresetManager.h"

#include "SMTConstants.h"

#include "SMTHelperFunctions.h"

SMTPresetManager::SMTPresetManager(AudioProcessor* inProcessor)
:   mCurrentPresetIsSaved(false),
mCurrentPresetName("Untitled"),
mProcessor(inProcessor)
{
    if (!File(PLUGIN_DATA_PRESETS_DIRECTORY).exists()) {
        File(PLUGIN_DATA_PRESETS_DIRECTORY).createDirectory();
    }
    
    storeLocalPreset();
}

SMTPresetManager::~SMTPresetManager()
{
    
}

void SMTPresetManager::getXmlForPreset(XmlElement* inElement)
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

void SMTPresetManager::loadPresetForXml(XmlElement* inElement)
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

int SMTPresetManager::getNumberOfPresets()
{
    return mLocalPresets.size();
}

String SMTPresetManager::getPresetName(int inPresetIndex)
{
    return mLocalPresets[inPresetIndex].getFileNameWithoutExtension();
}

void SMTPresetManager::createNewPreset()
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

void SMTPresetManager::savePreset()
{
    MemoryBlock destinationData;
    mProcessor->getStateInformation(destinationData);
    
    mCurrentlyLoadedPreset.deleteFile();
    
    mCurrentlyLoadedPreset.appendData(destinationData.getData(),
                                      destinationData.getSize());
    
    mCurrentPresetIsSaved = true;
}

void SMTPresetManager::saveAsPreset(String inPresetName)
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

void SMTPresetManager::loadPreset(int inPresetIndex)
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

bool SMTPresetManager::getIsCurrentPresetSaved()
{
    return mCurrentPresetIsSaved;
}

String SMTPresetManager::getCurrentPresetName()
{
    return mCurrentPresetName;
}

void SMTPresetManager::storeLocalPreset()
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
