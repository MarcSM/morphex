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

void PresetManager::getXmlForPreset (XmlElement* inElement)
{
    // Get preset name
    inElement->setAttribute (PRESET_NAME_PARAMETER_ID, mCurrentPresetName);
    
    // Get morph sound's file path
    this->getSoundInformation (inElement);
    
    // Get the plugin parameters
    auto& parameters = mProcessor->getParameters();
    
    for (int i=0; i < parameters.size(); i++)
    {
        AudioProcessorParameterWithID* parameter =
        (AudioProcessorParameterWithID*) parameters.getUnchecked (i);
        
        inElement->setAttribute (parameter->paramID,
                                 parameter->getValue());
    }
}

bool PresetManager::loadPresetFromXml (XmlElement* inElement)
{
    mCurrentPresetXml = inElement;
    bool present_was_loaded = false;
    
    bool sounds_were_loaded = this->setSoundInformation (inElement);
    
    if (sounds_were_loaded)
    {
        auto& parameters = mProcessor->getParameters();
        
        for (int i=0; i < mCurrentPresetXml->getNumAttributes(); i++)
        {
            const String paramId = mCurrentPresetXml->getAttributeName (i);
            const float value = mCurrentPresetXml->getDoubleAttribute (paramId);
            
            for (int j=0; j < parameters.size(); j++)
            {
                AudioProcessorParameterWithID* parameter =
                (AudioProcessorParameterWithID*) parameters.getUnchecked(j);
                
                if (paramId == parameter->paramID)
                {
                    parameter->setValueNotifyingHost(value);
                }
            }
        }
        
        // Set preset name
        mCurrentPresetName = inElement->getStringAttribute (PRESET_NAME_PARAMETER_ID);
        
        present_was_loaded = true;
    }
    
    return present_was_loaded;
}

int PresetManager::getNumberOfPresets()
{
    return mLocalPresets.size();
}

String PresetManager::getPresetName (int inPresetIndex)
{
    return mLocalPresets[inPresetIndex].getFileNameWithoutExtension();
}

void PresetManager::createNewPreset()
{
    auto& parameters = mProcessor->getParameters();
    
    for (int i=0; i < parameters.size(); i++)
    {
        AudioProcessorParameterWithID* parameter =
        (AudioProcessorParameterWithID*) parameters.getUnchecked(i);
        
        const float defaultValue =
        parameter->getDefaultValue();
        
        parameter->setValueNotifyingHost (defaultValue);
    }
    
    mMorphexSynth->reset();
    
    mCurrentPresetIsSaved = false;
    mCurrentPresetName = "Untitled";
}

void PresetManager::savePreset()
{
    MemoryBlock destinationData;
    mProcessor->getStateInformation(destinationData);
    
    mCurrentlyLoadedPreset.deleteFile();
    
    mCurrentlyLoadedPreset.appendData (destinationData.getData(),
                                       destinationData.getSize());
    
    mCurrentPresetIsSaved = true;
}

void PresetManager::saveAsPreset (String inPresetName)
{
    File presetFile = File (PLUGIN_DATA_PRESETS_DIRECTORY + directorySeparator + inPresetName + PRESET_FILE_EXTENSION);
    
    if (!presetFile.exists())
    {
        presetFile.create();
    }
    else
    {
        presetFile.deleteFile();
    }
    
    MemoryBlock destinationData;
    mProcessor->getStateInformation (destinationData);
    
    presetFile.appendData (destinationData.getData(),
                           destinationData.getSize());
    
    mCurrentPresetIsSaved = true;
    mCurrentPresetName = inPresetName;
    
    storeLocalPreset();
}

bool PresetManager::loadPreset (int inPresetIndex = 0)
{
    bool present_was_loaded = false;
    
    mCurrentlyLoadedPreset = mLocalPresets[inPresetIndex];
    
    MemoryBlock presetBinary;
    
    if (mCurrentlyLoadedPreset.loadFileAsData (presetBinary))
    {
        std::unique_ptr<XmlElement> xmlState = juce::AudioPluginInstance::getXmlFromBinary (presetBinary.getData(), (int) presetBinary.getSize());
        
        if (xmlState != nullptr)
        {
            forEachXmlChildElement (*xmlState, subChild)
            {
                // Load the preset parameters
                present_was_loaded = this->loadPresetFromXml (subChild);
                
                if (present_was_loaded)
                {
                    mCurrentPresetIsSaved = true;
                    mCurrentPresetName = getPresetName (inPresetIndex);
                }
            }
        }
        else
        {
            jassertfalse;
        }
    }
    
    return present_was_loaded;
}

// TODO - Save only the full path of loaded sounds on instrument->notes-velocity->sounds, not on instrument->morph_sounds
// afterwards, save which note and velocity are referenced to each morph_sounds, follow the same structure in the xml,
// maybe swith to a ValueTree to make it more straightforward
void PresetManager::getSoundInformation (XmlElement* inElement)
{
    MorphSounds morph_sounds = this->mMorphexSynth->instrument.getMorphSounds();
    
    for (int i = 0; i < morph_sounds.size(); i++)
    {
        std::string sound_file_path = morph_sounds[i]->path;
        removeSubStr (sound_file_path, (PLUGIN_DATA_COLLECTIONS_DIRECTORY + directorySeparator).toStdString());
        String sound_file_path_id = SOUND_FILE_PATH_PARAMETER_ID + String (i + 1);
        inElement->setAttribute (sound_file_path_id, sound_file_path);
    }
}

// TODO - Save only the full path of loaded sounds on instrument->notes-velocity->sounds, not on instrument->morph_sounds
// afterwards, save which note and velocity are referenced to each morph_sounds, follow the same structure in the xml,
// maybe swith to a ValueTree to make it more straightforward
bool PresetManager::setSoundInformation (XmlElement* presetBody)
{
    bool sounds_were_loaded = true;
    
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
        std::string sound_file_path = presetBody->getStringAttribute (sound_file_path_id).toStdString();
        
        // If it's not empty
        if (!sound_file_path.empty())
        {
            // Prepend the user's plugin data directory
            sound_file_path = (PLUGIN_DATA_COLLECTIONS_DIRECTORY + directorySeparator).toStdString() + sound_file_path;
            
            // Check if sound file path is valid
            File sound_file_path_check (sound_file_path);
            
            if (!sound_file_path_check.existsAsFile())
            {
                sounds_were_loaded = false;
                error_message += String ("\n" + sound_file_path);
            }
            else
            {
                sound_file_paths_to_load.push_back (sound_file_path);
            }
        }
    }
    
    if (sounds_were_loaded)
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
    
    return sounds_were_loaded;
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
    
    for (DirectoryIterator di (File (PLUGIN_DATA_PRESETS_DIRECTORY),
                               false,
                               "*" + (String) PRESET_FILE_EXTENSION,
                               File::TypesOfFileToFind::findFiles); di.next();)
    {
        File preset = di.getFile();
        mLocalPresets.add (preset);
    }
}
