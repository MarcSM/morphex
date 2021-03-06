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

#include "JuceHeader.h"

#include "Model.h"

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

namespace Core { class Sound; }

class Core::Sound
{
public:
    
    enum HadFileSource
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
    
    // Control flags
    bool loaded;
    bool analyzed;
    bool had_file_loaded;

    int fs;
    int note;
    int velocity;
    int max_harmonics;
    int max_frames;
    int sound_length;

    struct SoundLoop {
        int start;
        int end;
    };
    SoundLoop loop;
    
    Model* model;
    
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
    struct SoundAnalysis
    {
        struct SoundAnalysisParameters
        {
            std::vector<double> window;
            WindowType window_type;
            int window_size;
            int fft_size;
            int magnitude_threshold;
            int hearing_threshold;
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
        SoundAnalysisParameters parameters;
    };
    SoundAnalysis analysis;
    
    // Original sound synthesized
    struct SoundSynthesis
    {
        std::vector<float> harmonic;
        std::vector<float> stochastic;
        std::vector<float> x;
    };
    SoundSynthesis synthesis;
    
    /** Sound Features */
    struct SoundFeatures
    {
        struct NotesMap : public std::map<float, int, std::less<float>>
        {
            ADD_ITER (iterFrequency, iterOccurences);
            ADD_MAPPAIR (frequency, occurences);
        };
        NotesMap notes;
        float predominant_note;
        float normalization_factor = 1.0;
    };
    SoundFeatures features;
    
    // Original sound values
    Model::Values original;
    
    // TODO - Improve the Frame structure/class and maybe embedded it in Sound->Model
    class Frame
    {
    public:
        
        enum Component
        {
            HarmonicFreqs = 0,
            HarmonicMags,
            HarmonicPhases,
            SinusoidalFreqs,
            SinusoidalMags,
            SinusoidalPhases,
            Stochastic,
            Attack,
            Residual
        };
        
        struct SoundFrameFMP
        {
            std::vector<float> freqs;
            std::vector<float> mags;
            std::vector<float> phases;
        };
        
        SoundFrameFMP harmonic;
        SoundFrameFMP sinusoidal;
        std::vector<float> stochastic;
        std::vector<float> attack;
        std::vector<float> residual;
        
        int getMaxHarmonics()
        {
            int max_harmonics = (int) std::max
            ({
                this->harmonic.freqs.size(),
                this->harmonic.mags.size(),
                this->harmonic.phases.size()
            });
            
            return max_harmonics;
        };
        
        int getMaxSinusoids()
        {
            int max_sinusoids = (int) std::max
            ({
                this->sinusoidal.freqs.size(),
                this->sinusoidal.mags.size(),
                this->sinusoidal.phases.size()
            });
            
            return max_sinusoids;
        };
        
        bool hasHarmonic() { return this->harmonic.freqs.size() > 0; };
        bool hasSinusoidal() { return this->sinusoidal.freqs.size() > 0; };
        bool hasPhases (SoundFrameFMP sound_frame_fmp) { return sound_frame_fmp.phases.size() > 0; };
        bool hasStochastic() { return this->stochastic.size() > 0; };
        bool hasAttack() { return this->attack.size() > 0; };
        bool hasResidual() { return this->residual.size() > 0; };
    };
    
    Sound();
    Sound (std::string file_path);
    Sound (std::string file_path, int note, int velocity = 1);
    Sound (String file_data, std::string filepath);
    ~Sound();
    
    void init (bool init_model = true);
    void reset();
    
    String loadDataFromFile (std::string file_path);
    void load (String file_data, HadFileSource file_source = HadFileSource::Path);
    
    Sound::Frame getFrame (int i_num_frame, int i_hop_size);
    std::vector<float> getComponentFrame (Frame::Component component_name, int i_num_frame, int i_hop_size = 0);
    
    void synthesize();
    
    void extractFeatures();
    void getFundamentalNotes();
    
    void saveOriginalValues();
    void normalizeMagnitudes();
    void normalizeWaveform (std::vector<float>& waveform, float max_val, float max_db);

private:
    
    int file_version;
    int decimal_places;
};
