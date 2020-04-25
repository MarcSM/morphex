/*
  ==============================================================================

    Sound.cpp
    Created: 19 Jul 2019 6:00:01pm
    Author:  Marc Sanchez Martinez

  ==============================================================================
*/

#include "Sound.h"

#include "SMTHelperFunctions.h"

#include "SMTAudioHelpers.h"

namespace Core
{
void Sound::commonInit()
{
    // Initialize default values
    this->loaded = false;
    this->analyzed = false;
    this->had_file_loaded = false;
    
    // Initializing default values
    //    this->x = []
    this->fs = 44100;
    this->note = NULL;
    this->velocity = NULL;
    this->max_harmonics = 0;
    this->max_frames = 0;
    this->loop.start = 0;
    this->loop.end = 0;
    
    // Model object
    this->model = new Model();
    
    //    // Initializing the harmonic analysis data structure for the .had file
    //    this->had = {}
}

Sound::Sound()
{
    this->commonInit();
};

// Read the file from a file_path
Sound::Sound(std::string file_path)
{
    this->commonInit();
    
    // Load data from file_path
    String file_data = loadDataFromFile(file_path);
    
    // Load the sound
    this->load(file_data, HadFileSource::Binary);
};

Sound::Sound(std::string file_path, int note, int velocity)
{
    this->commonInit();
    
    this->note = note;
    this->velocity = velocity;
    
    // Load data from file_path
    String file_data = loadDataFromFile(file_path);
    
    // Load the sound
    this->load(file_data, HadFileSource::Binary);
};

// Read the file from a binary file with an hypothetical file_path
Sound::Sound(String file_data, std::string file_path)
{
    // Save the file_path
    this->path = file_path;
    
    // Load the sound
    this->load(file_data, HadFileSource::Binary);
};

Sound::~Sound(){};

// Load data of ".had" file
String Sound::loadDataFromFile(std::string file_path)
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

void Sound::load(String file_data, HadFileSource file_source)
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
            // Sound
            XmlElement *xml_sound = xml->getChildByName("sound"); // had["sound"]
            this->fs = xml_sound->getChildByName("fs")->getAllSubText().getIntValue();
            this->note = xml_sound->getChildByName("note")->getAllSubText().getIntValue();
            this->velocity = xml_sound->getChildByName("velocity")->getAllSubText().getIntValue();
            this->max_harmonics = xml_sound->getChildByName("max_harmonics")->getAllSubText().getIntValue();
            this->max_frames = xml_sound->getChildByName("max_frames")->getAllSubText().getIntValue();
            this->loop.start = xml_sound->getChildByName("loop")->getChildByName("start")->getAllSubText().getIntValue();
            this->loop.end = xml_sound->getChildByName("loop")->getChildByName("end")->getAllSubText().getIntValue();
            
            // Model
            XmlElement *xml_synthesis = xml->getChildByName("synthesis"); // had["synthesis"]
            this->model->harmonic   = hasChild(xml_synthesis, "f") and hasChild(xml_synthesis, "m");
            this->model->phases     = hasChild(xml_synthesis, "p");
            this->model->sinusoidal = hasChild(xml_synthesis, "l");
            this->model->stochastic = hasChild(xml_synthesis, "s");
            this->model->residual   = hasChild(xml_synthesis, "r");
            if (this->model->harmonic)      this->model->setHarmonics(  getMatrixOfFloats(xml_synthesis, "f"), getMatrixOfFloats(xml_synthesis, "m") );
            if (this->model->phases)        this->model->setPhases(     getMatrixOfFloats(xml_synthesis, "p") );
            if (this->model->sinusoidal)    this->model->setSinusoidal( getMatrixOfFloats(xml_synthesis, "l") );
            if (this->model->stochastic)    this->model->setStochastic( getMatrixOfFloats(xml_synthesis, "s") );
            if (this->model->residual)      this->model->setResidual(   getMatrixOfFloats(xml_synthesis, "r") );

//            // Decode the values from the ".had" files
//            self.decodeHadValues(had, self.model)
            
//            // File
//            XmlElement *xml_sound_file = xml_sound->getChildByName("file"); // had["sound"]["file"]
//            this->file.name = xml_sound_file->getChildByName("name")->getAllSubText().toStdString();
//            this->file.extension = xml_sound_file->getChildByName("extension")->getAllSubText().toStdString();
//            this->file.path = xml_sound_file->getChildByName("path")->getAllSubText().toStdString();
//            this->file.dirpath = xml_sound_file->getChildByName("dirpath")->getAllSubText().toStdString();
            
            /** Binary Data Cases */
//            if (this->path.empty()) this->path = "";
            if (this->name.empty()) this->name = this->file.name;
            if (this->extension.empty()) this->extension = ".had";
//            if (this->dirpath.empty()) this->dirpath = "";

            /** Parameters */
            XmlElement *xml_parameters = xml->getChildByName("parameters"); // had["parameters"]
//            this->analysis.parameters.window = getVectorOfDoubles(xml_parameters, "window");
            this->analysis.parameters.window_type = (WindowType)xml_parameters->getChildByName("window_type")->getAllSubText().getIntValue(); // TODO - Cast properly
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
            
//            /** Analysis Output Values */
//            XmlElement *xml_analysis = xml->getChildByName("output")->getChildByName("values"); // had["output"]
//            this->harmonic_frequencies = getMatrixOfFloats(xml_analysis, "harmonic_frequencies");
//            this->harmonic_magnitudes = getMatrixOfFloats(xml_analysis, "harmonic_magnitudes");
//            this->harmonic_phases = getMatrixOfFloats(xml_analysis, "harmonic_phases");
//            this->stochastic_residual = getMatrixOfFloats(xml_analysis, "stochastic_residual");
            
            /** Original Sound Synthesized */
            this->synthesize();
            
            /** Extract Additional Features */
            this->extractFeatures();

            /** Save Original Values */
            this->saveOriginalValues();
            
            /** Normalize magnitudes on load by default */
            this->normalizeMagnitudes();
            
            /** Updating flags */
            this->had_file_loaded = true;
            
            /* TODO? - Refreshing the UI (waveform visualization) */
        }
        catch (std::exception& e)
        {
            std::cout << "Error while loading the sound: '" << e.what() << "'\n";
        }
    }
    else
    {
        std::cout << "Error while loading the sound\n";
    }
};







void Sound::synthesize()
{
//    // TODO - Generate window dynamically
//    std::vector<double> window;
//    this->synthesis.parameters.window = generateSynthesisWindow();
    
    // Initialize the synthesis engine
    this->synthesis.engine = std::make_unique<SynthesisEngine>();
    
    // Synthesize Harmonics
    this->synthesis.harmonic = this->synthesis.engine->sineModelSynth(this->model->values.harmonics_freqs,
                                                                      this->model->values.harmonics_mags,
                                                                      this->model->values.harmonics_phases,
                                                                      this->analysis.parameters.synthesis_fft_size,
                                                                      this->analysis.parameters.hop_size,
                                                                      this->synthesis.engine->window,
                                                                      this->file.fs);
    
    // TODO - Synthesize Stochastic Component
    this->synthesis.stochastic = this->synthesis.engine->stochasticModelSynth(this->model->values.stochastic,
                                                                              this->analysis.parameters.synthesis_fft_size,
                                                                              this->analysis.parameters.hop_size);
    
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
    for (int i_frame = 0; i_frame < this->model->values.harmonics_freqs.size(); i_frame++)
    {
        // Current fundamental
        float current_fundamental = this->model->values.harmonics_freqs[i_frame][0];
        
        // Find the closest note for this fundamental in Hz
        float i_closest_note = findClosest(notes_array, (int) notes.size(), current_fundamental);
        
        // Incrementing the number of occurrences
        notes.find(i_closest_note)->second++;
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
    this->original.harmonics_freqs  = this->model->values.harmonics_freqs;
    this->original.harmonics_mags   = this->model->values.harmonics_mags;
    this->original.harmonics_phases = this->model->values.harmonics_phases;
    this->original.stochastic       = this->model->values.stochastic;
}

void Sound::normalizeMagnitudes()
{
    // Initialize min and max db
    float min_db = -100.0;
    float max_db = 0.0;
    
    // Initialize min and max values
    float min_val = 0.0;
    float max_val = -100.0;
    
    // Find the min and max of the harmonic magnitudes matrix
    for (int i=0; i<this->model->values.harmonics_mags.size(); i++)
    {
        float local_min_val = *min_element(this->model->values.harmonics_mags[i].begin(), this->model->values.harmonics_mags[i].end());
        float local_max_val = *max_element(this->model->values.harmonics_mags[i].begin(), this->model->values.harmonics_mags[i].end());
        
        if (local_min_val < min_val) min_val = local_min_val;
        if (local_max_val > max_val) max_val = local_max_val;
    }
    
    // Normalize the harmonic magnitudes
    for (int i=0; i<this->model->values.harmonics_mags.size(); i++)
    {
        for (int j=0; j<this->model->values.harmonics_mags[i].size(); j++)
        {
            this->model->values.harmonics_mags[i][j] = (max_db - min_db) * ( (this->model->values.harmonics_mags[i][j] - min_val) / (max_val - min_val) ) + min_db;
        }
    }
}
    
} // namespace Core
