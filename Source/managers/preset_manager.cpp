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

#include "preset_manager.h"

#include "../helpers/helper_functions.h"

namespace Constants
{
constexpr auto PresetFileExtension      = ".mpf";
constexpr auto PresetNameParameterId    = "PresetName";
constexpr auto SoundFilePathParameterId = "MorphSoundFilePath";
const auto     PluginDataDirectory      = (File::getSpecialLocation (File::userDesktopDirectory)).getFullPathName() + File::getSeparatorChar() + ProjectInfo::projectName;
const auto     PresetsDirectory         = PluginDataDirectory + File::getSeparatorChar() + "Presets";
const auto     CollectionDirectory      = PluginDataDirectory + File::getSeparatorChar() + "Collections";
} // namespace Constants

namespace morphex
{
PresetManager::PresetManager (AudioProcessor& processor, Core::MorphexSynth& synth) :
    m_processor (processor),
    m_synth (synth),
    m_currentPresetName ("Untitled"),
    m_currentPresetIsSaved (false)
{
    if (!File (Constants::PresetsDirectory).exists())
    {
        File (Constants::PresetsDirectory).createDirectory();
    }

    storeLocalPreset();
}

void PresetManager::getXmlForPreset (XmlElement* xmlElement)
{
    // Get preset name
    xmlElement->setAttribute (Constants::PresetNameParameterId, m_currentPresetName);

    // Get morph sound's file path
    getSoundInformation (xmlElement);

    // Get the plugin parameters
    auto& parameters = m_processor.getParameters();

    for (int i = 0; i < parameters.size(); i++)
    {
        AudioProcessorParameterWithID* parameter = (AudioProcessorParameterWithID*) parameters.getUnchecked (i);

        xmlElement->setAttribute (parameter->paramID, parameter->getValue());
    }
}

bool PresetManager::loadPresetFromXml (XmlElement* xmlElement)
{
    m_currentPresetXml    = xmlElement;
    bool presentWasLoaded = false;

    bool soundsWereLoaded = setSoundInformation (xmlElement);

    if (soundsWereLoaded)
    {
        auto& parameters = m_processor.getParameters();

        for (int i = 0; i < m_currentPresetXml->getNumAttributes(); i++)
        {
            const String paramId = m_currentPresetXml->getAttributeName (i);
            const float  value   = m_currentPresetXml->getDoubleAttribute (paramId);

            for (int j = 0; j < parameters.size(); j++)
            {
                AudioProcessorParameterWithID* parameter =
                    (AudioProcessorParameterWithID*) parameters.getUnchecked (j);

                if (paramId == parameter->paramID)
                {
                    parameter->setValueNotifyingHost (value);
                }
            }
        }

        // Set preset name
        m_currentPresetName = xmlElement->getStringAttribute (Constants::PresetNameParameterId);

        presentWasLoaded = true;
    }

    return presentWasLoaded;
}

int PresetManager::getNumberOfPresets()
{
    return m_localPresets.size();
}

String PresetManager::getPresetName (int inPresetIndex)
{
    return m_localPresets[inPresetIndex].getFileNameWithoutExtension();
}

void PresetManager::createNewPreset()
{
    auto& parameters = m_processor.getParameters();

    for (int i = 0; i < parameters.size(); i++)
    {
        AudioProcessorParameterWithID* parameter = (AudioProcessorParameterWithID*) parameters.getUnchecked (i);

        const float defaultValue = parameter->getDefaultValue();

        parameter->setValueNotifyingHost (defaultValue);
    }

    m_synth.reset();

    m_currentPresetIsSaved = false;
    m_currentPresetName    = "Untitled";
}

void PresetManager::savePreset()
{
    MemoryBlock destinationData;
    m_processor.getStateInformation (destinationData);

    m_currentLoadedPreset.deleteFile();

    m_currentLoadedPreset.appendData (destinationData.getData(),
                                      destinationData.getSize());

    m_currentPresetIsSaved = true;
}

void PresetManager::saveAsPreset (String inPresetName)
{
    auto presetFilePath = Constants::PresetsDirectory + File::getSeparatorChar() + inPresetName + Constants::PresetFileExtension;
    File presetFile     = File (presetFilePath);

    if (!presetFile.exists())
    {
        presetFile.create();
    }
    else
    {
        presetFile.deleteFile();
    }

    MemoryBlock destinationData;
    m_processor.getStateInformation (destinationData);

    presetFile.appendData (destinationData.getData(),
                           destinationData.getSize());

    m_currentPresetIsSaved = true;
    m_currentPresetName    = inPresetName;

    storeLocalPreset();
}

bool PresetManager::loadPreset (int inPresetIndex = 0)
{
    bool presentWasLoaded = false;

    m_currentLoadedPreset = m_localPresets[inPresetIndex];

    MemoryBlock presetBinary;

    if (m_currentLoadedPreset.loadFileAsData (presetBinary))
    {
        std::unique_ptr<XmlElement> xmlState = juce::AudioPluginInstance::getXmlFromBinary (presetBinary.getData(), (int) presetBinary.getSize());

        if (xmlState != nullptr)
        {
            forEachXmlChildElement (*xmlState, subChild)
            {
                // Load the preset parameters
                presentWasLoaded = loadPresetFromXml (subChild);

                if (presentWasLoaded)
                {
                    m_currentPresetIsSaved = true;
                    m_currentPresetName    = getPresetName (inPresetIndex);
                }
            }
        }
        else
        {
            jassertfalse;
        }
    }

    return presentWasLoaded;
}

bool PresetManager::getIsCurrentPresetSaved() const
{
    return m_currentPresetIsSaved;
}

String PresetManager::getCurrentPresetName() const
{
    return m_currentPresetName;
}

// TODO - Save only the full path of loaded sounds on instrument->notes-velocity->sounds, not on instrument->morphSounds
// afterwards, save which note and velocity are referenced to each morphSounds, follow the same structure in the xml,
// maybe swith to a ValueTree to make it more straightforward
void PresetManager::getSoundInformation (XmlElement* xmlElement) const
{
    MorphSounds morphSounds = m_synth.m_instrument.getMorphSounds();

    for (int i = 0; i < morphSounds.size(); i++)
    {
        std::string soundFilePath = morphSounds[i]->path;
        removeSubStr (soundFilePath, (Constants::CollectionDirectory + File::getSeparatorChar()).toStdString());
        String soundFilePathId = Constants::SoundFilePathParameterId + String (i + 1);
        xmlElement->setAttribute (soundFilePathId, soundFilePath);
    }
}

// TODO - Save only the full path of loaded sounds on instrument->notes-velocity->sounds, not on instrument->morphSounds
// afterwards, save which note and velocity are referenced to each morphSounds, follow the same structure in the xml,
// maybe swith to a ValueTree to make it more straightforward
bool PresetManager::setSoundInformation (XmlElement* presetBody)
{
    bool soundsWereLoaded = true;

    // Get morph sound's file path
    MorphSounds              morphSounds = m_synth.m_instrument.getMorphSounds();
    std::vector<std::string> soundFilePathsToLoad;

    // Error vars
    String errorMessage = "The preset couldn't be loaded, the following sounds are not in your library:\n";

    // Checking of the sounds exist
    for (int i = 0; i < morphSounds.size(); i++)
    {
        // Generate sound ID
        String soundFilePathId = Constants::SoundFilePathParameterId + String (i + 1);

        // Retrieve the sound file path
        std::string soundFilePath = presetBody->getStringAttribute (soundFilePathId).toStdString();

        // If it's not empty
        if (!soundFilePath.empty())
        {
            // Prepend the user's plugin data directory
            soundFilePath = (Constants::CollectionDirectory + File::getSeparatorChar()).toStdString() + soundFilePath;

            // Check if sound file path is valid
            File soundFilePath_check (soundFilePath);

            if (!soundFilePath_check.existsAsFile())
            {
                soundsWereLoaded = false;
                errorMessage += String ("\n" + soundFilePath);
            }
            else
            {
                soundFilePathsToLoad.push_back (soundFilePath);
            }
        }
    }

    if (soundsWereLoaded)
    {
        // Reset the plugin
        m_synth.reset();

        // Load the sounds
        for (int i = 0; i < soundFilePathsToLoad.size(); i++)
        {
            m_synth.m_instrument.loadSound (soundFilePathsToLoad[i], (MorphLocation) i);
        }
    }
    else
    {
        DBG ("Sounds not found");
        //        AlertWindow aux ("Sounds not found", "", AlertWindow::NoIcon);
        //        aux.showMessageBox (AlertWindow::WarningIcon, "Sounds not found", errorMessage, "Accept");
    }

    return soundsWereLoaded;
}

void PresetManager::storeLocalPreset()
{
    m_localPresets.clear();

    for (DirectoryIterator di (File (Constants::PresetsDirectory),
                               false,
                               "*" + static_cast<std::string> (Constants::PresetFileExtension),
                               File::TypesOfFileToFind::findFiles);
         di.next();)
    {
        File preset = di.getFile();
        m_localPresets.add (preset);
    }
}
} // namespace morphex
