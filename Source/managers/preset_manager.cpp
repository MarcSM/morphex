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

namespace Constants
{
constexpr auto PresetFileExtension      = ".mpf";
constexpr auto PresetNameParameterId    = "PresetName";
constexpr auto SoundFilePathParameterId = "MorphSoundFilePath";
const auto     PluginDataDirectory      = (juce::File::getSpecialLocation (juce::File::userDesktopDirectory)).getFullPathName() + juce::File::getSeparatorChar() + ProjectInfo::projectName;
const auto     PresetsDirectory         = PluginDataDirectory + juce::File::getSeparatorChar() + "Presets";
const auto     CollectionDirectory      = PluginDataDirectory + juce::File::getSeparatorChar() + "Collections";
} // namespace Constants

namespace morphex
{
PresetManager::PresetManager (juce::AudioProcessor& processor, Synth& synth) :
    m_processor (processor),
    m_synth (synth),
    m_currentPresetName ("Untitled"),
    m_currentPresetIsSaved (false)
{
    if (!juce::File (Constants::PresetsDirectory).exists())
    {
        juce::File (Constants::PresetsDirectory).createDirectory();
    }

    storeLocalPreset();
}

void PresetManager::getXmlForPreset (juce::XmlElement* xmlElement)
{
    // Get preset name
    xmlElement->setAttribute (Constants::PresetNameParameterId, m_currentPresetName);

    // Get morph sound's file path
    getSoundInformation (xmlElement);

    // Get the plugin parameters
    auto& parameters = m_processor.getParameters();

    for (int i = 0; i < parameters.size(); i++)
    {
        juce::AudioProcessorParameterWithID* parameter = (juce::AudioProcessorParameterWithID*) parameters.getUnchecked (i);

        xmlElement->setAttribute (parameter->paramID, parameter->getValue());
    }
}

bool PresetManager::loadPresetFromXml (juce::XmlElement* xmlElement)
{
    m_currentPresetXml    = xmlElement;
    bool presentWasLoaded = false;

    bool soundsWereLoaded = setSoundInformation (xmlElement);

    if (soundsWereLoaded)
    {
        auto& parameters = m_processor.getParameters();

        for (int i = 0; i < m_currentPresetXml->getNumAttributes(); i++)
        {
            const juce::String paramId = m_currentPresetXml->getAttributeName (i);
            const float        value   = m_currentPresetXml->getDoubleAttribute (paramId);

            for (int j = 0; j < parameters.size(); j++)
            {
                juce::AudioProcessorParameterWithID* parameter =
                    (juce::AudioProcessorParameterWithID*) parameters.getUnchecked (j);

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

juce::String PresetManager::getPresetName (int inPresetIndex)
{
    return m_localPresets[inPresetIndex].getFileNameWithoutExtension();
}

void PresetManager::createNewPreset()
{
    auto& parameters = m_processor.getParameters();

    for (int i = 0; i < parameters.size(); i++)
    {
        juce::AudioProcessorParameterWithID* parameter = (juce::AudioProcessorParameterWithID*) parameters.getUnchecked (i);

        const float defaultValue = parameter->getDefaultValue();

        parameter->setValueNotifyingHost (defaultValue);
    }

    m_synth.reset();

    m_currentPresetIsSaved = false;
    m_currentPresetName    = "Untitled";
}

void PresetManager::savePreset()
{
    juce::MemoryBlock destinationData;
    m_processor.getStateInformation (destinationData);

    m_currentLoadedPreset.deleteFile();

    m_currentLoadedPreset.appendData (destinationData.getData(),
                                      destinationData.getSize());

    m_currentPresetIsSaved = true;
}

void PresetManager::saveAsPreset (juce::String inPresetName)
{
    auto       presetFilePath = Constants::PresetsDirectory + juce::File::getSeparatorChar() + inPresetName + Constants::PresetFileExtension;
    juce::File presetFile     = juce::File (presetFilePath);

    if (!presetFile.exists())
    {
        presetFile.create();
    }
    else
    {
        presetFile.deleteFile();
    }

    juce::MemoryBlock destinationData;
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

    juce::MemoryBlock presetBinary;

    if (m_currentLoadedPreset.loadFileAsData (presetBinary))
    {
        std::unique_ptr<juce::XmlElement> xmlState = juce::AudioPluginInstance::getXmlFromBinary (presetBinary.getData(), (int) presetBinary.getSize());

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

juce::String PresetManager::getCurrentPresetName() const
{
    return m_currentPresetName;
}

// TODO - Save only the full path of loaded sounds on instrument->notes-velocity->sounds, not on instrument->morphSounds
// afterwards, save which note and velocity are referenced to each morphSounds, follow the same structure in the xml,
// maybe swith to a ValueTree to make it more straightforward
void PresetManager::getSoundInformation (juce::XmlElement* xmlElement) const
{
    MorphSounds morphSounds = m_synth.m_instrument.getMorphSounds();

    for (int i = 0; i < morphSounds.size(); i++)
    {
        std::string soundFilePath = morphSounds[i]->getHadFileInfo()->path;
        removeSubStr (soundFilePath, (Constants::CollectionDirectory + juce::File::getSeparatorChar()).toStdString());
        juce::String soundFilePathId = Constants::SoundFilePathParameterId + juce::String (i + 1);
        xmlElement->setAttribute (soundFilePathId, soundFilePath);
    }
}

// TODO - Save only the full path of loaded sounds on instrument->notes-velocity->sounds, not on instrument->morphSounds
// afterwards, save which note and velocity are referenced to each morphSounds, follow the same structure in the xml,
// maybe swith to a ValueTree to make it more straightforward
bool PresetManager::setSoundInformation (juce::XmlElement* presetBody)
{
    bool soundsWereLoaded = true;

    // Get morph sound's file path
    MorphSounds              morphSounds = m_synth.m_instrument.getMorphSounds();
    std::vector<std::string> soundFilePathsToLoad;

    // Error vars
    juce::String errorMessage = "The preset couldn't be loaded, the following sounds are not in your library:\n";

    // Checking of the sounds exist
    for (int i = 0; i < morphSounds.size(); i++)
    {
        // Generate sound ID
        juce::String soundFilePathId = Constants::SoundFilePathParameterId + juce::String (i + 1);

        // Retrieve the sound file path
        std::string soundFilePath = presetBody->getStringAttribute (soundFilePathId).toStdString();

        // If it's not empty
        if (!soundFilePath.empty())
        {
            // Prepend the user's plugin data directory
            soundFilePath = (Constants::CollectionDirectory + juce::File::getSeparatorChar()).toStdString() + soundFilePath;

            // Check if sound file path is valid
            juce::File soundFilePath_check (soundFilePath);

            if (!soundFilePath_check.existsAsFile())
            {
                soundsWereLoaded = false;
                errorMessage += juce::String ("\n" + soundFilePath);
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

    for (juce::DirectoryIterator di (juce::File (Constants::PresetsDirectory),
                                     false,
                                     "*" + static_cast<std::string> (Constants::PresetFileExtension),
                                     juce::File::TypesOfFileToFind::findFiles);
         di.next();)
    {
        juce::File preset = di.getFile();
        m_localPresets.add (preset);
    }
}

// Remove First Occurrence of given substring from main string.
void PresetManager::removeSubStr (std::string& mainStr, const std::string& toRemove) const
{
    // Search for the substring in string
    size_t pos = mainStr.find (toRemove);

    if (pos != std::string::npos)
    {
        // If found then remove it from string
        mainStr.erase (pos, toRemove.length());
    }
}
} // namespace morphex
