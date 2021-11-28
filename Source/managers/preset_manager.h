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

#include "../core/synth.h"

namespace morphex
{
class PresetManager
{
public:
    PresetManager (juce::AudioProcessor& processor, Synth& synth);

    void getXmlForPreset (juce::XmlElement* xmlElement);
    bool loadPresetFromXml (juce::XmlElement* xmlElement);

    int    getNumberOfPresets();
    juce::String getPresetName (int presetIndex);

    void   createNewPreset();
    void   savePreset();
    void   saveAsPreset (juce::String presetName);
    bool   loadPreset (int presetIndex);
    bool   getIsCurrentPresetSaved() const;
    juce::String getCurrentPresetName() const;

    void getSoundInformation (juce::XmlElement* xmlElement) const;
    bool setSoundInformation (juce::XmlElement* presetBody);

private:
    void storeLocalPreset();

    juce::AudioProcessor&     m_processor;
    Synth& m_synth;

    juce::Array<juce::File> m_localPresets;
    juce::String      m_currentPresetName;
    bool        m_currentPresetIsSaved;
    juce::XmlElement* m_currentPresetXml;
    juce::File        m_currentLoadedPreset;
};
}; // namespace morphex
