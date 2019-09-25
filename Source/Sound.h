/*
  ==============================================================================

    SMTSound.h
    Created: 19 Jul 2019 6:00:01pm
    Author:  Marc Sanchez Martinez

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"

#include "SynthesisEngine.h"
//#include "SMTWaveformPreview.h"

#include <vector>
#include <numeric>

#define ADD_MAPPAIR(_first,_second) \
    struct Pair { \
    std::pair<const key_type,mapped_type> & _p; \
    Pair(std::pair<const key_type,mapped_type> & _a) : _p(_a){}; \
    key_type _first () {return _p.first;}; \
    mapped_type & _second () {return _p.second;}; \
}

#define ADD_ITER(_first,_second) \
    struct Iterator : public iterator { \
    Iterator() {}; \
    Iterator(const iterator & _a) : iterator(_a){}; \
    key_type _first() { return (*this)->first;}; \
    mapped_type & _second() { return (*this)->second;}; \
}

//#include "SMTUtils.h"
//#include "SMTConstants.h"

//#include "SMTAudioHelpers.h"

//#include <unordered_map>
//#include <list>
//#include <boost/filesystem.hpp>

enum class HadFileSource
{
    Path = 0,   // The ".had" file will be loaded from a filepath
    Binary,     // The ".had" file will be loaded from a binary data object
};

enum WindowType
{
    blackmanharris = 0,
    triang,
    hanning
};

class Sound
{
public:
    
    /** Sound parameters specified by the user */
    std::string path;
    std::string name;
    std::string extension;
    std::string dirpath;
    
    /** Sound parameters of the actual ".had" file */
    struct SoundFile {
        std::string name;
        std::string extension;
        std::string path;
        std::string dirpath;
        int fs; // Frame size (sample rate)
    };
    SoundFile file;
    
    /** Analysis parameters readen from the ".had" file */
    struct Analysis {
        struct AnalysisParameters {
            std::vector<double> window;
            WindowType window_type;
            int window_size;
            int fft_size;
            int magnitude_threshold;
            double min_sine_dur;
            int max_harm;
            int min_f0;
            int max_f0;
            int max_f0_error;
            double harm_dev_slope;
            double stoc_fact;
            int synthesis_fft_size;
            int hop_size;
        };
        AnalysisParameters parameters;
    };
    Analysis analysis;
    
    /** Original Sound Synthesized */
    struct Synthesis {
        std::unique_ptr<SynthesisEngine> engine;
        std::vector<float> harmonic;
        std::vector<float> stochastic;
        std::vector<float> x;
//        std::unique_ptr<SMTWaveformPreview> asdf;
//        SMTWaveformPreview* waveform_preview;
//        struct SynthesisParameters {
//            // TODO MAKE DYNAMIC WINDOW
//            //            std::vector<double> ow;
//            //            std::vector<double> bh;
//            std::vector<float> window;
//        };
//        SynthesisParameters parameters;
    };
    Synthesis synthesis;
    
    /** Sound Features */
    struct SoundFeatures {
        struct NotesMap : public std::map<float, int, std::less<float>>
        {
            ADD_ITER(iterFrequency, iterOccurences);
            ADD_MAPPAIR(frequency, occurences);
        };
        NotesMap notes;
        float predominant_note;
    };
    SoundFeatures features;
    
    /** Original Sound Values */
    struct OriginalValues {
        /** Analysis output readen from the ".had" file */
        std::vector<std::vector<float>> harmonic_frequencies;
        std::vector<std::vector<float>> harmonic_magnitudes;
        std::vector<std::vector<float>> harmonic_phases;
        std::vector<std::vector<float>> stochastic_residual;
    };
    OriginalValues original;
    
    /** Analysis output readen from the ".had" file */
    std::vector<std::vector<float>> harmonic_frequencies;
    std::vector<std::vector<float>> harmonic_magnitudes;
    std::vector<std::vector<float>> harmonic_phases;
    std::vector<std::vector<float>> stochastic_residual;
    
    /** Control flags */
    bool loaded;
    
    Sound();
    Sound(std::string filepath);
    Sound(String file_data, std::string filepath);
    ~Sound();
    
    String loadDataFromFile(std::string file_path);
    
    void load(String file_data, HadFileSource file_source = HadFileSource::Path);
    void synthesize();
    
    void extractFeatures();
    void getFundamentalNotes();
    
    void saveOriginalValues();
    void normalizeMagnitudes();
    
private:
    
//    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Sound);
};

typedef std::array<std::unique_ptr<Sound>, 3> SoundArray;
