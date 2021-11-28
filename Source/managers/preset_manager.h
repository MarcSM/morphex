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
    PresetManager (AudioProcessor& processor, Core::MorphexSynth& synth);

    void getXmlForPreset (XmlElement* xmlElement);
    bool loadPresetFromXml (XmlElement* xmlElement);

    int    getNumberOfPresets();
    String getPresetName (int presetIndex);

    void   createNewPreset();
    void   savePreset();
    void   saveAsPreset (String presetName);
    bool   loadPreset (int presetIndex);
    bool   getIsCurrentPresetSaved() const;
    String getCurrentPresetName() const;

    void getSoundInformation (XmlElement* xmlElement) const;
    bool setSoundInformation (XmlElement* presetBody);

private:
    void storeLocalPreset();

    AudioProcessor&     m_processor;
    Core::MorphexSynth& m_synth;

    Array<File> m_localPresets;
    String      m_currentPresetName;
    bool        m_currentPresetIsSaved;
    XmlElement* m_currentPresetXml;
    File        m_currentLoadedPreset;
};
}; // namespace morphex
