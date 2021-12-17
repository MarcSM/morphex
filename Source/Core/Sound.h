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

#include <JuceHeader.h>

#include "frame.h"

// #include <numeric>
// #include <vector>

// TODO - Remove this and use std::map
// #define ADD_MAPPAIR(_first, _second)                                    \
//     struct Pair                                                         \
//     {                                                                   \
//         std::pair<const key_type, mapped_type>& _p;                     \
//         Pair (std::pair<const key_type, mapped_type>& _a) : _p (_a) {}; \
//         key_type     _first() { return _p.first; };                     \
//         mapped_type& _second() { return _p.second; };                   \
//     }

// #define ADD_ITER(_first, _second)                           \
//     struct Iterator : public iterator                       \
//     {                                                       \
//         Iterator() {};                                      \
//         Iterator (const iterator& _a) : iterator (_a) {};   \
//         key_type     _first() { return (*this)->first; };   \
//         mapped_type& _second() { return (*this)->second; }; \
//     }

namespace morphex
{
class Sound
{
public:
    enum WindowType
    {
        blackmanharris = 0,
        triang,
        hanning
    };
    
    struct SourceInfo
    {
        std::string name;
        std::string extension;
        std::string path;
        std::string dirpath;
    };

    struct HadFileInfo
    {
        std::string name;
        std::string extension;
        std::string path;
        std::string dirpath;
    };

    struct Info
    {
        int note;
        int velocity;
        int loopStart;
        int loopEnd;
        int length;
    };

    struct HadInfo
    {
        int        fileVersion         = 0;
        int        decimalPlaces       = 0;
        WindowType window_type         = WindowType::blackmanharris;
        int        window_size         = 0;
        int        fft_size            = 0;
        int        magnitude_threshold = 0;
        int        hearing_threshold   = 0;
        double     min_sine_dur        = 0.0;
        int        max_harm            = 0;
        int        min_f0              = 0;
        int        max_f0              = 0;
        int        max_f0_error        = 0;
        double     harm_dev_slope      = 0.0;
        double     stoc_fact           = 0.0;
        int        synthesis_fft_size  = 0;
        int        hop_size            = 0;
        int        fs                  = 0; // Frame size (sample rate)
    };

    struct SoundFeatures
    {
        // struct NotesMap : public std::map<float, int, std::less<float>>
        // {
        //     ADD_ITER (iterFrequency, iterOccurences);
        //     ADD_MAPPAIR (frequency, occurences);
        // };
        // NotesMap notes;
        std::map<float, int> notes;
        float                predominantNote;
        // float                normalizationFactor = 1.0;
        // int                  maxHarmonics        = 0;
        // int                  maxFrames           = 0;
    };

    Sound (std::string filePath);

    void reset();
    void loadHadFile (juce::String filePath);
    bool isLoaded();
    bool isAnalyzed();

    Info getInfo();
    int getMaxFrames();
    std::unique_ptr<Frame> getFrame (int frameIndex, int hopSize) const;

private:
    void synthesize();
    void getFundamentalNotes();
    // void normalizeMagnitudes();
    // void normalizeWaveform (std::vector<float>& waveform, float max_val, float max_db);

    bool                                hasChild (juce::XmlElement* parent, juce::String child_name);
    std::vector<long long>              splitInts (const std::string& list_of_ints);
    std::vector<float>                  splitFloats (const std::string& list_of_floats, bool int_to_float = false);
    std::vector<float>                  getVectorOfFloats (juce::XmlElement* parent, juce::String child_name);
    std::vector<std::vector<long long>> getMatrixOfInts (juce::XmlElement* parent, juce::String child_name);
    std::vector<std::vector<float>>     getMatrixOfFloats (juce::XmlElement* parent, juce::String child_name);

    // float findClosest (float arr[], int n, float target);
    // float getClosestValue (float val1, float val2, float target);

    std::vector<Frame> m_frames;
    std::vector<float> m_waveform;

    std::unique_ptr<SourceInfo> m_sourceInfo;
    std::unique_ptr<HadFileInfo>     m_hadFileInfo;
    std::unique_ptr<Info>       m_info;
    std::unique_ptr<HadInfo>         m_hadInfo;
    std::unique_ptr<SoundFeatures>   m_soundFeatures;
};
}; // namespace morphex
