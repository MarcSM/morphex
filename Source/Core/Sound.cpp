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

#include "../helpers/utils.h"
#include "../helpers/helper_functions.h"

// TODO - Adapt / remake the "Sound" class to use "ValueTree"
// TODO - Merge with synth_sound

namespace Constants
{
constexpr auto CurrentFileVersion = 1;
} // namespace Constants

namespace morphex
{
Sound::Sound()
{
    init();
};

// Read the file from a file_path
Sound::Sound (std::string file_path)
{
    init();

    // Load data from file_path
    juce::String file_data = loadDataFromFile (file_path);

    // Load the sound
    load (file_data, HadFileSource::Binary);
};

Sound::Sound (std::string file_path, int note, int velocity)
{
    init();

    note     = note;
    velocity = velocity;

    // Load data from file_path
    juce::String file_data = loadDataFromFile (file_path);

    // Load the sound
    load (file_data, HadFileSource::Binary);
};

// Read the file from a binary file with an hypothetical file_path
Sound::Sound (juce::String file_data, std::string file_path)
{
    init();

    // Save the file_path
    path = file_path;

    // Load the sound
    load (file_data, HadFileSource::Binary);
};

Sound::~Sound() {};

void Sound::init (bool init_model)
{
    // Initialize default values
    path            = "";
    name            = "";
    extension       = "";
    loaded          = false;
    analyzed        = false;
    had_file_loaded = false;

    // Initializing default values
    //    x = []
    fs            = 44100;
    note          = 0;
    velocity      = 0;
    max_harmonics = 0;
    max_frames    = 0;
    sound_length  = 0;
    loop.start    = 0;
    loop.end      = 0;

    if (init_model)
        model = new Model();
}

void Sound::reset()
{
    init (false);

    model->reset();
}

// Load data of ".had" file
juce::String Sound::loadDataFromFile (std::string file_path)
{
    juce::File file = juce::File (file_path);
    name      = file.getFileNameWithoutExtension().toStdString();
    extension = file.getFileExtension().toStdString();
    path      = file_path;
    dirpath   = file.getParentDirectory().getFullPathName().toStdString();

    std::stringstream had_file_path_aux;
    had_file_path_aux << dirpath << static_cast<std::string> (juce::File::getSeparatorString()) << name << ".had";

    std::string had_file_path = had_file_path_aux.str();
    juce::File        had_file      = juce::File (had_file_path);

    return had_file.loadFileAsString();
}

void Sound::load (juce::String file_data, HadFileSource file_source)
{
    // Initialize default values
    loaded = false;

    // If file_data is a file_path
    if (file_source == HadFileSource::Path)
    {
        juce::String file_path = file_data;
        file_data        = loadDataFromFile (file_data.toStdString());
    }

    std::unique_ptr<juce::XmlElement> xml (juce::XmlDocument::parse (file_data));

    if (xml)
    {
        try
        {
            juce::XmlElement* xml_file = xml->getChildByName ("file"); // had["file"]

            if (xml_file == nullptr)
                throw "Sound file not compatible";

            file_version = hasChild (xml_file, "v") ? xml_file->getChildByName ("v")->getAllSubText().getIntValue() : 0;

            if (file_version == Constants::CurrentFileVersion and hasChild (xml_file, "dp"))
            {
                decimal_places = xml_file->getChildByName ("dp")->getAllSubText().getIntValue();

                // Sound
                juce::XmlElement* xml_sound = xml->getChildByName ("sound"); // had["sound"]
                fs                    = xml_sound->getChildByName ("fs")->getAllSubText().getIntValue();
                note                  = xml_sound->getChildByName ("note")->getAllSubText().getIntValue();
                velocity              = xml_sound->getChildByName ("velocity")->getAllSubText().getIntValue();
                max_harmonics         = xml_sound->getChildByName ("max_harmonics")->getAllSubText().getIntValue();
                max_frames            = xml_sound->getChildByName ("max_frames")->getAllSubText().getIntValue();
                loop.start            = xml_sound->getChildByName ("loop")->getChildByName ("start")->getAllSubText().getIntValue();
                loop.end              = xml_sound->getChildByName ("loop")->getChildByName ("end")->getAllSubText().getIntValue();

                // Model
                juce::XmlElement* xml_synthesis = xml->getChildByName ("synthesis"); // had["synthesis"]
                model->harmonic           = hasChild (xml_synthesis, "h");
                model->sinusoidal         = hasChild (xml_synthesis, "s");
                model->stochastic         = hasChild (xml_synthesis, "c");
                model->attack             = hasChild (xml_synthesis, "a");
                model->residual           = hasChild (xml_synthesis, "r");
                if (model->harmonic)
                {
                    juce::XmlElement* xml_synthesis_harmonic = xml_synthesis->getChildByName ("h"); // had["synthesis"]["h"]
                    model->setHarmonic (getMatrixOfInts (xml_synthesis_harmonic, "f"),
                                        getMatrixOfInts (xml_synthesis_harmonic, "m"),
                                        getMatrixOfInts (xml_synthesis_harmonic, "p"),
                                        true);
                }
                if (model->sinusoidal)
                {
                    juce::XmlElement* xml_synthesis_sinusoidal = xml_synthesis->getChildByName ("s"); // had["synthesis"]["s"]
                    model->setSinusoidal (getMatrixOfInts (xml_synthesis_sinusoidal, "f"),
                                          getMatrixOfInts (xml_synthesis_sinusoidal, "m"),
                                          getMatrixOfInts (xml_synthesis_sinusoidal, "p"),
                                          true);
                }
                if (model->stochastic)
                    model->setStochastic (getMatrixOfFloats (xml_synthesis, "c"), true);
                if (model->attack)
                    model->setAttack (getVectorOfFloats (xml_synthesis, "a"), true);
                if (model->residual)
                    model->setResidual (getVectorOfFloats (xml_synthesis, "r"), true);

                // If source file is binary data, fill the missing attributes
                if (name.empty())
                    name = file.name;
                if (extension.empty())
                    extension = ".had";
                //                    if (path.empty()) path = "";
                //                    if (dirpath.empty()) dirpath = "";

                // Analysis parameters
                juce::XmlElement* xml_parameters              = xml->getChildByName ("parameters"); // had["parameters"]
                analysis.parameters.window_type         = (WindowType) xml_parameters->getChildByName ("window_type")->getAllSubText().getIntValue();
                analysis.parameters.window_size         = xml_parameters->getChildByName ("window_size")->getAllSubText().getIntValue();
                analysis.parameters.fft_size            = xml_parameters->getChildByName ("fft_size")->getAllSubText().getIntValue();
                analysis.parameters.magnitude_threshold = xml_parameters->getChildByName ("magnitude_threshold")->getAllSubText().getIntValue();
                analysis.parameters.hearing_threshold   = xml_parameters->getChildByName ("hearing_threshold")->getAllSubText().getIntValue();
                analysis.parameters.min_sine_dur        = xml_parameters->getChildByName ("min_sine_dur")->getAllSubText().getDoubleValue();
                analysis.parameters.max_harm            = xml_parameters->getChildByName ("max_harm")->getAllSubText().getIntValue();
                analysis.parameters.min_f0              = xml_parameters->getChildByName ("min_f0")->getAllSubText().getIntValue();
                analysis.parameters.max_f0              = xml_parameters->getChildByName ("max_f0")->getAllSubText().getIntValue();
                analysis.parameters.max_f0_error        = xml_parameters->getChildByName ("max_f0_error")->getAllSubText().getIntValue();
                analysis.parameters.harm_dev_slope      = xml_parameters->getChildByName ("harm_dev_slope")->getAllSubText().getDoubleValue();
                analysis.parameters.stoc_fact           = xml_parameters->getChildByName ("stoc_fact")->getAllSubText().getDoubleValue();
                analysis.parameters.synthesis_fft_size  = xml_parameters->getChildByName ("synthesis_fft_size")->getAllSubText().getIntValue();
                analysis.parameters.hop_size            = xml_parameters->getChildByName ("hop_size")->getAllSubText().getIntValue();

                //                    // Fix missing data
                //                    if (max_frames == 0)
                //                    {
                //                        max_frames = (int)std::max({
                //                            model->values.harmonic.freqs.size(),
                //                            model->values.harmonic.mags.size(),
                //                            model->values.harmonic.phases.size(),
                //                            model->values.sinusoidal.freqs.size(),
                //                            model->values.sinusoidal.mags.size(),
                //                            model->values.sinusoidal.phases.size(),
                //                            model->values.stochastic.size(),
                //                            model->values.attack.size(),
                //                            model->values.residual.size()
                //                        });
                //                    };

                // TODO TOFIX - Fix max_frames on .had files (max_frames    int    22050 (attack))
                // and add XML_DECIMAL_PLACES on the .had file too
                max_frames = (int) std::max ({ (int) model->values.harmonic.freqs.size(),
                                               (int) model->values.harmonic.mags.size(),
                                               (int) model->values.harmonic.phases.size(),
                                               (int) model->values.sinusoidal.freqs.size(),
                                               (int) model->values.sinusoidal.mags.size(),
                                               (int) model->values.sinusoidal.phases.size(),
                                               (int) model->values.stochastic.size(),
                                               (int) std::trunc (model->values.attack.size() / analysis.parameters.hop_size),
                                               (int) std::trunc (model->values.residual.size() / analysis.parameters.hop_size) });

                sound_length = max_frames * analysis.parameters.hop_size;
                if (loop.end == 0)
                    loop.end = sound_length;

                // TODO TEST - Temporal override
                loop.end = (max_frames - 24) * analysis.parameters.hop_size;

                //                    // Synthesize the original sound
                //                    synthesize();
                //
                // Extract Additional Features
                extractFeatures();

                // Save Original Values
                //                saveOriginalValues();

                /** Normalize magnitudes on load by default */
                // TODO - Fix this method
                //                normalizeMagnitudes();

                /** Updating flags */
                had_file_loaded = true;

                // TODO - Test
                loaded = true;

                /* TODO? - Refreshing the UI (waveform visualization) */
            }
            else
            {
                // TODO - Raise and catch exception
                std::cout << "Error while loading the sound\n";
                jassertfalse;

                reset();
            }
        }
        //            catch (std::exception& e)
        catch (const char* msg)
        {
            std::cout << "Error while loading the sound: '" << msg << "'\n";
            //                jassertfalse;
        }
    }
    else
    {
        std::cout << "Error while loading the sound\n";
        //            jassertfalse;
    }
};

//    // TODO - If there is no ".mif" (Morphex Instrument File)
//    // It is necessary to preload or even pre-analyze the sound file
//    // to locate it on the right note. Try to use copy constructors
//    // to prevent process the whole file again.
//    void Sound::getInstrumentFromFile(std::string file_path)
//    {
//        File had_file = File(file_path);
//        file_data = had_file.loadFileAsString();
//        return
//
//        XmlDocument    (    const File &     file    )
//
//
//        std::unique_ptr<XmlElement> xml( XmlDocument::parse( file_data ) );
//
//        std::string file_path
//    }

Sound::Frame Sound::getFrame (int i_num_frame, int i_frame_length)
{
    // Output
    Sound::Frame sound_frame;

    // TODO - Improve the name tagging for the components of a frame
    if (i_num_frame < model->values.harmonic.freqs.size())
    {
        sound_frame.harmonic.freqs  = getComponentFrame (Frame::Component::HarmonicFreqs, i_num_frame);
        sound_frame.harmonic.mags   = getComponentFrame (Frame::Component::HarmonicMags, i_num_frame);
        sound_frame.harmonic.phases = getComponentFrame (Frame::Component::HarmonicPhases, i_num_frame);
        //            sound_frame.harmonic.phases = std::vector<float>(0);
    }
    else
    {
        sound_frame.harmonic.freqs  = std::vector<float> (0);
        sound_frame.harmonic.mags   = std::vector<float> (0);
        sound_frame.harmonic.phases = std::vector<float> (0);
    }

    if (i_num_frame < model->values.sinusoidal.freqs.size())
    {
        sound_frame.sinusoidal.freqs  = getComponentFrame (Frame::Component::SinusoidalFreqs, i_num_frame);
        sound_frame.sinusoidal.mags   = getComponentFrame (Frame::Component::SinusoidalMags, i_num_frame);
        sound_frame.sinusoidal.phases = getComponentFrame (Frame::Component::SinusoidalPhases, i_num_frame);
        //            sound_frame.sinusoidal.phases = std::vector<float>(0);
    }
    else
    {
        sound_frame.sinusoidal.freqs  = std::vector<float> (0);
        sound_frame.sinusoidal.mags   = std::vector<float> (0);
        sound_frame.sinusoidal.phases = std::vector<float> (0);
    }

    if (i_num_frame < model->values.stochastic.size())
    {
        sound_frame.stochastic = getComponentFrame (Frame::Component::Stochastic, i_num_frame);
    }

    if ((i_num_frame * i_frame_length) < model->values.attack.size())
    {
        sound_frame.attack = getComponentFrame (Frame::Component::Attack, i_num_frame, i_frame_length);
    }
    else
    {
        sound_frame.attack = std::vector<float> (0);
    }

    if ((i_num_frame * i_frame_length) < model->values.residual.size())
    {
        sound_frame.residual = getComponentFrame (Frame::Component::Residual, i_num_frame, i_frame_length);
    }
    else
    {
        sound_frame.residual = std::vector<float> (0);
    }

    return sound_frame;
}

// TODO - TOOPT- Optimize CPU
std::vector<float> Sound::getComponentFrame (Frame::Component component_name, int i_num_frame, int i_frame_length)
{
    // Output
    std::vector<float> component_frame;

    switch (component_name)
    {
        case Frame::Component::HarmonicFreqs:
            component_frame = model->values.harmonic.freqs[i_num_frame];
            break;
        case Frame::Component::HarmonicMags:
            component_frame = model->values.harmonic.mags[i_num_frame];
            break;
        case Frame::Component::HarmonicPhases:
            if (model->values.harmonic.phases.size() > i_num_frame)
            {
                component_frame = model->values.harmonic.phases[i_num_frame];
            }
            else
            {
                component_frame = std::vector<float> (0);
            }
            break;
        case Frame::Component::SinusoidalFreqs:
            component_frame = model->values.sinusoidal.freqs[i_num_frame];
            break;
        case Frame::Component::SinusoidalMags:
            component_frame = model->values.sinusoidal.mags[i_num_frame];
            break;
        case Frame::Component::SinusoidalPhases:
            if (model->values.sinusoidal.phases.size() > i_num_frame)
            {
                component_frame = model->values.sinusoidal.phases[i_num_frame];
            }
            else
            {
                component_frame = std::vector<float> (0);
            }
            break;
        case Frame::Component::Stochastic:
            component_frame = model->values.stochastic[i_num_frame];
            break;
        case Frame::Component::Attack:
        case Frame::Component::Residual:
        {
            // TODO - Improve the CPU on this section

            std::vector<float> vector_component = model->values.attack;

            if (component_name == Frame::Component::Residual)
            {
                vector_component = model->values.residual;
            }

            int i_start_sample = i_num_frame * i_frame_length;
            int i_end_sample   = i_start_sample + i_frame_length;
            int i_vec_size     = (int) vector_component.size();

            if (i_vec_size < i_end_sample)
            {
                i_end_sample = i_vec_size;
            }

            if (i_vec_size < i_start_sample)
            {
                i_start_sample = i_vec_size;
            }

            std::vector<float> residual_frame = Tools::Get::valuesInRange (vector_component, i_start_sample, i_end_sample);

            for (int i = 0; i < residual_frame.size(); i++)
            {
                //                    component_frame[i] = residual_frame[i];
                component_frame.push_back (residual_frame[i]);
            }

            break;
        }
    }

    return component_frame;
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
    //        synthesis.harmonic = synthesis.engine->sineModelSynth(model->values.harmonics_freqs,
    //                                                                          model->values.harmonics_mags,
    //                                                                          model->values.harmonics_phases,
    //                                                                          analysis.parameters.synthesis_fft_size,
    //                                                                          analysis.parameters.hop_size,
    //                                                                          synthesis.engine->window,
    //                                                                          file.fs);
    //
    //        // TODO - Synthesize Stochastic Component
    //        synthesis.stochastic = synthesis.engine->stochasticModelSynth(model->values.stochastic,
    //                                                                                  analysis.parameters.synthesis_fft_size,
    //                                                                                  analysis.parameters.hop_size);
    //
    //    // Sum the harmonic and stochastic components together
    //    synthesis.x = yh[:min(yh.size, yst.size)]+yst[:min(yh.size, yst.size)]
}

void Sound::extractFeatures()
{
    // Extract the fundamentals
    getFundamentalNotes();
}

void Sound::getFundamentalNotes()
{
    // Define the "notes" array
    SoundFeatures::NotesMap notes;

    // Define A4 in Hz
    const float A4 = 440.0;

    // Populate the "notes" array
    for (int i_octave = -4; i_octave < 4; i_octave++)
    {
        float a = A4 * powf (2, (float) i_octave);

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

    float notes_array[notes.size()];

    int i_note = 0;

    for (const auto& [frequency, number_of_occurrences] : notes)
    {
        notes_array[i_note] = frequency;
        i_note++;
    }

    // Count the number of occurrences for each fundamental present on "harmonic_frequencies"
    for (int i_frame = 0; i_frame < model->values.harmonic.freqs.size(); i_frame++)
    {
        if (model->values.harmonic.freqs[i_frame].size() > 0)
        {
            // Current fundamental
            float current_fundamental = model->values.harmonic.freqs[i_frame][0];

            // Find the closest note for this fundamental in Hz
            float i_closest_note = findClosest (notes_array, (int) notes.size(), current_fundamental);

            // Incrementing the number of occurrences
            notes.find (i_closest_note)->second++;
        }
    }

    // Save the result on features.fundamentals
    features.notes = notes;

    // Find the predominant fundamental
    typedef SoundFeatures::NotesMap::iterator iter;
    iter                                      it  = notes.begin();
    iter                                      end = notes.end();

    float max_frequency  = it->first;
    int   max_occurences = it->second;
    for (; it != end; ++it)
    {
        if (it->second > max_occurences)
        {
            max_frequency  = it->first;
            max_occurences = it->second;
        }
    }

    // Save the predominant note
    features.predominant_note = max_frequency;
}

void Sound::saveOriginalValues()
{
    original.harmonic.freqs    = model->values.harmonic.freqs;
    original.harmonic.mags     = model->values.harmonic.mags;
    original.harmonic.phases   = model->values.harmonic.phases;
    original.sinusoidal.freqs  = model->values.sinusoidal.freqs;
    original.sinusoidal.mags   = model->values.sinusoidal.mags;
    original.sinusoidal.phases = model->values.sinusoidal.phases;
    original.attack            = model->values.attack;
    original.stochastic        = model->values.stochastic;
    original.residual          = model->values.residual;
}

// TODO
void Sound::normalizeMagnitudes()
{
    jassertfalse;
    DBG ("This normalization function must be updated, now it's possible to not have an harmonic component");

    // Define normalization range db
    float min_db = -100.0;
    float max_db = -1.0;

    // Initialize min and max values
    float min_val = 0.0;
    float max_val = -100.0;

    // Get the harmonic magnitudes matrix
    std::vector<std::vector<float>> harmonic_mags = model->values.harmonic.mags;

    // Find the min and max of the harmonic magnitudes matrix
    for (int i = 0; i < harmonic_mags.size(); i++)
    {
        if (harmonic_mags[i].size() > 0)
        {
            float local_min_val = *min_element (harmonic_mags[i].begin(), harmonic_mags[i].end());
            float local_max_val = *max_element (harmonic_mags[i].begin(), harmonic_mags[i].end());

            if (local_min_val < min_val)
                min_val = local_min_val;
            if (local_max_val > max_val)
                max_val = local_max_val;
        }
    }

    // Normalize the harmonic magnitudes
    for (int i = 0; i < harmonic_mags.size(); i++)
    {
        for (int j = 0; j < harmonic_mags[i].size(); j++)
        {
            harmonic_mags[i][j] = (max_db - min_db) * ((harmonic_mags[i][j] - min_val) / (max_val - min_val)) + min_db;
        }
    }

    // Set the harmonic magnitudes matrix
    model->setHarmonicMagnitudes (harmonic_mags);

    // Calcualte the normalization factor applied over the harmonics magnitudes
    features.normalization_factor = Tools::Calculate::dbToLinear (-std::abs (max_val - max_db));

    if (model->hasSinusoidal())
    {
        // Get the sinusoidal magnitudes matrix
        std::vector<std::vector<float>> sinusoidal_mags = model->values.sinusoidal.mags;

        // Normalize the sinusoidal magnitudes
        for (int i = 0; i < sinusoidal_mags.size(); i++)
        {
            for (int j = 0; j < sinusoidal_mags[i].size(); j++)
            {
                sinusoidal_mags[i][j] = (max_db - min_db) * ((sinusoidal_mags[i][j] - min_val) / (max_val - min_val)) + min_db;
            }
        }

        // Set the sinusoidal magnitudes matrix
        model->setSinusoidalMagnitudes (sinusoidal_mags);
    }

    // Attack component normalization
    if (model->hasAttack())
    {
        // Get the residual component
        std::vector<float> attack = model->values.attack;

        // Normalize attack component
        normalizeWaveform (attack, max_val, max_db);

        // Set the normalized attack component
        model->setAttack (attack);
    }

    // Residual component normalization
    if (model->hasResidual())
    {
        // Get the residual component
        std::vector<float> residual = model->values.residual;

        // Normalize residual component
        normalizeWaveform (residual, max_val, max_db);

        // Set the normalized residual component
        model->setResidual (residual);
    }
}

void Sound::normalizeWaveform (std::vector<float>& waveform, float max_val, float max_db)
{
    // Define normalization range linear
    float max_val_from_db = Tools::Calculate::dbToLinear (max_val);
    float max_linear      = Tools::Calculate::dbToLinear (max_db);
    float min_linear      = -max_linear;

    // Initialize min and max values
    float min_val_linear = 0.0;
    float max_val_linear = 0.0;

    // Find the max value of the waveform
    for (int i = 0; i < waveform.size(); i++)
    {
        float current_val = std::abs (waveform[i]);
        if (current_val > max_val_linear)
            max_val_linear = current_val;
    }

    if (max_val_from_db > max_val_linear)
    {
        max_val_linear = max_val_from_db;
    }

    min_val_linear = -max_val_linear;

    //        float waveform_normalization_factor = std::abs(max_val_linear - max_linear);

    // Normalize the waveform component with the same factor
    for (int i = 0; i < waveform.size(); i++)
    {
        waveform[i] = (max_linear - min_linear) * ((waveform[i] - min_val_linear) / (max_val_linear - min_val_linear)) + min_linear;
    }

    std::vector<float> results_great;
    copy_if (waveform.begin(), waveform.end(), back_inserter (results_great), [] (float n) { return n > 1.0; });

    std::vector<float> results_less;
    copy_if (waveform.begin(), waveform.end(), back_inserter (results_less), [] (float n) { return n < -1.0; });

    jassert (results_great.size() == 0); // Upper clipping
    jassert (results_less.size() == 0);  // Lower clipping
}
} // namespace moprhex
