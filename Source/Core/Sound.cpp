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

#include "sound.h"
#include "tools.h"
#include "codec.h"

// TODO - Adapt/remake this class to make use of JUCE's "ValueTree" class (undo/redo feature)
// TODO - Merge with synth_sound

namespace Constants
{
constexpr auto CurrentFileVersion = 1;
constexpr auto A4Hz               = 440.0;
} // namespace Constants

namespace morphex
{
Sound::Sound (std::string filePath)
{
    loadHadFile (filePath);
};

void Sound::reset()
{
    m_frames.clear();
    m_waveform.clear();

    m_sourceInfo = nullptr;
    m_hadFileInfo     = nullptr;
    m_info       = nullptr;
    m_hadInfo         = nullptr;
    m_soundFeatures   = nullptr;
}

void Sound::loadHadFile (juce::String filePath)
{
    reset();

    try
    {
        juce::File hadFile = juce::File (filePath);

        if (hadFile.existsAsFile())
        {
            m_hadFileInfo            = std::make_unique<HadFileInfo>();
            m_hadFileInfo->name      = hadFile.getFileNameWithoutExtension().toStdString();
            m_hadFileInfo->extension = hadFile.getFileExtension().toStdString();
            m_hadFileInfo->path      = filePath.toStdString();
            m_hadFileInfo->dirpath   = hadFile.getParentDirectory().getFullPathName().toStdString();

            std::unique_ptr<juce::XmlElement> xml (juce::XmlDocument::parse (hadFile.loadFileAsString()));

            if (xml)
            {
                juce::XmlElement* xml_file = xml->getChildByName ("file"); // had["file"]

                if (xml_file)
                {
                    const auto file_version = hasChild (xml_file, "v") ? xml_file->getChildByName ("v")->getAllSubText().getIntValue() : 0;

                    if (file_version == Constants::CurrentFileVersion and hasChild (xml_file, "dp"))
                    {
                        // Sound
                        juce::XmlElement* xmlSound = xml->getChildByName ("sound"); // had["sound"]
                        m_info                = std::make_unique<Info>();
                        m_hadInfo                  = std::make_unique<HadInfo>();
                        m_info->note          = xmlSound->getChildByName ("note")->getAllSubText().getIntValue();
                        m_info->velocity      = xmlSound->getChildByName ("velocity")->getAllSubText().getIntValue();
                        m_hadInfo->fs              = xmlSound->getChildByName ("fs")->getAllSubText().getIntValue();
                        m_info->loopStart     = xmlSound->getChildByName ("loop")->getChildByName ("start")->getAllSubText().getIntValue();
                        m_info->loopEnd       = xmlSound->getChildByName ("loop")->getChildByName ("end")->getAllSubText().getIntValue();

                        // Analysis parameters
                        juce::XmlElement* xmlParameters = xml->getChildByName ("parameters"); // had["parameters"]
                        m_hadInfo                       = std::make_unique<HadInfo>();
                        m_hadInfo->window_type          = static_cast<WindowType>(xmlParameters->getChildByName ("window_type")->getAllSubText().getIntValue());
                        m_hadInfo->window_size          = xmlParameters->getChildByName ("window_size")->getAllSubText().getIntValue();
                        m_hadInfo->fft_size             = xmlParameters->getChildByName ("fft_size")->getAllSubText().getIntValue();
                        m_hadInfo->magnitude_threshold  = xmlParameters->getChildByName ("magnitude_threshold")->getAllSubText().getIntValue();
                        m_hadInfo->hearing_threshold    = xmlParameters->getChildByName ("hearing_threshold")->getAllSubText().getIntValue();
                        m_hadInfo->min_sine_dur         = xmlParameters->getChildByName ("min_sine_dur")->getAllSubText().getDoubleValue();
                        m_hadInfo->max_harm             = xmlParameters->getChildByName ("max_harm")->getAllSubText().getIntValue();
                        m_hadInfo->min_f0               = xmlParameters->getChildByName ("min_f0")->getAllSubText().getIntValue();
                        m_hadInfo->max_f0               = xmlParameters->getChildByName ("max_f0")->getAllSubText().getIntValue();
                        m_hadInfo->max_f0_error         = xmlParameters->getChildByName ("max_f0_error")->getAllSubText().getIntValue();
                        m_hadInfo->harm_dev_slope       = xmlParameters->getChildByName ("harm_dev_slope")->getAllSubText().getDoubleValue();
                        m_hadInfo->stoc_fact            = xmlParameters->getChildByName ("stoc_fact")->getAllSubText().getDoubleValue();
                        m_hadInfo->synthesis_fft_size   = xmlParameters->getChildByName ("synthesis_fft_size")->getAllSubText().getIntValue();
                        m_hadInfo->hop_size             = xmlParameters->getChildByName ("hop_size")->getAllSubText().getIntValue();

                        // Synthesis
                        juce::XmlElement* xml_synthesis = xml->getChildByName ("synthesis"); // had["synthesis"]

                        // Harmonic
                        if (hasChild (xml_synthesis, "h"))
                        {
                            juce::XmlElement* xml_synthesis_harmonic = xml_synthesis->getChildByName ("h"); // had["synthesis"]["h"]
                            
                            auto harmonicFrequencies = getMatrixOfFloats (xml_synthesis_harmonic, "f");
                            auto harmonicMagnitudes  = getMatrixOfFloats (xml_synthesis_harmonic, "m");
                            auto harmonicPhases      = getMatrixOfFloats (xml_synthesis_harmonic, "p");

                            codec::decodeMatrixDiff (harmonicFrequencies);
                            codec::decodeMatrixDiff (harmonicMagnitudes);
                            codec::decodeMatrixDiff (harmonicPhases);
                            
                            codec::decodeMatrix (harmonicFrequencies, false, false);
                            codec::decodeMatrix (harmonicMagnitudes, true, false);
                            codec::decodeMatrix (harmonicPhases, false, false);

                            if (harmonicFrequencies.size() != harmonicMagnitudes.size())
                            {
                                throw "Harmonic information sizes does not match";
                            }
                            
                            if (harmonicPhases.size() > 0 && harmonicFrequencies.size() != harmonicPhases.size())
                            {
                                throw "Harmonic phase information size does not match";
                            }

                            const auto maxFramesSize = std::max (m_frames.size(), harmonicFrequencies.size());

                            m_frames.resize (maxFramesSize);

                            for (auto i = 0u; i < harmonicFrequencies.size(); ++i)
                            {
                                // TODO - All had files must have the phase info
                                if (harmonicPhases.size() < i)
                                {
                                    m_frames[i]->setHarmonicComponent ({ harmonicFrequencies[i], harmonicMagnitudes[i], harmonicPhases[i] });
                                }
                                else
                                {
                                    std::vector<float> freqs = harmonicFrequencies[i];
                                    std::vector<float> mags = harmonicMagnitudes[i];
                                    std::vector<float> phases;
//                                    Frame::FftComponent frame = { freqs,  mags, phases };
                                    
                                    m_frames[i]->setHarmonicComponent (freqs,  mags, phases);
//                                    m_frames[i]->setHarmonicComponent ({ freqs,  mags, phases });
//                                    m_frames[i]->setHarmonicComponent ({ harmonicFrequencies[i], harmonicMagnitudes[i], harmonicMagnitudes[i] });
                                }
                            }
                        }

                        // Sinusoidal
                        if (hasChild (xml_synthesis, "s"))
                        {
                            juce::XmlElement* xml_synthesis_sinusoidal = xml_synthesis->getChildByName ("s"); // had["synthesis"]["s"]
                            
                            auto sinusoidalFrequencies = getMatrixOfFloats (xml_synthesis_sinusoidal, "f");
                            auto sinusoidalMagnitudes = getMatrixOfFloats (xml_synthesis_sinusoidal, "m");
                            auto sinusoidalPhases = getMatrixOfFloats (xml_synthesis_sinusoidal, "p");

                            codec::decodeMatrixDiff (sinusoidalFrequencies);
                            codec::decodeMatrixDiff (sinusoidalMagnitudes);
                            codec::decodeMatrixDiff (sinusoidalPhases);
                            
                            codec::decodeMatrix (sinusoidalFrequencies, false, false);
                            codec::decodeMatrix (sinusoidalMagnitudes, true, false);
                            codec::decodeMatrix (sinusoidalPhases, false, false);

                            if (sinusoidalFrequencies.size() != sinusoidalMagnitudes.size() ||
                                sinusoidalFrequencies.size() != sinusoidalPhases.size())
                            {
                                throw "Sinusoidal information sizes do not match";
                            }

                            const auto maxFramesSize = std::max (m_frames.size(), sinusoidalFrequencies.size());

                            m_frames.resize (maxFramesSize);

                            for (auto i = 0u; i < sinusoidalFrequencies.size(); ++i)
                            {
                                m_frames[i]->setSinusoidalComponent ({ sinusoidalFrequencies[i], sinusoidalMagnitudes[i], sinusoidalPhases[i] });
                            }
                        }

                        // Stochastic
                        if (hasChild (xml_synthesis, "c"))
                        {
                            auto stochasticMagnitudes = getMatrixOfFloats (xml_synthesis, "c");
                            
                            codec::decodeMatrix (stochasticMagnitudes, true);

                            const auto maxFramesSize = std::max (m_frames.size(), stochasticMagnitudes.size());

                            m_frames.resize (maxFramesSize);

                            for (auto i = 0u; i < stochasticMagnitudes.size(); ++i)
                            {
                                m_frames[i]->setStochasticMagnitudes (stochasticMagnitudes[i]);
                            }
                        }

                        const auto hopSize = m_hadInfo->hop_size;

                        if (hopSize <= 0u)
                        {
                            throw "hop_size value is not valid";
                        }

                        // Attack
                        if (hasChild (xml_synthesis, "a"))
                        {
                            auto attackWaveform = getVectorOfFloats (xml_synthesis, "a");
                            
                            codec::decodeVector (attackWaveform);
                            
                            const auto attackNumOfFrames = static_cast<size_t>( std::ceil ( static_cast<double>(attackWaveform.size() / hopSize)));
                            const auto maxFramesSize     = std::max (m_frames.size(), attackNumOfFrames);

                            m_frames.resize (maxFramesSize);

                            auto currentSample = 0u;

                            for (auto i = 0u; i < attackNumOfFrames; ++i)
                            {
                                const auto waveformSize = std::min (attackWaveform.size() - currentSample, hopSize);

                                m_frames[i]->setAttackWaveform ({ attackWaveform.begin() + currentSample, attackWaveform.begin() + currentSample + waveformSize });

                                currentSample += hopSize;
                            }
                        }

                        // Residual
                        if (hasChild (xml_synthesis, "r"))
                        {
                            auto residualWaveform = getVectorOfFloats (xml_synthesis, "r");
                            
                            codec::decodeVector (residualWaveform);
                            
                            const auto residualNumOfFrames = static_cast<size_t>( std::ceil ( static_cast<double>(residualWaveform.size() / hopSize)));
                            const auto maxFramesSize       = std::max (m_frames.size(), residualNumOfFrames);

                            m_frames.resize (maxFramesSize);

                            auto currentSample = 0u;

                            for (auto i = 0u; i < residualNumOfFrames; ++i)
                            {
                                const auto waveformSize = std::min (residualWaveform.size() - currentSample, hopSize);

                                m_frames[i]->setResidualWaveform ({ residualWaveform.begin() + currentSample, residualWaveform.begin() + currentSample + waveformSize });

                                currentSample += hopSize;
                            }
                        }

                        
                        m_soundFeatures = std::make_unique<SoundFeatures>();
                        
                        getFundamentalNotes();

                        /** Normalize magnitudes on load by default */
                        // TODO - Fix this method
                        //                normalizeMagnitudes();
                    }
                    else
                    {
                        throw "Error while parsing the XML hda file data\n";
                        jassertfalse;
                    }
                }
                else
                {
                    throw "Sound file not compatible";
                }
            }
            else
            {
                throw "Error while loading the sound\n";
                jassertfalse;
            }
        }
        else
        {
            throw "The file doesn't exist\n";
            jassertfalse;
        }
    }
    catch (const char* msg)
    {
        DBG("Error while loading the sound: '" << msg << "'\n");
        jassertfalse;
        reset();
    }
};

bool Sound::isLoaded() const
{
    return m_info && isAnalyzed();
}

bool Sound::isAnalyzed() const
{
    return m_hadInfo != nullptr;
}

//    // TODO - If there is no ".mif" (Morphex Instrument File)
//    // It is necessary to preload or even pre-analyze the sound file
//    // to locate it on the right note. Try to use copy constructors
//    // to prevent process the whole file again.
//    void Sound::getInstrumentFromFile(std::string filePath)
//    {
//        File had_file = File(filePath);
//        fileData = had_file.loadFileAsString();
//        return
//
//        XmlDocument    (    const File &     file    )
//
//
//        std::unique_ptr<XmlElement> xml( XmlDocument::parse( fileData ) );
//
//        std::string filePath
//    }

Sound::Info* Sound::getInfo() const
{
    return m_info.get();
}

Sound::HadFileInfo* Sound::getHadFileInfo() const
{
    return m_hadFileInfo.get();
}

int Sound::getMaxFrames() const
{
    return static_cast<int>(m_frames.size());
}

std::shared_ptr<const Frame> Sound::getFrame (int frameIndex, int hopSize) const
{
    auto frame = nullptr;

    if (frameIndex < m_frames.size())
    {
        return m_frames[frameIndex];
    }
    else
    {
        jassertfalse;
    }

    return frame;
}

void Sound::setVelocity(const int velocity)
{
    if (m_info)
    {
        m_info->velocity = velocity;
    }
    else
    {
        jassertfalse;
    }
}

// TODO
void Sound::synthesize()
{
    //    std::vector<double> window;
    //    synthesis.parameters.window = generateSynthesisWindow();
    //
    //        // Initialize the synthesis engine
    //        synthesis.engine = std::make_unique<SynthesisEngine>();
    //
    //        // Synthesize Harmonics
    //        synthesis.harmonic = synthesis.engine->sineModelSynth(m_model->values.harmonics_freqs,
    //                                                                          m_model->values.harmonics_mags,
    //                                                                          m_model->values.harmonics_phases,
    //                                                                          analysis.parameters.synthesis_fft_size,
    //                                                                          analysis.parameters.hop_size,
    //                                                                          synthesis.engine->window,
    //                                                                          file.fs);
    //
    //        // TODO - Synthesize Stochastic Component
    //        synthesis.stochastic = synthesis.engine->stochasticModelSynth(m_model->values.stochastic,
    //                                                                                  analysis.parameters.synthesis_fft_size,
    //                                                                                  analysis.parameters.hop_size);
    //
    //    // Sum the harmonic and stochastic components together
    //    synthesis.x = yh[:min(yh.size, yst.size)]+yst[:min(yh.size, yst.size)]

    // TODO - Save more information
    // features.max_frames = m_model->getMaxNumOfFrames (analysis.parameters.hop_size);
    // m_info->sound_length = max_frames * analysis.parameters.hop_size;
    // if (m_info->loopEnd == 0)
    // {
    //     m_info->loopEnd = sound_length;
    // }

    // TODO - Refresh the UI (waveform visualization)
}

void Sound::getFundamentalNotes()
{
    if(m_soundFeatures)
    {
        auto& notes = m_soundFeatures->notes;

        notes.clear();

        // Populate the notes array
        for (auto octaveNumber = -4; octaveNumber < 4; octaveNumber++)
        {
            float a = Constants::A4Hz * powf (2, (float) octaveNumber);

            notes.emplace ((a * powf (2, -9.0 / 12)), 0); // "C"
            notes.emplace ((a * powf (2, -8.0 / 12)), 0); // "C#"
            notes.emplace ((a * powf (2, -7.0 / 12)), 0); // "D"
            notes.emplace ((a * powf (2, -6.0 / 12)), 0); // "D#"
            notes.emplace ((a * powf (2, -5.0 / 12)), 0); // "E"
            notes.emplace ((a * powf (2, -4.0 / 12)), 0); // "F"
            notes.emplace ((a * powf (2, -3.0 / 12)), 0); // "F#"
            notes.emplace ((a * powf (2, -2.0 / 12)), 0); // "G"
            notes.emplace ((a * powf (2, -1.0 / 12)), 0); // "G#"
            notes.emplace ((a), 0);                       // "A"
            notes.emplace ((a * powf (2, 1.0 / 12)), 0);  // "A#"
            notes.emplace ((a * powf (2, 2.0 / 12)), 0);  // "B"
        }

        // Count the number of occurrences for each fundamental present on "harmonic_frequencies"
        for (const auto& frame : m_frames)
        {
            const auto& currentFundamental = frame->getHarmonicComponent().freqs[0];

            // Find closest note for this fundamental
            const auto& closestNote = std::min_element (notes.begin(), notes.end(), [&] (const auto& lowerNote, const auto& upperNote) {
                return std::abs (lowerNote.second - currentFundamental) < std::abs (upperNote.second - currentFundamental);
            });

            if (closestNote != notes.end())
            {
                // Increment the number of occurrences
                ++closestNote->second;
            }
            else
            {
                jassertfalse;
            }
        }

        // Find the predominant fundamental
        const auto& predominantNote = std::max_element (notes.begin(), notes.end(), [] (const auto& lowerNote, const auto& upperNote) {
            return lowerNote.second < upperNote.second;
        });

        if (predominantNote != notes.end())
        {
            // Save the predominant note
            m_soundFeatures->predominantNote = predominantNote->second;
        }
        else
        {
            jassertfalse;
        }
    }
    else
    {
        jassertfalse;
    }
}

// // TODO
// void Sound::normalizeMagnitudes()
// {
//     DBG ("This normalization function must be updated, now it's possible to not have an harmonic component");
//     jassertfalse;

//     // Define normalization range db
//     float min_db = -100.0;
//     float max_db = -1.0;

//     // Initialize min and max values
//     float min_val = 0.0;
//     float max_val = -100.0;

//     // Get the harmonic magnitudes matrix
//     std::vector<std::vector<float>> harmonic_mags = m_model->values.harmonic.mags;

//     // Find the min and max of the harmonic magnitudes matrix
//     for (int i = 0; i < harmonic_mags.size(); i++)
//     {
//         if (harmonic_mags[i].size() > 0)
//         {
//             float local_min_val = *min_element (harmonic_mags[i].begin(), harmonic_mags[i].end());
//             float local_max_val = *max_element (harmonic_mags[i].begin(), harmonic_mags[i].end());

//             if (local_min_val < min_val)
//                 min_val = local_min_val;
//             if (local_max_val > max_val)
//                 max_val = local_max_val;
//         }
//     }

//     // Normalize the harmonic magnitudes
//     for (int i = 0; i < harmonic_mags.size(); i++)
//     {
//         for (int j = 0; j < harmonic_mags[i].size(); j++)
//         {
//             harmonic_mags[i][j] = (max_db - min_db) * ((harmonic_mags[i][j] - min_val) / (max_val - min_val)) + min_db;
//         }
//     }

//     // Set the harmonic magnitudes matrix
//     m_model->setHarmonicMagnitudes (harmonic_mags);

//     // Calcualte the normalization factor applied over the harmonics magnitudes
//     features.normalization_factor = Tools::Calculate::dbToLinear (-std::abs (max_val - max_db));

//     if (m_model->hasSinusoidal())
//     {
//         // Get the sinusoidal magnitudes matrix
//         std::vector<std::vector<float>> sinusoidal_mags = m_model->values.sinusoidal.mags;

//         // Normalize the sinusoidal magnitudes
//         for (int i = 0; i < sinusoidal_mags.size(); i++)
//         {
//             for (int j = 0; j < sinusoidal_mags[i].size(); j++)
//             {
//                 sinusoidal_mags[i][j] = (max_db - min_db) * ((sinusoidal_mags[i][j] - min_val) / (max_val - min_val)) + min_db;
//             }
//         }

//         // Set the sinusoidal magnitudes matrix
//         m_model->setSinusoidalMagnitudes (sinusoidal_mags);
//     }

//     // Attack component normalization
//     if (m_model->hasAttack())
//     {
//         // Get the residual component
//         std::vector<float> attack = m_model->values.attack;

//         // Normalize attack component
//         normalizeWaveform (attack, max_val, max_db);

//         // Set the normalized attack component
//         m_model->setAttack (attack);
//     }

//     // Residual component normalization
//     if (m_model->hasResidual())
//     {
//         // Get the residual component
//         std::vector<float> residual = m_model->values.residual;

//         // Normalize residual component
//         normalizeWaveform (residual, max_val, max_db);

//         // Set the normalized residual component
//         m_model->setResidual (residual);
//     }
// }

//void Sound::normalizeWaveform (std::vector<float>& waveform, float max_val, float max_db)
//{
//    // Define normalization range linear
//    float max_val_from_db = Tools::Calculate::dbToLinear (max_val);
//    float max_linear      = Tools::Calculate::dbToLinear (max_db);
//    float min_linear      = -max_linear;
//
//    // Initialize min and max values
//    float min_val_linear = 0.0;
//    float max_val_linear = 0.0;
//
//    // Find the max value of the waveform
//    for (int i = 0; i < waveform.size(); i++)
//    {
//        float current_val = std::abs (waveform[i]);
//        if (current_val > max_val_linear)
//            max_val_linear = current_val;
//    }
//
//    if (max_val_from_db > max_val_linear)
//    {
//        max_val_linear = max_val_from_db;
//    }
//
//    min_val_linear = -max_val_linear;
//
//    //        float waveform_normalization_factor = std::abs(max_val_linear - max_linear);
//
//    // Normalize the waveform component with the same factor
//    for (int i = 0; i < waveform.size(); i++)
//    {
//        waveform[i] = (max_linear - min_linear) * ((waveform[i] - min_val_linear) / (max_val_linear - min_val_linear)) + min_linear;
//    }
//
//    std::vector<float> results_great;
//    copy_if (waveform.begin(), waveform.end(), back_inserter (results_great), [] (float n) { return n > 1.0; });
//
//    std::vector<float> results_less;
//    copy_if (waveform.begin(), waveform.end(), back_inserter (results_less), [] (float n) { return n < -1.0; });
//
//    jassert (results_great.size() == 0); // Upper clipping
//    jassert (results_less.size() == 0);  // Lower clipping
//}

/** Check if an xml element has any child with a given name */
bool Sound::hasChild (juce::XmlElement* parent, juce::String child_name)
{
    return parent->getChildByName (child_name) != nullptr;
}

/** Split a list of ints in a string */
std::vector<long long> Sound::splitInts (const std::string& list_of_ints)
{
    std::string clean_list_of_ints = list_of_ints.substr (1, list_of_ints.size() - 2); // Removing the "[]"
    std::replace_if (
        clean_list_of_ints.begin(), clean_list_of_ints.end(), [] (const char& c) { return c == ','; }, ' ');
    //        [] (const char& c) { return std::ispunct(c) ;},' ');
    std::stringstream      ss (clean_list_of_ints);
    std::vector<long long> result_int;

    int num;
    while (ss >> num)
        result_int.push_back (num);

    return result_int;
}

/** Split a list of floats in a string */
std::vector<float> Sound::splitFloats (const std::string& list_of_floats, bool int_to_float)
{
    if (int_to_float)
    {
        std::string clean_list_of_ints = list_of_floats.substr (1, list_of_floats.size() - 2); // Removing the "[]"
        std::replace_if (
            clean_list_of_ints.begin(), clean_list_of_ints.end(), [] (const char& c) { return c == ','; }, ' ');
        //        [] (const char& c) { return std::ispunct(c) ;},' ');
        std::stringstream ss (clean_list_of_ints);
        ss.precision (20);
        std::vector<float> result_float;

        int num;
        while (ss >> num)
            result_float.push_back ((float) num);

        return result_float;
    }
    else
    {
        // TODO - not remove commas, just skip them
        std::string clean_list_of_floats = list_of_floats.substr (1, list_of_floats.size() - 2);                                              // Removing the "[]"
        clean_list_of_floats.erase (std::remove (clean_list_of_floats.begin(), clean_list_of_floats.end(), ','), clean_list_of_floats.end()); // Removing the ","
        std::istringstream iss (clean_list_of_floats);

        return std::vector<float> {
            std::istream_iterator<float> (iss),
            std::istream_iterator<float>()
        };
    }
}

/** Get a vector of floats from an xml element */
std::vector<float> Sound::getVectorOfFloats (juce::XmlElement* parent, juce::String child_name)
{
    return splitFloats (parent->getChildByName (child_name)->getAllSubText().toStdString(), true);
}

/** Get a matrix of ints from an xml element */
std::vector<std::vector<long long>> Sound::getMatrixOfInts (juce::XmlElement* parent, juce::String child_name)
{
    std::vector<std::vector<long long>> result_matrix;

    //    int i=1;
    // For each row of the matrix
    forEachXmlChildElementWithTagName (*parent, child, child_name)
    {
        //        if(i == 28){
        //            DBG("CHECK");
        //        }
        //        auto aux = child->getAllSubText();
        result_matrix.push_back (splitInts (child->getAllSubText().toStdString()));
        //        i++;
    }

    return result_matrix;
}

/** Get a matrix of floats from an xml element */
std::vector<std::vector<float>> Sound::getMatrixOfFloats (juce::XmlElement* parent, juce::String child_name)
{
    std::vector<std::vector<float>> result_matrix;

    //    int i=1;
    // For each row of the matrix
    forEachXmlChildElementWithTagName (*parent, child, child_name)
    {
        //        if(i == 28){
        //            DBG("CHECK");
        //        }
        //        auto aux = child->getAllSubText();
        result_matrix.push_back (splitFloats (child->getAllSubText().toStdString(), true));
        //        i++;
    }

    return result_matrix;
}

// // Returns element closest to target in arr[]
// float Sound::findClosest (float arr[], int n, float target)
// {
//     // Corner cases
//     if (target <= arr[0])
//         return arr[0];
//     if (target >= arr[n - 1])
//         return arr[n - 1];

//     // Doing binary search
//     int i = 0, j = n, mid = 0;
//     while (i < j)
//     {
//         mid = (i + j) / 2;

//         if (arr[mid] == target)
//             return arr[mid];

//         // If target is less than array element, then search in left
//         if (target < arr[mid])
//         {
//             // If target is greater than previous to mid, return closest of two
//             if (mid > 0 && target > arr[mid - 1])
//                 return getClosestValue (arr[mid - 1], arr[mid], target);

//             // Repeat for left half
//             j = mid;
//         }

//         // If target is greater than mid
//         else
//         {
//             if (mid < n - 1 && target < arr[mid + 1])
//                 return getClosestValue (arr[mid], arr[mid + 1], target);
//             // update i
//             i = mid + 1;
//         }
//     }

//     // Only single element left after search
//     return arr[mid];
// }

// // Method to compare which one is the closest.
// // We find the closest by taking the difference
// // between the target and both values. It assumes
// // that val2 is greater than val1 and target lies
// // between these two.
// float Sound::getClosestValue (float val1, float val2, float target)
// {
//     if (target - val1 >= val2 - target)
//         return val2;
//     else
//         return val1;
// }

} // namespace morphex
