/*
  ==============================================================================

    Sound.cpp
    Created: 19 Jul 2019 6:00:01pm
    Author:  Marc Sanchez Martinez

  ==============================================================================
*/

#include "Tools.h"
#include "Sound.h"

#include "SMTHelperFunctions.h"
#include "SMTAudioHelpers.h"

namespace Core
{
    // TODO - Adapt / remake the "Sound" class to use "ValueTree"
    
    Sound::Sound()
    {
        this->init();
    };
    
//    Sound::Sound (const Sound& obj)
//    {
//
//    }
    
//    Sound::Sound (const Sound& obj)
//    :   model (obj.model)
//    {
//        // Initialize default values
//        this->loaded = obj.loaded;
//        this->analyzed = obj.analyzed;
//        this->had_file_loaded = obj.had_file_loaded;
//
//        // Initializing default values
//        this->fs = obj.fs;
//        this->note = obj.note;
//        this->velocity = obj.velocity;
//        this->max_harmonics = obj.max_harmonics;
//        this->max_frames = obj.max_frames;
//        this->loop = obj.loop;
//
////        // Model object
////        this->model = obj.model;
//
//        // File
//        this->file = obj.file;
//        this->path = obj.path;
//        this->name = obj.name;
//        this->extension = obj.extension;
//        this->dirpath = obj.dirpath;
//
//        // Analysis
//        this->analysis = obj.analysis;
//
//        // Synthesis
//        this->synthesis = obj.synthesis;
//
//        // Features
//        this->features = obj.features;
//
//        // Original Values
//        this->original = obj.original;
//    };

    // Read the file from a file_path
    Sound::Sound (std::string file_path)
    {
        this->init();
        
        // Load data from file_path
        String file_data = loadDataFromFile(file_path);
        
        // Load the sound
        this->load(file_data, HadFileSource::Binary);
    };

    Sound::Sound (std::string file_path, int note, int velocity)
    {
        this->init();
        
        this->note = note;
        this->velocity = velocity;
        
        // Load data from file_path
        String file_data = loadDataFromFile(file_path);
        
        // Load the sound
        this->load(file_data, HadFileSource::Binary);
    };

    // Read the file from a binary file with an hypothetical file_path
    Sound::Sound (String file_data, std::string file_path)
    {
        this->init();
        
        // Save the file_path
        this->path = file_path;
        
        // Load the sound
        this->load(file_data, HadFileSource::Binary);
    };

    Sound::~Sound() {};
    
    void Sound::init (bool init_model)
    {
        // Initialize default values
        this->path = "";
        this->name = "";
        this->extension = "";
        this->loaded = false;
        this->analyzed = false;
        this->had_file_loaded = false;
        
        // Initializing default values
        //    this->x = []
        this->fs = 44100;
        this->note = 0;
        this->velocity = 0;
        this->max_harmonics = 0;
        this->max_frames = 0;
        this->sound_length = 0;
        this->loop.start = 0;
        this->loop.end = 0;
        
        if (init_model) this->model = new Model();
        
        // Model object
        //        if (this->model == nullptr) this->model = new Model();
        //        else this->model->reset();
        
        
        //    // Initializing the harmonic analysis data structure for the .had file
        //    this->had = {}
    }
    
    void Sound::reset()
    {
        this->init (false);
        
        this->model->reset();
    }

    // Load data of ".had" file
    String Sound::loadDataFromFile (std::string file_path)
    {
        File file = File(file_path);
        this->name = file.getFileNameWithoutExtension().toStdString();
        this->extension = file.getFileExtension().toStdString();
        this->path = file_path;
        this->dirpath = file.getParentDirectory().getFullPathName().toStdString();
        
        std::stringstream had_file_path_aux;
        had_file_path_aux << this->dirpath << directorySeparator << this->name << ".had";
        
        std::string had_file_path = had_file_path_aux.str();
        File had_file = File(had_file_path);
        
        return had_file.loadFileAsString();
    }

    void Sound::load (String file_data, HadFileSource file_source)
    {
        // Initialize default values
        this->loaded = false;
        
        // If file_data is a file_path
        if (file_source == HadFileSource::Path)
        {
            String file_path = file_data;
            file_data = loadDataFromFile(file_data.toStdString());
        }
        
        std::unique_ptr<XmlElement> xml( XmlDocument::parse( file_data ) );
        
        if (xml)
        {
            try
            {
//                if (hasChild(xml, "file"))
//                {
//                    
//                }
                // File
                XmlElement* xml_file = xml->getChildByName("file"); // had["file"]
                
                if (xml_file == nullptr)
                {
                    throw "Sound file not compatible";
                }
                
                this->file_version = hasChild(xml_file, "v") ? xml_file->getChildByName("v")->getAllSubText().getIntValue() : 0;
                
                if (this->file_version == CURRENT_FILE_VERSION and hasChild(xml_file, "dp"))
                {
                    this->decimal_places = xml_file->getChildByName("dp")->getAllSubText().getIntValue();
                    
                    // Sound
                    XmlElement* xml_sound = xml->getChildByName("sound"); // had["sound"]
                    this->fs = xml_sound->getChildByName("fs")->getAllSubText().getIntValue();
                    // TODO - Embeded binary don't have note and velocity
                    this->note = xml_sound->getChildByName("note")->getAllSubText().getIntValue();
                    this->velocity = xml_sound->getChildByName("velocity")->getAllSubText().getIntValue();
                    this->max_harmonics = xml_sound->getChildByName("max_harmonics")->getAllSubText().getIntValue();
                    this->max_frames = xml_sound->getChildByName("max_frames")->getAllSubText().getIntValue();
                    this->loop.start = xml_sound->getChildByName("loop")->getChildByName("start")->getAllSubText().getIntValue();
                    this->loop.end = xml_sound->getChildByName("loop")->getChildByName("end")->getAllSubText().getIntValue();
                    
                    // Model
                    XmlElement* xml_synthesis = xml->getChildByName("synthesis"); // had["synthesis"]
                    this->model->harmonic   = hasChild(xml_synthesis, "h");
                    this->model->sinusoidal = hasChild(xml_synthesis, "s");
                    this->model->stochastic = hasChild(xml_synthesis, "c");
                    this->model->attack     = hasChild(xml_synthesis, "a");
                    this->model->residual   = hasChild(xml_synthesis, "r");
                    if (this->model->harmonic)
                    {
                        XmlElement* xml_synthesis_harmonic = xml_synthesis->getChildByName("h"); // had["synthesis"]["h"]
                        this->model->setHarmonic (getMatrixOfInts (xml_synthesis_harmonic, "f"),
                                                  getMatrixOfInts (xml_synthesis_harmonic, "m"), true);
                    }
                    if (this->model->sinusoidal)
                    {
                        XmlElement* xml_synthesis_sinusoidal = xml_synthesis->getChildByName("s"); // had["synthesis"]["s"]
                        this->model->setSinusoidal (getMatrixOfInts (xml_synthesis_sinusoidal, "f"),
                                                    getMatrixOfInts (xml_synthesis_sinusoidal, "m"), true);
                    }
                    if (this->model->stochastic)    this->model->setStochastic  (getMatrixOfFloats(xml_synthesis, "c"), true);
                    if (this->model->attack)        this->model->setAttack      (getVectorOfFloats(xml_synthesis, "a"), true);
                    if (this->model->residual)      this->model->setResidual    (getVectorOfFloats(xml_synthesis, "r"), true);
                    
                    // If source file is binary data, fill the missing attributes
                    if (this->name.empty()) this->name = this->file.name;
                    if (this->extension.empty()) this->extension = ".had";
                    //                if (this->path.empty()) this->path = "";
                    //                if (this->dirpath.empty()) this->dirpath = "";
                    
                    // Analysis parameters
                    XmlElement* xml_parameters = xml->getChildByName("parameters"); // had["parameters"]
                    this->analysis.parameters.window_type = (WindowType)xml_parameters->getChildByName("window_type")->getAllSubText().getIntValue();
                    this->analysis.parameters.window_size = xml_parameters->getChildByName("window_size")->getAllSubText().getIntValue();
                    this->analysis.parameters.fft_size = xml_parameters->getChildByName("fft_size")->getAllSubText().getIntValue();
                    this->analysis.parameters.magnitude_threshold = xml_parameters->getChildByName("magnitude_threshold")->getAllSubText().getIntValue();
                    this->analysis.parameters.hearing_threshold = xml_parameters->getChildByName("hearing_threshold")->getAllSubText().getIntValue();
                    this->analysis.parameters.min_sine_dur = xml_parameters->getChildByName("min_sine_dur")->getAllSubText().getDoubleValue();
                    this->analysis.parameters.max_harm = xml_parameters->getChildByName("max_harm")->getAllSubText().getIntValue();
                    this->analysis.parameters.min_f0 = xml_parameters->getChildByName("min_f0")->getAllSubText().getIntValue();
                    this->analysis.parameters.max_f0 = xml_parameters->getChildByName("max_f0")->getAllSubText().getIntValue();
                    this->analysis.parameters.max_f0_error = xml_parameters->getChildByName("max_f0_error")->getAllSubText().getIntValue();
                    this->analysis.parameters.harm_dev_slope = xml_parameters->getChildByName("harm_dev_slope")->getAllSubText().getDoubleValue();
                    this->analysis.parameters.stoc_fact = xml_parameters->getChildByName("stoc_fact")->getAllSubText().getDoubleValue();
                    this->analysis.parameters.synthesis_fft_size = xml_parameters->getChildByName("synthesis_fft_size")->getAllSubText().getIntValue();
                    this->analysis.parameters.hop_size = xml_parameters->getChildByName("hop_size")->getAllSubText().getIntValue();
                    
                    // Fix missing data
                    //                if (this->max_frames == 0)
                    //                {
                    //                    this->max_frames = (int)std::max({
                    //                        this->model->values.harmonic.freqs.size(),
                    //                        this->model->values.harmonic.mags.size(),
                    //                        this->model->values.harmonic.phases.size(),
                    //                        this->model->values.sinusoidal.freqs.size(),
                    //                        this->model->values.sinusoidal.mags.size(),
                    //                        this->model->values.sinusoidal.phases.size(),
                    //                        this->model->values.stochastic.size(),
                    //                        this->model->values.attack.size(),
                    //                        this->model->values.residual.size()
                    //                    });
                    //                };
                    
                    // TODO TOFIX - Fix max_frames on .had files (max_frames    int    22050 (attack))
                    // and add XML_DECIMAL_PLACES on the .had file too
                    this->max_frames = (int)std::max({
                        this->model->values.harmonic.freqs.size(),
                        this->model->values.harmonic.mags.size(),
                        this->model->values.harmonic.phases.size(),
                        this->model->values.sinusoidal.freqs.size(),
                        this->model->values.sinusoidal.mags.size(),
                        this->model->values.sinusoidal.phases.size(),
                        this->model->values.stochastic.size(),
                        this->model->values.attack.size(),
                        this->model->values.residual.size()
                    });
                    
                    this->sound_length = this->max_frames * this->analysis.parameters.hop_size;
                    if (this->loop.end == 0) this->loop.end = this->sound_length;
                    
                    // TODO TEST - Temporal override
                    this->loop.end = (this->max_frames - 24) * this->analysis.parameters.hop_size;
                    
                    //                // Original sound synthesis
                    //                this->synthesize();
                    //
                    // Extract Additional Features
                    this->extractFeatures();
                    
                    // Save Original Values
                    //                this->saveOriginalValues();
                    
                    /** Normalize magnitudes on load by default */
                    // TODO - Fix this method
                    //                this->normalizeMagnitudes();
                    
                    /** Updating flags */
                    this->had_file_loaded = true;
                    
                    // TODO - Test
                    this->loaded = true;
                    
                    /* TODO? - Refreshing the UI (waveform visualization) */
                }
                else
                {
                    // TODO - Raise and catch exception
                    std::cout << "Error while loading the sound\n";
                    jassertfalse;
                    
                    this->reset();
                }
                
            }
//            catch (std::exception& e)
            catch (const char* msg)
            {
//                std::cout << "Error while loading the sound: '" << e.what() << "'\n";
                std::cout << "Error while loading the sound: '" << msg << "'\n";
//                jassertfalse;
            }
        }
        else
        {
            std::cout << "Error while loading the sound\n";
            jassertfalse;
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
        if ( i_num_frame < this->model->values.harmonic.freqs.size())
        {
            sound_frame.harmonic.freqs = getComponentFrame(Frame::Component::HarmonicFreqs, i_num_frame);
            sound_frame.harmonic.mags = getComponentFrame(Frame::Component::HarmonicMags, i_num_frame);
//            sound_frame.harmonic.phases = getComponentFrame(Frame::Component::HarmonicPhases, i_num_frame);
            sound_frame.harmonic.phases = std::vector<float>(0);
        }
        else
        {
            sound_frame.harmonic.freqs = std::vector<float>(0);
            sound_frame.harmonic.mags = std::vector<float>(0);
            sound_frame.harmonic.phases = std::vector<float>(0);
        }
        
        if ( i_num_frame < this->model->values.sinusoidal.freqs.size())
        {
            sound_frame.sinusoidal.freqs = getComponentFrame(Frame::Component::SinusoidalFreqs, i_num_frame);
            sound_frame.sinusoidal.mags = getComponentFrame(Frame::Component::SinusoidalMags, i_num_frame);
            //            sound_frame.sinusoidal.phases = getComponentFrame(Frame::Component::sinusoidalPhases, i_num_frame);
            sound_frame.sinusoidal.phases = std::vector<float>(0);
        }
        else
        {
            sound_frame.sinusoidal.freqs = std::vector<float>(0);
            sound_frame.sinusoidal.mags = std::vector<float>(0);
            sound_frame.sinusoidal.phases = std::vector<float>(0);
        }
        
        if ( i_num_frame < this->model->values.stochastic.size())
        {
            sound_frame.stochastic = getComponentFrame(Frame::Component::Stochastic, i_num_frame);
        }
        
        if ( (i_num_frame * i_frame_length) < this->model->values.attack.size())
        {
            sound_frame.attack = getComponentFrame(Frame::Component::Attack, i_num_frame, i_frame_length);
        }
        else
        {
            sound_frame.attack = std::vector<float>(0);
        }
        
        if ( (i_num_frame * i_frame_length) < this->model->values.residual.size())
        {
            sound_frame.residual = getComponentFrame(Frame::Component::Residual, i_num_frame, i_frame_length);
        }
        else
        {
            sound_frame.residual = std::vector<float>(0);
        }
        
        return sound_frame;
    }

    std::vector<float> Sound::getComponentFrame(Frame::Component component_name, int i_num_frame, int i_frame_length)
    {
        // Output
        std::vector<float> component_frame;

        switch (component_name)
        {
            case Frame::Component::HarmonicFreqs:
                component_frame = this->model->values.harmonic.freqs[i_num_frame];
                break;
            case Frame::Component::HarmonicMags:
                component_frame = this->model->values.harmonic.mags[i_num_frame];
                break;
            case Frame::Component::HarmonicPhases:
                component_frame = this->model->values.harmonic.phases[i_num_frame];
                break;
            case Frame::Component::SinusoidalFreqs:
                component_frame = this->model->values.sinusoidal.freqs[i_num_frame];
                break;
            case Frame::Component::SinusoidalMags:
                component_frame = this->model->values.sinusoidal.mags[i_num_frame];
                break;
            case Frame::Component::SinusoidalPhases:
                component_frame = this->model->values.sinusoidal.phases[i_num_frame];
                break;
            case Frame::Component::Stochastic:
                component_frame = this->model->values.stochastic[i_num_frame];
                break;
            case Frame::Component::Attack:
            case Frame::Component::Residual:
            {
                std::vector<float> vector_component = this->model->values.attack;
                
                if (component_name == Frame::Component::Residual)
                {
                    vector_component = this->model->values.residual;
                }

                int i_start_sample = i_num_frame * i_frame_length;
                int i_end_sample = i_start_sample + i_frame_length;
                int i_vec_size = (int)vector_component.size();

                if (i_vec_size < i_end_sample)
                {
                    i_end_sample = i_vec_size;
                }

                if (i_vec_size < i_start_sample)
                {
                    i_start_sample = i_vec_size;
                }

                std::vector<float> residual_frame = Tools::Get::valuesInRange(vector_component, i_start_sample, i_end_sample);

                for (int i = 0; i < residual_frame.size(); i++)
                {
//                    component_frame[i] = residual_frame[i];
                    component_frame.push_back( residual_frame[i] );
                }

                break;
            }
//            {
//                std::vector<std::vector<float>> matrix_component = this->model->values.harmonics_freqs;
                
//                switch (component_name)
//                {
//                    case Frame::Component::HarmonicsFreqs:
//                        component_frame = this->model->values.harmonics_freqs[i_num_frame];
//                        break;
//                    case Frame::Component::HarmonicsMags:
//                        component_frame = this->model->values.harmonics_mags[i_num_frame];
//                }
                
                
//                std::vector<std::vector<float>> matrix_component;
//
//                switch (component_name)
//                {
//                    case Frame::Component::HarmonicsFreqs:
//                        matrix_component = this->model->values.harmonics_freqs;
//                        break;
//                    case Frame::Component::HarmonicsMags:
//                        matrix_component = this->model->values.harmonics_mags;
//                        break;
//                    case Frame::Component::HarmonicsPhases:
//                        matrix_component = this->model->values.harmonics_phases;
//                        break;
//                    case Frame::Component::Sinusoidal:
//                        matrix_component = this->model->values.sinusoidal;
//                        break;
//                    case Frame::Component::Stochastic:
//                        matrix_component = this->model->values.stochastic;
//                        break;
//                    case Frame::Component::Residual:
//                        break;
//                }
                
//                component_frame = std::vector<float>(0);
                
//                if ( i_num_frame < matrix_component.size() )
//                {
//                    for(int i = 0; i < matrix_component[i_num_frame].size(); i++)
//                    {
////                        component_frame[i] = matrix_component[i_num_frame][i];
//                        component_frame.push_back( matrix_component[i_num_frame][i] );
//                    }
//                }
//                else
//                {
//                    std::vector<float> empty_frame(DEFAULT_HZ, this->max_harmonics);
//
//                    if (component_name == Frame::Component::HarmonicsMags)
//                    {
//                        std::fill(empty_frame.begin(), empty_frame.end(), DEFAULT_DB);
//                    }
//
//                    component_frame = empty_frame;
//                }
//                break;
//            }
//            case Frame::Component::Residual:
//            {
//                std::vector<float> vector_component = this->model->values.residual;
//
//                int i_start_sample = i_num_frame * i_frame_length;
//                int i_end_sample = i_start_sample + i_frame_length;
//                int i_vec_size = (int)vector_component.size();
//
//                if (i_vec_size < i_end_sample)
//                {
//                    i_end_sample = i_vec_size;
//                }
//
//                if (i_vec_size < i_start_sample)
//                {
//                    i_start_sample = i_vec_size;
//                }
//
//                std::vector<float> residual_frame = Tools::Get::valuesInRange(vector_component, i_start_sample, i_end_sample);
//
//                for (int i = 0; i < residual_frame.size(); i++)
//                {
////                    component_frame[i] = residual_frame[i];
//                    component_frame.push_back( residual_frame[i] );
//                }
//
//                break;
//            }
        }
            
        return component_frame;
    }
    
    void Sound::synthesize()
    {
    //    // TODO - Generate window dynamically
    //    std::vector<double> window;
    //    this->synthesis.parameters.window = generateSynthesisWindow();
//
//        // Initialize the synthesis engine
//        this->synthesis.engine = std::make_unique<SynthesisEngine>();
//
//        // Synthesize Harmonics
//        this->synthesis.harmonic = this->synthesis.engine->sineModelSynth(this->model->values.harmonics_freqs,
//                                                                          this->model->values.harmonics_mags,
//                                                                          this->model->values.harmonics_phases,
//                                                                          this->analysis.parameters.synthesis_fft_size,
//                                                                          this->analysis.parameters.hop_size,
//                                                                          this->synthesis.engine->window,
//                                                                          this->file.fs);
//
//        // TODO - Synthesize Stochastic Component
//        this->synthesis.stochastic = this->synthesis.engine->stochasticModelSynth(this->model->values.stochastic,
//                                                                                  this->analysis.parameters.synthesis_fft_size,
//                                                                                  this->analysis.parameters.hop_size);
//
    //    // Sum the harmonic and stochastic components together
    //    this->synthesis.x = yh[:min(yh.size, yst.size)]+yst[:min(yh.size, yst.size)]
    }

    void Sound::extractFeatures()
    {
        // Extract the fundamentals
        this->getFundamentalNotes();
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
            float a = A4 * powf(2, (float) i_octave);
            
            notes.emplace( (a * powf(2, -9.0/12) ),   0 ); // "C"
            notes.emplace( (a * powf(2, -8.0/12) ),   0 ); // "C#"
            notes.emplace( (a * powf(2, -7.0/12) ),   0 ); // "D"
            notes.emplace( (a * powf(2, -6.0/12) ),   0 ); // "D#"
            notes.emplace( (a * powf(2, -5.0/12) ),   0 ); // "E"
            notes.emplace( (a * powf(2, -4.0/12) ),   0 ); // "F"
            notes.emplace( (a * powf(2, -3.0/12) ),   0 ); // "F#"
            notes.emplace( (a * powf(2, -2.0/12) ),   0 ); // "G"
            notes.emplace( (a * powf(2, -1.0/12) ),   0 ); // "G#"
            notes.emplace( (a),                       0 ); // "A"
            notes.emplace( (a * powf(2, 1.0/12) ),    0 ); // "A#"
            notes.emplace( (a * powf(2, 2.0/12) ),    0 ); // "B"
        }
        
        float notes_array[notes.size()];
        
        int i_note = 0;
        
        for( const auto& [frequency, number_of_occurrences] : notes )
        {
            notes_array[i_note] = frequency;
            
            i_note++;
        }

        // Count the number of occurrences for each fundamental present on "harmonic_frequencies"
        for (int i_frame = 0; i_frame < this->model->values.harmonic.freqs.size(); i_frame++)
        {
            if (this->model->values.harmonic.freqs[i_frame].size() > 0)
            {
                // Current fundamental
                float current_fundamental = this->model->values.harmonic.freqs[i_frame][0];
                
                // Find the closest note for this fundamental in Hz
                float i_closest_note = findClosest(notes_array, (int) notes.size(), current_fundamental);
                
                // Incrementing the number of occurrences
                notes.find(i_closest_note)->second++;
            }
        }
        
        // Save the result on features.fundamentals
        this->features.notes = notes;
        
        // Find the predominant fundamental
        typedef SoundFeatures::NotesMap::iterator iter;
        iter it = notes.begin();
        iter end = notes.end();
        
        float max_frequency = it->first;
        int max_occurences = it->second;
        for( ; it != end; ++it) {
            if(it->second > max_occurences) {
                max_frequency = it->first;
                max_occurences = it->second;
            }
        }
        
        // Save the predominant note
        this->features.predominant_note = max_frequency;
    }

    void Sound::saveOriginalValues()
    {
        this->original.harmonic.freqs       = this->model->values.harmonic.freqs;
        this->original.harmonic.mags        = this->model->values.harmonic.mags;
        this->original.harmonic.phases      = this->model->values.harmonic.phases;
        this->original.sinusoidal.freqs     = this->model->values.sinusoidal.freqs;
        this->original.sinusoidal.mags      = this->model->values.sinusoidal.mags;
        this->original.sinusoidal.phases    = this->model->values.sinusoidal.phases;
        this->original.attack               = this->model->values.attack;
        this->original.stochastic           = this->model->values.stochastic;
        this->original.residual             = this->model->values.residual;
    }

    void Sound::normalizeMagnitudes()
    {
        jassertfalse;
        DBG("This normalization function must be updated, now it is possible that we do not have an harmonic component");
        
        // Define normalization range db
        float min_db = -100.0;
        float max_db = -1.0;
        
        // Initialize min and max values
        float min_val = 0.0;
        float max_val = -100.0;
        
        // Get the harmonic magnitudes matrix
        std::vector<std::vector<float>> harmonic_mags = this->model->values.harmonic.mags;
        
        // Find the min and max of the harmonic magnitudes matrix
        for (int i=0; i<harmonic_mags.size(); i++)
        {
            if (harmonic_mags[i].size() > 0)
            {
                float local_min_val = *min_element(harmonic_mags[i].begin(), harmonic_mags[i].end());
                float local_max_val = *max_element(harmonic_mags[i].begin(), harmonic_mags[i].end());
                
                if (local_min_val < min_val) min_val = local_min_val;
                if (local_max_val > max_val) max_val = local_max_val;
            }
        }
        
        // Normalize the harmonic magnitudes
        for (int i=0; i<harmonic_mags.size(); i++)
        {
            for (int j=0; j<harmonic_mags[i].size(); j++)
            {
                harmonic_mags[i][j] = (max_db - min_db) * ( (harmonic_mags[i][j] - min_val) / (max_val - min_val) ) + min_db;
            }
        }
        
        // Set the harmonic magnitudes matrix
        this->model->setHarmonicMagnitudes( harmonic_mags );
        
        // Calcualte the normalization factor applied over the harmonics magnitudes
        this->features.normalization_factor = Tools::Calculate::dbToLinear( -std::abs(max_val - max_db) );
        
        if (this->model->hasSinusoidal())
        {
            // Get the sinusoidal magnitudes matrix
            std::vector<std::vector<float>> sinusoidal_mags = this->model->values.sinusoidal.mags;
            
            // Normalize the sinusoidal magnitudes
            for (int i=0; i<sinusoidal_mags.size(); i++)
            {
                for (int j=0; j<sinusoidal_mags[i].size(); j++)
                {
                    sinusoidal_mags[i][j] = (max_db - min_db) * ( (sinusoidal_mags[i][j] - min_val) / (max_val - min_val) ) + min_db;
                }
            }
            
            // Set the sinusoidal magnitudes matrix
            this->model->setSinusoidalMagnitudes( sinusoidal_mags );
        }
        
        // Attack component normalization
        if (this->model->hasAttack())
        {
            // Get the residual component
            std::vector<float> attack = this->model->values.attack;
            
            // Normalize attack component
            this->normalizeWaveform (attack, max_val, max_db);
            
            // Set the normalized attack component
            this->model->setAttack (attack);
        }
        
        // Residual component normalization
        if (this->model->hasResidual())
        {
            // Get the residual component
            std::vector<float> residual = this->model->values.residual;
            
            // Normalize residual component
            this->normalizeWaveform (residual, max_val, max_db);
            
            // Set the normalized residual component
            this->model->setResidual (residual);
        }
    }
    
    void Sound::normalizeWaveform(std::vector<float>& waveform, float max_val, float max_db)
    {
        // Define normalization range linear
        float max_val_from_db = Tools::Calculate::dbToLinear( max_val );
        float max_linear = Tools::Calculate::dbToLinear(max_db);
        float min_linear = -max_linear;
        
        // Initialize min and max values
        float min_val_linear = 0.0;
        float max_val_linear = 0.0;
        
        // Find the max value of the waveform
        for (int i = 0; i < waveform.size(); i++)
        {
            float current_val = std::abs( waveform[i] );
            if (current_val > max_val_linear) max_val_linear = current_val;
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
            //            waveform[i] /= waveform_normalization_factor;
            //            waveform[i] /= this->features.normalization_factor;
            //            waveform[i] = (max_linear - min_linear) * ( (waveform[i] - min_val_linear) / (max_val_linear - min_val_linear) ) + min_linear;
            waveform[i] = (max_linear - min_linear) * ( (waveform[i] - min_val_linear) / (max_val_linear - min_val_linear) ) + min_linear;
        }
        
        std::vector<float> results_great;
        copy_if(waveform.begin(), waveform.end(), back_inserter(results_great),[](float n ){ return  n > 1.0;});
        
        std::vector<float> results_less;
        copy_if(waveform.begin(), waveform.end(), back_inserter(results_less),[](float n ){ return  n < -1.0;});
        
        jassert (results_great.size() == 0); // Upper clipping
        jassert (results_less.size() == 0); // Lower clipping
    }
} // namespace Core
