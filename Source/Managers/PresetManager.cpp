/*
  ==============================================================================

    PresetManager.cpp
    Created: 7 Jul 2019 12:48:04pm
    Author:  Marc Sanchez Martinez

  ================================¡==============================================
*/

#include "PresetManager.h"

#include "../Helpers/SMTConstants.h"

#include "../Helpers/SMTHelperFunctions.h"

PresetManager::PresetManager (AudioProcessor* inProcessor, MorphexSynth* inMorphexSynth)
:   mCurrentPresetIsSaved (false),
    mCurrentPresetName ("Untitled"),
    mProcessor (inProcessor),
    mMorphexSynth (inMorphexSynth)
{
    if (!File (PLUGIN_DATA_PRESETS_DIRECTORY).exists())
    {
        File (PLUGIN_DATA_PRESETS_DIRECTORY).createDirectory();
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
    
//    // Save morph sound's file path
//    MorphSounds morph_sounds = this->mMorphexSynth->instrument.getMorphSounds();
//
//    for (int i = 0; i < morph_sounds.size(); i++)
//    {
//        std::string sound_file_path = morph_sounds[i]->path;
//        removeSubStr (sound_file_path, (PLUGIN_DATA_COLLECTIONS_DIRECTORY + directorySeparator).toStdString());
//        String sound_file_path_id = SOUND_FILE_PATH_PARAMETER_ID + String (i + 1);
//        presetBody->setAttribute (sound_file_path_id, sound_file_path);
//    }
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
    
    mMorphexSynth->reset();
    
    mCurrentPresetIsSaved = false;
    mCurrentPresetName = "Untitled";
}

void PresetManager::savePreset()
{
    MemoryBlock destinationData;
    mProcessor->getStateInformation(destinationData);
    this->getSoundInformation (destinationData);
    
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
    mProcessor->getStateInformation (destinationData);
    this->getSoundInformation (destinationData);
    
    presetFile.appendData (destinationData.getData(),
                           destinationData.getSize());
    
    mCurrentPresetIsSaved = true;
    mCurrentPresetName = inPresetName;
    
    storeLocalPreset();
}

// TODO
void PresetManager::getSoundInformation (MemoryBlock& destData)
{
    std::unique_ptr<XmlElement> preset = AudioPluginInstance::getXmlFromBinary (destData.getData(), (int) destData.getSize());
    
    XmlElement* presetBody = preset->getChildByName("MPF_Preset");
    
    // Save morph sound's file path
    MorphSounds morph_sounds = this->mMorphexSynth->instrument.getMorphSounds();

    for (int i = 0; i < morph_sounds.size(); i++)
    {
        std::string sound_file_path = morph_sounds[i]->path;
        removeSubStr (sound_file_path, (PLUGIN_DATA_COLLECTIONS_DIRECTORY + directorySeparator).toStdString());
        String sound_file_path_id = SOUND_FILE_PATH_PARAMETER_ID + String (i + 1);
        
        presetBody->setAttribute (sound_file_path_id, sound_file_path);
    }
    
    juce::AudioPluginInstance::copyXmlToBinary (*preset, destData);
}

bool PresetManager::loadPreset (int inPresetIndex)
{
    bool present_was_loaded = false;
    
    mCurrentlyLoadedPreset = mLocalPresets[inPresetIndex];
    
    MemoryBlock presetBinary;
    
    if (mCurrentlyLoadedPreset.loadFileAsData (presetBinary))
    {
        bool sounds_can_be_loaded = this->setSoundInformation (presetBinary.getData(), (int) presetBinary.getSize());
        
        if (sounds_can_be_loaded)
        {
            mCurrentPresetIsSaved = true;
            mCurrentPresetName = getPresetName (inPresetIndex);
            mProcessor->setStateInformation (presetBinary.getData(), (int) presetBinary.getSize());
            present_was_loaded = true;
        }
    }
    
    return present_was_loaded;
}

bool PresetManager::setSoundInformation (const void* data, int sizeInBytes)
{
    bool sounds_can_be_loaded = true;
    
    std::unique_ptr<XmlElement> xmlState;
    xmlState = juce::AudioPluginInstance::getXmlFromBinary (data, sizeInBytes);
    
    if (xmlState != nullptr)
    {
        forEachXmlChildElement (*xmlState, subChild)
        {
            // Get morph sound's file path
            MorphSounds morph_sounds = this->mMorphexSynth->instrument.getMorphSounds();
            std::vector<std::string> sound_file_paths_to_load;
            
            // Error vars
            String error_message = "The preset couldn't be loaded, the following sounds are not in your library:\n";
            
            // Checking of the sounds exist
            for (int i = 0; i < morph_sounds.size(); i++)
            {
                // Generate sound ID
                String sound_file_path_id = SOUND_FILE_PATH_PARAMETER_ID + String (i + 1);
                
                // Retrieve the sound file path
                std::string sound_file_path = subChild->getStringAttribute (sound_file_path_id).toStdString();
                
                // If it's not empty
                if (!sound_file_path.empty())
                {
                    // Prepend the user's plugin data directory
                    sound_file_path = (PLUGIN_DATA_COLLECTIONS_DIRECTORY + directorySeparator).toStdString() + sound_file_path;
                    
                    // Check if sound file path is valid
                    File sound_file_path_check (sound_file_path);
                    
                    if (!sound_file_path_check.existsAsFile())
                    {
                        sounds_can_be_loaded = false;
                        error_message += String ("\n" + sound_file_path);
                    }
                    else
                    {
                        sound_file_paths_to_load.push_back (sound_file_path);
                        
//                        // Remove the sound file path element from the xml
//                        subChild->removeAttribute ( String(sound_file_path_id) );
                    }
                }
            }
            
            if (sounds_can_be_loaded)
            {
                // Reset the plugin
                this->mMorphexSynth->reset();
                
                // Load the sounds
                for (int i = 0; i < sound_file_paths_to_load.size(); i++)
                {
                    this->mMorphexSynth->instrument.loadSound (sound_file_paths_to_load[i], (MorphLocation) i);
                }
            }
            else
            {
                AlertWindow aux ("Sounds not found", "", AlertWindow::NoIcon);
                aux.showMessageBox (AlertWindow::WarningIcon, "Sounds not found", error_message, "Accept");
            }
        }
    }
    else
    {
        jassertfalse;
        sounds_can_be_loaded = false;
    }
    
    return sounds_can_be_loaded;
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
