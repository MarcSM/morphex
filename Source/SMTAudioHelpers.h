/*
 ==============================================================================
 
 SMTAudioHelpers.h
 Created: 3 Jun 2019 10:54:55am
 Author:  Marc Sanchez Martinez
 
 ==============================================================================
 */

#pragma once

#include "SMTUtils.h"
#include "SMTConstants.h"
#include "SMTHelperFunctions.h"

enum class FrameInterpolationMode
{
    Harmonics = 0,  // Interpolate Harmonics
    Magnitudes,     // Interpolate Magnitudes
    Stochastic      // Interpolate Stochastic components
};

/**
 * Interpolate the frames (frequencies or magnitudes) for the given harmonics.
 *
 * This sum is the arithmetic sum, not some other kind of sum that only
 * mathematicians have heard of.
 *
 * @param frame_1 list of doubles - frame of sound 1
 * @param frame_2 list of doubles - frame of sound 2
 * @param harmonics list of ints - harmonics to interpolate
 * @param interp_factor double - interpolation factor
 * @return interpolated_frame list of doubles - interpolated frame
 */
//inline std::vector<float> interpolateFrames(std::vector<float> frame_1,
//                                            std::vector<float> frame_2,
//                                            std::vector<int> harmonics,
//                                            float interp_factor,
//                                            FrameInterpolationMode interpolation_mode)
//{
//    // Default initialization
//    std::vector<float> interpolated_frame(frame_1.size(), 0.0);
//    
//    // Harmonics Initialization
//    if (interpolation_mode == FrameInterpolationMode::Harmonics)
//    {
//        // Fill with 0.0
//        std::fill(interpolated_frame.begin(),interpolated_frame.end(), 0.0);
//    }
//    // Magnitudes Initialization
//    else if (interpolation_mode == FrameInterpolationMode::Magnitudes)
//    {
//        // Fill with -100.0
//        std::fill(interpolated_frame.begin(),interpolated_frame.end(), -100.0);
//    }
//    
//    int i_harmonic_to_interpolate;
//    
//    for (int i = 0; i < harmonics.size(); i++)
//    {
//        i_harmonic_to_interpolate = harmonics[i];
//        interpolated_frame[i_harmonic_to_interpolate] = (1-interp_factor) * frame_1[i_harmonic_to_interpolate] + interp_factor * frame_2[i_harmonic_to_interpolate];
//    }
//
//    return interpolated_frame;
//}

inline void genspecsines_C(float *iploc, float *ipmag, float *ipphase, int n_peaks, float *real, float*imag, int size_spec)
{
    int ii=0,jj=0, ploc_int;
    int size_spec_half = (int)floor(size_spec/2);
    float bin_remainder,loc,mag;
    
    for(ii=0;ii<n_peaks; ii++)
    {
        loc = iploc[ii];
        bin_remainder = floor(loc + 0.5)-loc;
        ploc_int = (int)floor(loc+0.5);
        
        if((loc>=5)&&(loc<size_spec_half-4))
        {
            mag = pow(10,(ipmag[ii]/20.0));
            
            for(jj=-4;jj<5;jj++)
            {
                real[ploc_int+jj] += mag*BH_92_1001[(int)((bin_remainder+jj)*100) + BH_SIZE_BY2]*cos(ipphase[ii]);
                imag[ploc_int+jj] += mag*BH_92_1001[(int)((bin_remainder+jj)*100) + BH_SIZE_BY2]*sin(ipphase[ii]);
            }
        }
        else if((loc>0)&&(loc<5))
        {
            mag = pow(10,(ipmag[ii]/20.0));
            
            for(jj=-4;jj<5;jj++)
            {
                
                if(ploc_int+jj<0)
                {
                    real[-1*(ploc_int+jj)] += mag*BH_92_1001[(int)((bin_remainder+jj)*100) + BH_SIZE_BY2]*cos(ipphase[ii]);
                    imag[-1*(ploc_int+jj)] += -1*mag*BH_92_1001[(int)((bin_remainder+jj)*100) + BH_SIZE_BY2]*sin(ipphase[ii]);
                    
                }
                else if (ploc_int+jj==0)
                {
                    real[(ploc_int+jj)] += 2*mag*BH_92_1001[(int)((bin_remainder+jj)*100) + BH_SIZE_BY2]*cos(ipphase[ii]);
                }
                else
                {
                    real[(ploc_int+jj)] += mag*BH_92_1001[(int)((bin_remainder+jj)*100) + BH_SIZE_BY2]*cos(ipphase[ii]);
                    imag[ploc_int+jj] += mag*BH_92_1001[(int)((bin_remainder+jj)*100) + BH_SIZE_BY2]*sin(ipphase[ii]);
                }
            }
        }
        else if((loc>=size_spec_half-4)&&(loc<size_spec_half-1))
        {
            mag = pow(10,(ipmag[ii]/20.0));
            
            for(jj=-4;jj<5;jj++)
            {
                if(ploc_int+jj>size_spec_half)
                {
                    real[size_spec-(ploc_int+jj)] += mag*BH_92_1001[(int)((bin_remainder+jj)*100) + BH_SIZE_BY2]*cos(ipphase[ii]);
                    imag[size_spec-(ploc_int+jj)] += -1*mag*BH_92_1001[(int)((bin_remainder+jj)*100) + BH_SIZE_BY2]*sin(ipphase[ii]);
                    
                }
                else if(ploc_int+jj==size_spec_half)
                {
                    real[(ploc_int+jj)] += 2*mag*BH_92_1001[(int)((bin_remainder+jj)*100) + BH_SIZE_BY2]*cos(ipphase[ii]);
                    
                }
                else
                {
                    real[(ploc_int+jj)] += mag*BH_92_1001[(int)((bin_remainder+jj)*100) + BH_SIZE_BY2]*cos(ipphase[ii]);
                    imag[ploc_int+jj] += mag*BH_92_1001[(int)((bin_remainder+jj)*100) + BH_SIZE_BY2]*sin(ipphase[ii]);
                }
            }
        }
    }
    
    for(ii=1;ii<size_spec_half;ii++)
    {
        real[size_spec_half+ii]=real[size_spec_half-ii];
        imag[size_spec_half+ii]=-1*imag[size_spec_half-ii];
    }
}

inline std::vector<std::complex<float>> genSpecSines(std::vector<float> iploc, std::vector<float> ipmag, std::vector<float> ipphase, int N, int fs)
{
    for (int i=0; i<iploc.size(); i++) iploc[i] = N * iploc[i] / (float)fs;
    
    std::vector<float> real(N);
    std::vector<float> imag(N);

    genspecsines_C(iploc.data(), ipmag.data(), ipphase.data(), (int)iploc.size(), real.data(), imag.data(), N);

    std::vector<std::complex<float>> result(N);
    
    std::transform( real.begin(), real.end(), imag.begin(), result.begin(), []( float dreal, float dimag )
    {
        return std::complex<float>( dreal, dimag );
    });

    return result;
}

/** FFT shift for double data */
inline void fftShift(float* data, int data_size)
{
    // even number of elements
    if (data_size % 2)
        std::rotate(&data[0], &data[data_size >> 1], &data[data_size]);
    // even number of elements
    else
        std::rotate(&data[0], &data[(data_size >> 1) + 1], &data[data_size]);
}

/**
 * Generate the main lobe of a sinc function (Dirichlet kernel)
 *
 * @param x list of ints - array of indexes to compute;
 * @param N int - size of FFT to simulate
 * @return y list of floats - samples of the main lobe of a sinc function
 */
inline std::vector<float> sinc(std::vector<float> x, int N)
{
    // Initialize y
    std::vector<float> y(x.size(), N);
    
    for (int i=0; i<x.size(); i++) y[i] = std::sin(N*x[i]/2) / std::sin(x[i]/2);
    
    return y;
}

/**
 * Generate the main lobe of a Blackman-Harris window
 *
 * @param x list of ints - bin positions to compute (real values)
 * @return y list of floats - main lobe os spectrum of a Blackman-Harris window
 */
inline std::vector<float> genBhLobe(std::vector<float> x)
{
    // Size of fft to use
    int N = 512;
    
    // Initialize f
    std::vector<float> f(x.size(), 0.0);
    
    // Frequency sampling
    for (int i=0; i<x.size(); i++) f[i] = x[i]*M_PI*2/N;

    float df = 2*M_PI/N;
    
    // Initialize y
    std::vector<float> y(x.size(), 0.0);
    
    // Window constants
    std::vector<float> consts = {0.35875, 0.48829, 0.14128, 0.01168};
    
    std::vector<int> m_range = getRange(0, 3);
    
    // Iterate over the four sincs to sum
    for (int m : m_range)
    {
        //            y += consts[m]/2 * (sinc(f-df*m, N) + sinc(f+df*m, N))
        std::vector<float> aux_1(f.size(), 0.0);
        for (int i_aux=0; i_aux<aux_1.size(); i_aux++) aux_1[i_aux] = f[i_aux]-df*m;
        
        std::vector<float> aux_2(f.size(), 0.0);
        for (int i_aux=0; i_aux<aux_2.size(); i_aux++) aux_2[i_aux] = f[i_aux]+df*m;
        
        std::vector<float> aux_1_sinc = sinc(aux_1, N);
        std::vector<float> aux_2_sinc = sinc(aux_2, N);
        
        for (int i=0; i<y.size(); i++)
        {
            // Sum of scaled sinc functions
            y[i] += consts[m]/2 * (aux_1_sinc[i] + aux_2_sinc[i]);
        }
    }
    
    // Normalize
    for (int i=0; i<y.size(); i++) y[i] = y[i] / N / consts[0];
    
    return y;
}

/**
 * Generate a spectrum from a series of sine values (python implementation)
 *
 * @param iploc list of floats - sine peaks locations
 * @param ipmag list of floats - magnitudes
 * @param ipphase list of floats - phases
 * @param N int - size of the complex spectrum to generate
 * @param fs int - sampling rate
 * @return y list of complex - generated complex spectrum of sines
 */
inline std::vector<std::complex<float>> genSpecSines_p(std::vector<float> ipfreq,
                                                       std::vector<float> ipmag,
                                                       std::vector<float> ipphase,
                                                       int N, int fs)
{
    // Initialize output complex spectrum
    std::vector<std::complex<float>> Y(N);
    
    // Size of positive freq. spectrum
    int hN = int(N/2);
    
    // Generate all sine spectral lobes
    for (int i=0; i<ipfreq.size(); i++)
    {
        float loc = N * ipfreq[i] / fs;
        
        if (loc==0 or loc>hN-1) continue;
        
        float binremainder = std::round(loc)-loc;
        
        // Main lobe (real value) bins to read
        std::vector<float> lb = getRange(binremainder-4, binremainder+4);
        std::vector<float> lmag = genBhLobe(lb);
        
        for (int i_lmag=0; i_lmag<lmag.size(); i_lmag++) lmag[i_lmag] = lmag[i_lmag] * powf(10, ipmag[i]/20.0);
        
        std::vector<int> b = getRange(int(round(loc)-4), int(round(loc)+4));
        
        for (int m=0; m<9; m++)
        {
            // Peak lobe crosses DC bin
            if (b[m] < 0)
            {
                std::complex<float> aux_comp = std::complex<float>(0, -1) * std::complex<float>(ipphase[i]);
                std::complex<float> aux_exp_comp = std::exp<float>(aux_comp);
                
                Y[-b[m]] += lmag[m] * aux_exp_comp;
            }
            // Peak lobe croses Nyquist bin
            else if (b[m] > hN)
            {
                std::complex<float> aux_comp = std::complex<float>(0, -1) * std::complex<float>(ipphase[i]);
                std::complex<float> aux_exp_comp = std::exp<float>(aux_comp);
                
                Y[b[m]] += lmag[m] * aux_exp_comp;
            }
            // Peak lobe in the limits of the spectrum
            else if (b[m] == 0 or b[m] == hN)
            {
                std::complex<float> aux_comp_1 = std::complex<float>(0, 1) * std::complex<float>(ipphase[i]);
                std::complex<float> aux_exp_comp_1 = std::exp<float>(aux_comp_1);
                
                std::complex<float> aux_comp_2 = std::complex<float>(0, -1) * std::complex<float>(ipphase[i]);
                std::complex<float> aux_exp_comp_2 = std::exp<float>(aux_comp_2);
                
                Y[b[m]] += lmag[m] * aux_exp_comp_1 + lmag[m] * aux_exp_comp_2;
            }
            // Peak lobe in positive freq. range
            else
            {
                std::complex<float> aux_comp = std::complex<float>(0, 1) * std::complex<float>(ipphase[i]);
                std::complex<float> aux_exp_comp = std::exp<float>(aux_comp);
            
                Y[b[m]] += lmag[m] * aux_exp_comp;
            }
        }
        
        int indexes_len = int( Y.size() - (hN+1) );
        
        // Fill the negative part of the spectrum
        for (int i_aux=0; i_aux<indexes_len; i_aux++)
        {
            Y[hN+1+i_aux] = conj(Y[hN-1-i_aux]);
        }
    }
    
    return Y;
}

inline float* generateSine(float frequency = 440.0, float level = 0.5f, float samples_to_render = 512, int sample_rate = 44100, float currentAngle = 0.0)
{
    float cycles_per_sample = frequency / sample_rate;
    float angle_delta = cycles_per_sample * 2.0 * MathConstants<float>::pi;
    
    float* out_audio = new float[samples_to_render];
    
    for (auto sample = 0; sample < samples_to_render; ++sample)
    {
        auto currentSample = (float) std::sin (currentAngle);
        currentAngle += angle_delta;
        out_audio[sample]  = currentSample * level;
    }
    
    return out_audio;
}

inline std::vector<std::complex<float>> genStocComponent(std::vector<float> stocs_morph, int H, int NS)
{
    // Interpolate to original size
    std::vector<float> mY = stocs_morph;
    
    // Generate phase random values
    int hN = int(NS/2+1); // positive size of fft
    
    std::vector<float> pY_aux(hN);
    std::generate(pY_aux.begin(), pY_aux.end(), RandomGenerator(0.0, 1.0));
    
    for (int i=0; i<pY_aux.size(); i++) pY_aux[i] = pY_aux[i] * 2 * M_PI;
    
    std::vector<std::complex<float>> pY(hN);
    for (int i=0; i<pY.size(); i++) pY[i] = std::complex<float>(pY_aux[i]);
    
    // Initialize synthesis spectrum
    std::vector<std::complex<float>> Y(NS, 0.0);
    
    for (int i=0; i<hN; i++)
    {
        // Generate positive freq
        std::complex<float> aux_comp = std::complex<float>(0, -1) * std::complex<float>(pY[i]);
        std::complex<float> aux_exp_comp = std::exp<float>(aux_comp);
        
        pY[i] = std::powf(10,mY[i]/20.0) * aux_exp_comp;
    }
    
    for (int i=hN; i<Y.size(); i++)
    {
        
        // Generate negative freq
        std::complex<float> aux_comp = std::complex<float>(0, -1) * std::complex<float>(pY[i]);
        std::complex<float> aux_exp_comp = std::exp<float>(aux_comp);
        
        pY[i] = std::powf(10,mY[i]/20.0) * aux_exp_comp;
    }
    
    return pY;
}
