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
 *
 * The function "genspecsines_C" comes from the repository https://github.com/MTG/sms-tools by Xavier Serra.
 */

#include "synthesis.h"

namespace Constants
{
constexpr auto BhSize = 1001;
constexpr auto BhSizeBy2 = 501;
constexpr float Bh_92_1001[BhSize] = { 3.1942e-07, 1.1341e-08, 3.2373e-07, 6.8548e-07, 1.0735e-06, 1.4875e-06, 1.9268e-06, 2.3909e-06, 2.8791e-06, 3.3905e-06, 3.9244e-06, 4.48e-06, 5.0561e-06, 5.6514e-06, 6.265e-06, 6.8957e-06, 7.5422e-06, 8.2026e-06, 8.876e-06, 9.5606e-06, 1.0255e-05, 1.0956e-05, 1.1664e-05, 1.2376e-05, 1.309e-05, 1.3804e-05, 1.4516e-05, 1.5224e-05, 1.5927e-05, 1.662e-05, 1.7303e-05, 1.7973e-05, 1.8628e-05, 1.9265e-05, 1.9881e-05, 2.0475e-05, 2.1045e-05, 2.1586e-05, 2.2098e-05, 2.2578e-05, 2.3022e-05, 2.343e-05, 2.3798e-05, 2.4124e-05, 2.4406e-05, 2.4642e-05, 2.4829e-05, 2.4966e-05, 2.5051e-05, 2.5081e-05, 2.5055e-05, 2.4971e-05, 2.4828e-05, 2.4625e-05, 2.436e-05, 2.4032e-05, 2.3642e-05, 2.3187e-05, 2.2668e-05, 2.2085e-05, 2.1437e-05, 2.0726e-05, 1.9952e-05, 1.9116e-05, 1.822e-05, 1.7264e-05, 1.6253e-05, 1.5186e-05, 1.4069e-05, 1.2904e-05, 1.1694e-05, 1.0445e-05, 9.1607e-06, 7.8463e-06, 6.5078e-06, 5.1516e-06, 3.7846e-06, 2.4143e-06, 1.0491e-06, 3.0242e-07, 1.6305e-06, 2.925e-06, 4.1749e-06, 5.3683e-06, 6.4929e-06, 7.5353e-06, 8.481e-06, 9.3154e-06, 1.0022e-05, 1.0584e-05, 1.0984e-05, 1.1202e-05, 1.1219e-05, 1.1013e-05, 1.0564e-05, 9.8468e-06, 8.8379e-06, 7.5115e-06, 5.8412e-06, 3.7985e-06, 1.3548e-06, 1.5209e-06, 4.8605e-06, 8.6968e-06, 1.3065e-05, 1.8002e-05, 2.3544e-05, 2.973e-05, 3.6602e-05, 4.4202e-05, 5.2573e-05, 6.1761e-05, 7.1816e-05, 8.2779e-05, 9.471e-05, 0.00010766, 0.00012167, 0.00013682, 0.00015315, 0.00017072, 0.0001896, 0.00020986, 0.00023155, 0.00025474, 0.00027951, 0.00030592, 0.00033406, 0.00036401, 0.00039582, 0.00042959, 0.0004654, 0.00050335, 0.0005435, 0.00058595, 0.00063081, 0.00067816, 0.00072811, 0.00078076, 0.0008362, 0.00089452, 0.00095588, 0.0010203, 0.001088, 0.0011591, 0.0012336, 0.0013117, 0.0013935, 0.0014791, 0.0015687, 0.0016625, 0.0017604, 0.0018628, 0.0019696, 0.0020811, 0.0021974, 0.0023187, 0.0024451, 0.0025767, 0.0027137, 0.0028564, 0.0030048, 0.0031592, 0.0033195, 0.003486, 0.0036593, 0.0038388, 0.0040255, 0.004219, 0.0044196, 0.0046273, 0.0048431, 0.0050664, 0.0052977, 0.005537, 0.0057848, 0.0060412, 0.0063063, 0.0065804, 0.0068638, 0.0071565, 0.0074591, 0.0077713, 0.0080938, 0.0084265, 0.00877, 0.0091241, 0.0094893, 0.0098661, 0.010254, 0.010654, 0.011066, 0.01149, 0.011927, 0.012377, 0.01284, 0.013315, 0.013805, 0.014309, 0.014827, 0.015359, 0.015906, 0.016468, 0.017045, 0.017637, 0.018246, 0.01887, 0.019511, 0.020169, 0.020844, 0.021536, 0.022245, 0.022973, 0.023718, 0.024482, 0.025264, 0.026066, 0.026887, 0.027728, 0.028589, 0.02947, 0.030371, 0.031294, 0.032237, 0.033201, 0.03419, 0.035198, 0.03623, 0.037285, 0.03836, 0.039461, 0.040587, 0.041734, 0.042906, 0.044103, 0.045324, 0.046571, 0.047845, 0.049141, 0.050468, 0.051817, 0.053194, 0.054598, 0.056031, 0.057491, 0.058976, 0.060492, 0.062036, 0.063608, 0.065211, 0.066842, 0.068501, 0.070191, 0.071912, 0.073664, 0.075443, 0.077257, 0.079099, 0.080975, 0.082882, 0.08482, 0.086792, 0.088794, 0.090832, 0.0929, 0.095002, 0.097135, 0.099305, 0.10151, 0.10375, 0.10602, 0.10832, 0.11066, 0.11304, 0.11545, 0.11789, 0.12037, 0.12289, 0.12544, 0.12803, 0.13065, 0.13331, 0.13601, 0.13874, 0.14151, 0.14431, 0.14715, 0.15003, 0.15295, 0.1559, 0.15889, 0.16192, 0.16498, 0.16808, 0.17122, 0.1744, 0.17761, 0.18086, 0.18415, 0.18747, 0.19083, 0.19424, 0.19767, 0.20115, 0.20466, 0.20821, 0.21179, 0.21542, 0.21907, 0.22277, 0.22651, 0.23028, 0.23408, 0.23793, 0.24181, 0.24572, 0.24967, 0.25366, 0.25769, 0.26175, 0.26584, 0.26996, 0.27413, 0.27833, 0.28256, 0.28683, 0.29113, 0.29546, 0.29983, 0.30423, 0.30866, 0.31313, 0.31762, 0.32215, 0.32671, 0.3313, 0.33592, 0.34057, 0.34525, 0.34996, 0.35471, 0.35948, 0.36426, 0.36909, 0.37393, 0.37883, 0.38373, 0.38866, 0.39362, 0.39858, 0.4036, 0.40863, 0.41368, 0.41873, 0.42385, 0.42897, 0.43411, 0.43926, 0.44444, 0.44962, 0.45486, 0.46006, 0.46534, 0.47061, 0.47588, 0.48118, 0.48648, 0.49181, 0.49715, 0.50251, 0.50788, 0.51324, 0.51863, 0.52403, 0.52942, 0.53485, 0.54028, 0.5457, 0.55113, 0.55655, 0.56201, 0.56744, 0.57289, 0.57835, 0.58381, 0.58923, 0.59469, 0.60015, 0.60557, 0.61103, 0.61646, 0.62191, 0.62734, 0.63277, 0.63816, 0.64359, 0.64898, 0.65438, 0.65974, 0.66511, 0.67047, 0.6758, 0.68114, 0.68644, 0.69174, 0.69701, 0.70225, 0.70749, 0.71273, 0.71794, 0.72312, 0.72826, 0.73338, 0.7385, 0.74358, 0.74864, 0.75366, 0.75868, 0.76364, 0.7686, 0.7735, 0.7784, 0.78324, 0.78804, 0.79285, 0.79759, 0.80231, 0.80696, 0.81161, 0.8162, 0.82076, 0.82528, 0.82978, 0.83421, 0.83861, 0.84296, 0.84727, 0.85151, 0.85573, 0.85992, 0.86404, 0.8681, 0.87213, 0.8761, 0.88004, 0.88391, 0.88773, 0.89148, 0.8952, 0.89886, 0.90246, 0.90602, 0.90952, 0.91294, 0.91632, 0.91963, 0.92289, 0.92611, 0.92924, 0.93231, 0.93532, 0.9383, 0.94118, 0.944, 0.94676, 0.94946, 0.9521, 0.95467, 0.95718, 0.9596, 0.96199, 0.96428, 0.96651, 0.96868, 0.97076, 0.97278, 0.97473, 0.97662, 0.97845, 0.98019, 0.98186, 0.98344, 0.98499, 0.98642, 0.98781, 0.98912, 0.99036, 0.99154, 0.99262, 0.99361, 0.99457, 0.99541, 0.99622, 0.99693, 0.99758, 0.99814, 0.99864, 0.99904, 0.99938, 0.99966, 0.99984, 0.99994, 1, 0.99994, 0.99984, 0.99966, 0.99938, 0.99904, 0.99864, 0.99814, 0.99758, 0.99693, 0.99622, 0.99541, 0.99457, 0.99361, 0.99262, 0.99154, 0.99036, 0.98912, 0.98781, 0.98642, 0.98499, 0.98344, 0.98186, 0.98019, 0.97845, 0.97662, 0.97473, 0.97278, 0.97076, 0.96868, 0.96651, 0.96428, 0.96199, 0.9596, 0.95718, 0.95467, 0.9521, 0.94946, 0.94676, 0.944, 0.94118, 0.9383, 0.93532, 0.93231, 0.92924, 0.92611, 0.92289, 0.91963, 0.91632, 0.91294, 0.90952, 0.90602, 0.90246, 0.89886, 0.8952, 0.89148, 0.88773, 0.88391, 0.88004, 0.8761, 0.87213, 0.8681, 0.86404, 0.85992, 0.85573, 0.85151, 0.84727, 0.84296, 0.83861, 0.83421, 0.82978, 0.82528, 0.82076, 0.8162, 0.81161, 0.80696, 0.80231, 0.79759, 0.79285, 0.78804, 0.78324, 0.7784, 0.7735, 0.7686, 0.76364, 0.75868, 0.75366, 0.74864, 0.74358, 0.7385, 0.73338, 0.72826, 0.72312, 0.71794, 0.71273, 0.70749, 0.70225, 0.69701, 0.69174, 0.68644, 0.68114, 0.6758, 0.67047, 0.66511, 0.65974, 0.65438, 0.64898, 0.64359, 0.63816, 0.63277, 0.62734, 0.62191, 0.61646, 0.61103, 0.60557, 0.60015, 0.59469, 0.58923, 0.58381, 0.57835, 0.57289, 0.56744, 0.56201, 0.55655, 0.55113, 0.5457, 0.54028, 0.53485, 0.52942, 0.52403, 0.51863, 0.51324, 0.50788, 0.50251, 0.49715, 0.49181, 0.48648, 0.48118, 0.47588, 0.47061, 0.46534, 0.46006, 0.45486, 0.44962, 0.44444, 0.43926, 0.43411, 0.42897, 0.42385, 0.41873, 0.41368, 0.40863, 0.4036, 0.39858, 0.39362, 0.38866, 0.38373, 0.37883, 0.37393, 0.36909, 0.36426, 0.35948, 0.35471, 0.34996, 0.34525, 0.34057, 0.33592, 0.3313, 0.32671, 0.32215, 0.31762, 0.31313, 0.30866, 0.30423, 0.29983, 0.29546, 0.29113, 0.28683, 0.28256, 0.27833, 0.27413, 0.26996, 0.26584, 0.26175, 0.25769, 0.25366, 0.24967, 0.24572, 0.24181, 0.23793, 0.23408, 0.23028, 0.22651, 0.22277, 0.21907, 0.21542, 0.21179, 0.20821, 0.20466, 0.20115, 0.19767, 0.19424, 0.19083, 0.18747, 0.18415, 0.18086, 0.17761, 0.1744, 0.17122, 0.16808, 0.16498, 0.16192, 0.15889, 0.1559, 0.15295, 0.15003, 0.14715, 0.14431, 0.14151, 0.13874, 0.13601, 0.13331, 0.13065, 0.12803, 0.12544, 0.12289, 0.12037, 0.11789, 0.11545, 0.11304, 0.11066, 0.10832, 0.10602, 0.10375, 0.10151, 0.099305, 0.097135, 0.095002, 0.0929, 0.090832, 0.088794, 0.086792, 0.08482, 0.082882, 0.080975, 0.079099, 0.077257, 0.075443, 0.073664, 0.071912, 0.070191, 0.068501, 0.066842, 0.065211, 0.063608, 0.062036, 0.060492, 0.058976, 0.057491, 0.056031, 0.054598, 0.053194, 0.051817, 0.050468, 0.049141, 0.047845, 0.046571, 0.045324, 0.044103, 0.042906, 0.041734, 0.040587, 0.039461, 0.03836, 0.037285, 0.03623, 0.035198, 0.03419, 0.033201, 0.032237, 0.031294, 0.030371, 0.02947, 0.028589, 0.027728, 0.026887, 0.026066, 0.025264, 0.024482, 0.023718, 0.022973, 0.022245, 0.021536, 0.020844, 0.020169, 0.019511, 0.01887, 0.018246, 0.017637, 0.017045, 0.016468, 0.015906, 0.015359, 0.014827, 0.014309, 0.013805, 0.013315, 0.01284, 0.012377, 0.011927, 0.01149, 0.011066, 0.010654, 0.010254, 0.0098661, 0.0094893, 0.0091241, 0.00877, 0.0084265, 0.0080938, 0.0077713, 0.0074591, 0.0071565, 0.0068638, 0.0065804, 0.0063063, 0.0060412, 0.0057848, 0.005537, 0.0052977, 0.0050664, 0.0048431, 0.0046273, 0.0044196, 0.004219, 0.0040255, 0.0038388, 0.0036593, 0.003486, 0.0033195, 0.0031592, 0.0030048, 0.0028564, 0.0027137, 0.0025767, 0.0024451, 0.0023187, 0.0021974, 0.0020811, 0.0019696, 0.0018628, 0.0017604, 0.0016625, 0.0015687, 0.0014791, 0.0013935, 0.0013117, 0.0012336, 0.0011591, 0.001088, 0.0010203, 0.00095588, 0.00089452, 0.0008362, 0.00078076, 0.00072811, 0.00067816, 0.00063081, 0.00058595, 0.0005435, 0.00050335, 0.0004654, 0.00042959, 0.00039582, 0.00036401, 0.00033406, 0.00030592, 0.00027951, 0.00025474, 0.00023155, 0.00020986, 0.0001896, 0.00017072, 0.00015315, 0.00013682, 0.00012167, 0.00010766, 9.471e-05, 8.2779e-05, 7.1816e-05, 6.1761e-05, 5.2573e-05, 4.4202e-05, 3.6602e-05, 2.973e-05, 2.3544e-05, 1.8002e-05, 1.3065e-05, 8.6968e-06, 4.8605e-06, 1.5209e-06, 1.3548e-06, 3.7985e-06, 5.8412e-06, 7.5115e-06, 8.8379e-06, 9.8468e-06, 1.0564e-05, 1.1013e-05, 1.1219e-05, 1.1202e-05, 1.0984e-05, 1.0584e-05, 1.0022e-05, 9.3154e-06, 8.481e-06, 7.5353e-06, 6.4929e-06, 5.3683e-06, 4.1749e-06, 2.925e-06, 1.6305e-06, 3.0242e-07, 1.0491e-06, 2.4143e-06, 3.7846e-06, 5.1516e-06, 6.5078e-06, 7.8463e-06, 9.1607e-06, 1.0445e-05, 1.1694e-05, 1.2904e-05, 1.4069e-05, 1.5186e-05, 1.6253e-05, 1.7264e-05, 1.822e-05, 1.9116e-05, 1.9952e-05, 2.0726e-05, 2.1437e-05, 2.2085e-05, 2.2668e-05, 2.3187e-05, 2.3642e-05, 2.4032e-05, 2.436e-05, 2.4625e-05, 2.4828e-05, 2.4971e-05, 2.5055e-05, 2.5081e-05, 2.5051e-05, 2.4966e-05, 2.4829e-05, 2.4642e-05, 2.4406e-05, 2.4124e-05, 2.3798e-05, 2.343e-05, 2.3022e-05, 2.2578e-05, 2.2098e-05, 2.1586e-05, 2.1045e-05, 2.0475e-05, 1.9881e-05, 1.9265e-05, 1.8628e-05, 1.7973e-05, 1.7303e-05, 1.662e-05, 1.5927e-05, 1.5224e-05, 1.4516e-05, 1.3804e-05, 1.309e-05, 1.2376e-05, 1.1664e-05, 1.0956e-05, 1.0255e-05, 9.5606e-06, 8.876e-06, 8.2026e-06, 7.5422e-06, 6.8957e-06, 6.265e-06, 5.6514e-06, 5.0561e-06, 4.48e-06, 3.9244e-06, 3.3905e-06, 2.8791e-06, 2.3909e-06, 1.9268e-06, 1.4875e-06, 1.0735e-06, 6.8548e-07, 3.2373e-07 };
} // namespace Constants

namespace Core
{
Synthesis::Synthesis (Instrument& instrument) :
    m_instrument (instrument)
{
    // Default values
    parameters.fft_size = FFT_SIZE;
    parameters.hop_size = HOP_SIZE;
    parameters.fs       = FS;

    // TODO - Change this fixes values with the line below
    int fftOrder = 9; // 9 -> 2^9 = 512
    fft          = new dsp::FFT (fftOrder);

    buffer.length = parameters.fft_size * NUM_FRAMES_IN_BUFFER;

    // Generate synthesis windows
    getWindow();

    reset();
}

Synthesis::~Synthesis() {};

void Synthesis::reset()
{
    live_values.i_current_frame = 0;
    live_values.i_samples_ready = 0;
    live_values.first_frame     = true;
    live_values.last_frame      = false;

    std::vector<float> last_freqs;
    std::vector<float> phases;

    // Harmonic live values
    live_values.harmonic.last_freqs.resize (MAX_HARMONICS);
    live_values.harmonic.phases.resize (MAX_HARMONICS);

    std::fill (live_values.harmonic.last_freqs.begin(),
               live_values.harmonic.last_freqs.end(),
               0.0);

    std::fill (live_values.harmonic.phases.begin(),
               live_values.harmonic.phases.end(),
               0.0);

    // Sinusoidal live values
    live_values.sinusoidal.last_freqs.resize (MAX_SINUSOIDS);
    live_values.sinusoidal.phases.resize (MAX_SINUSOIDS);

    std::fill (live_values.sinusoidal.last_freqs.begin(),
               live_values.sinusoidal.last_freqs.end(),
               0.0);

    std::fill (live_values.sinusoidal.phases.begin(),
               live_values.sinusoidal.phases.end(),
               0.0);

    buffer.pointers.write = 0;
    buffer.pointers.play  = 0;

    // Clean buffer channels
    for (int i = 0; i < Channel::NUM_CHANNELS; i++)
    {
        buffer.channels[i].resize (buffer.length);
        std::fill (buffer.channels[i].begin(),
                   buffer.channels[i].end(),
                   0.0);
    }
}

std::vector<float> Synthesis::synthesizeSoundFrame (Sound::Frame sound_frame)
{
    // Parameters shortcut
    const int FS = parameters.fs;
    const int NS = parameters.fft_size;

    // Output
    std::vector<float> yw (NS, 0.0);

    // Harmonic component
    if (m_instrument.generate.harmonic and sound_frame.hasHarmonic())
    {
        // TODO - TOFIX - This does not work
        std::vector<float> y_harmonic =
            generateSines (sound_frame.harmonic.freqs,
                           sound_frame.harmonic.mags,
                           sound_frame.harmonic.phases,
                           NS,
                           FS);

        // Applying the window and saving the result on the output vector "harmonic"
        std::vector<float> yw_harmonic (NS);
        for (int i = 0; i < NS; i++)
        {
            // TODO - Is this faster in two separated for loops?
            yw_harmonic[i] = y_harmonic[i] * window.harm[i];
            yw[i] += yw_harmonic[i];
        }
    }

    // Sinusoidal component
    if (m_instrument.generate.sinusoidal and sound_frame.hasSinusoidal())
    {
        std::vector<float> y_sinusoidal =
            generateSines (sound_frame.sinusoidal.freqs,
                           sound_frame.sinusoidal.mags,
                           sound_frame.sinusoidal.phases,
                           NS,
                           FS);

        // Applying the window and saving the result on the output vector "harmonic"
        std::vector<float> yw_sinusoidal (NS);
        for (int i = 0; i < NS; i++)
        {
            // TODO - Is this faster in two separated for loops?
            yw_sinusoidal[i] = y_sinusoidal[i] * window.harm[i];
            yw[i] += yw_sinusoidal[i];
        }
    }

    // TODO - Stochastic component
    //        if ( sound_frame.hasStochastic() )
    //        {
    //            std::vector<float> y_stocs = generateStocs(sound_frame.stochastic, H * 2, NS);
    //
    //            // Applying the window and saving the result on the output vector "harmonic"
    //            std::vector<float> yw_stocs(NS);
    //            for (int i = 0; i < NS; i++)
    //            {
    //                // TODO - Is this faster in two separated for loops?
    //                yw_stocs[i] = y_stocs[i] * window.stoc[i];
    //                yw[i] += yw_stocs[i];
    //                //                yw_stocs[i] += y_stocs[i] * window.stoc[i];
    //            }
    //        }

    // Attack compontent
    if (m_instrument.generate.attack and sound_frame.hasAttack())
    {
        for (int i = 0; i < sound_frame.attack.size(); i++)
        {
            yw[i] += sound_frame.attack[i];
        }
    }

    // Residual compontent
    if (m_instrument.generate.residual and sound_frame.hasResidual())
    {
        for (int i = 0; i < sound_frame.residual.size(); i++)
        {
            yw[i] += sound_frame.residual[i];
        }
    }

    return yw;
}

void Synthesis::generateSoundFrame (Sound::Frame sound_frame, bool append_to_generated)
{
    // Get parameters
    const int H             = parameters.hop_size;
    const int MAX_HARMONICS = sound_frame.getMaxHarmonics();
    const int MAX_SINUSOIDS = sound_frame.getMaxSinusoids();

    // A list with the indexes of the harmonics we want to interpolate
    std::vector<int> idx_harmonics = Tools::Generate::range (0, MAX_HARMONICS);
    std::vector<int> idx_sinusoids = Tools::Generate::range (0, MAX_SINUSOIDS);

    // If the sound frame has harmonics but not its phases
    if (sound_frame.hasHarmonic())
    {
        // Get phase values
        if (live_values.first_frame && sound_frame.hasPhases (sound_frame.harmonic))
        {
            live_values.harmonic.phases = sound_frame.harmonic.phases;
        }
        else
        {
            sound_frame.harmonic.phases = Tools::Get::valuesByIndex (live_values.harmonic.phases, idx_harmonics);
        }

        // Save the current frequencies to be available fot the next iteration
        updateLastFreqs (live_values.harmonic, sound_frame.harmonic.freqs, idx_harmonics);

        // Update phases for the next iteration
        updatePhases (live_values.harmonic, sound_frame.harmonic.freqs, idx_harmonics, H);
    }

    // If the sound frame has sinusoids but not its phases
    if (sound_frame.hasSinusoidal())
    {
        // Get phase values
        if (live_values.first_frame && sound_frame.hasPhases (sound_frame.sinusoidal))
        {
            live_values.sinusoidal.phases = sound_frame.sinusoidal.phases;
        }
        else
        {
            sound_frame.sinusoidal.phases = Tools::Get::valuesByIndex (live_values.sinusoidal.phases, idx_sinusoids);
        }

        // Save the current frequencies to be available fot the next iteration
        updateLastFreqs (live_values.sinusoidal, sound_frame.sinusoidal.freqs, idx_sinusoids);

        // Update phases for the next iteration
        updatePhases (live_values.sinusoidal, sound_frame.sinusoidal.freqs, idx_sinusoids, H);
    }

    // Generate windowed audio frame
    std::vector<float> windowed_audio_frame = synthesizeSoundFrame (sound_frame);

    if (live_values.first_frame)
    {
        // Apply fade in
        Tools::Audio::applyFadeIn (windowed_audio_frame);
        live_values.first_frame = false;
    }

    if (live_values.last_frame)
    {
        // Apply fade out
        Tools::Audio::applyFadeOut (windowed_audio_frame);
        live_values.last_frame = false;
    }

    // Add the audio frame to the circular buffer
    updateBuffer (BufferSection::Write, BufferUpdateMode::Add, windowed_audio_frame, Channel::Mono);

    // Update samples ready to be played
    live_values.i_samples_ready += H;

    // Clean the part of the buffer that we must override
    updateBuffer (BufferSection::Clean, BufferUpdateMode::Delete);

    // Update write pointer position
    updateWritePointer (H);
}

void Synthesis::getWindow()
{
    // Get parameters
    const int NS  = parameters.fft_size;
    const int H   = parameters.hop_size;
    const int HNS = int (NS / 2);

    std::vector<float> ow (2 * H);
    dsp::WindowingFunction<float>::fillWindowingTables (ow.data(), ow.size(), dsp::WindowingFunction<float>::triangular, false);

    std::vector<float> bh (NS);
    dsp::WindowingFunction<float>::fillWindowingTables (bh.data(), bh.size(), dsp::WindowingFunction<float>::blackmanHarris, false);
    float bh_sum = std::accumulate (bh.begin(), bh.end(), 0.0);
    Tools::Calculate::divideByScalar (bh, bh_sum);

    // Harmonic window
    window.harm.resize (NS);
    std::fill (window.harm.begin(), window.harm.end(), 0.0);

    int j = 0;
    for (int i = HNS - H; i < HNS + H; i++)
    {
        window.harm[i] = ow[j] / bh[i];
        j++;
    }

    // Stochastic window
    window.stoc.resize (NS);
    dsp::WindowingFunction<float>::fillWindowingTables (window.stoc.data(), window.stoc.size(), dsp::WindowingFunction<float>::blackmanHarris, true);
}

// TODO - Check limits of "i_pointer_position" when it's "i_frame_length"
// if "i_pointer_position" == "buffer.length" modulo may return 0, which may
// not be expected if the the range asked to the buffer is equal to "i_frame_length"
// we will retrieve 0 samples from the buffer instead of "i_frame_length" samples
int Synthesis::getPointerInLimits (int i_pointer_position)
{
    return Tools::Calculate::modulo (i_pointer_position, buffer.length);
}

std::vector<int> Synthesis::getBufferIndexes (int i_head, int i_tail)
{
    // Output
    std::vector<int> buffer_indexes;

    // If the tail pointer is ahead the head pointer
    if ((i_tail - i_head) < 0)
    {
        std::vector<int> first_half  = Tools::Generate::range (i_head, buffer.length);
        std::vector<int> second_half = Tools::Generate::range (0, i_tail);

        // Concatenate "first_half" and "second_half"
        buffer_indexes = first_half;
        buffer_indexes.insert (buffer_indexes.end(), second_half.begin(), second_half.end());
    }
    else
    {
        buffer_indexes = Tools::Generate::range (i_head, i_tail);
    }

    return buffer_indexes;
}

std::vector<int> Synthesis::getBufferSectionIndexes (BufferSection buffer_section, int i_frame_length)
{
    // Parameters shortcut
    const int NS = parameters.fft_size;
    const int H  = parameters.hop_size;

    if (i_frame_length == 0)
        i_frame_length = NS;

    // Output
    std::vector<int> buffer_indexes;

    switch (buffer_section)
    {
        case BufferSection::Write:
            buffer_indexes = getBufferIndexes (buffer.pointers.write,
                                               buffer.pointers.clean (this));
            break;
        case BufferSection::Clean:
            buffer_indexes = getBufferIndexes (buffer.pointers.clean (this),
                                               buffer.pointers.clean (this, H));
            break;
        case BufferSection::Play:
            buffer_indexes = getBufferIndexes (buffer.pointers.play,
                                               getPointerInLimits (buffer.pointers.play + i_frame_length));
            break;
        default:
            // Return an empty vector
            return std::vector<int>();
    }

    return buffer_indexes;
}

std::vector<float> Synthesis::getBuffer (BufferSection buffer_section, Channel selected_channel, int i_frame_length)
{
    // TODO - Check argument: if int i_frame_length == parameters.fft_size

    // Output
    std::vector<float> buffer_section_samples;

    std::vector<int> buffer_indexes = getBufferSectionIndexes (buffer_section, i_frame_length);

    // For each index
    for (int i = 0; i < buffer_indexes.size(); i++)
    {
        buffer_section_samples.push_back (buffer.channels[selected_channel][buffer_indexes[i]]);
    }

    return buffer_section_samples;
}

void Synthesis::updateWritePointer (int i_pointer_increment)
{
    int new_pointer_position = buffer.pointers.write + i_pointer_increment;
    buffer.pointers.write    = getPointerInLimits (new_pointer_position);
}

void Synthesis::updatePlayPointer (int i_pointer_increment)
{
    int new_pointer_position = buffer.pointers.play + i_pointer_increment;
    buffer.pointers.play     = getPointerInLimits (new_pointer_position);
}

void Synthesis::updateBuffer (BufferSection buffer_section, BufferUpdateMode update_mode, std::vector<float> given_frame, Channel selected_channel)
{
    std::vector<int> buffer_indexes = getBufferSectionIndexes (buffer_section);

    // For each index
    for (int i = 0; i < buffer_indexes.size(); i++)
    {
        float new_value = 0.0;
        if (i < given_frame.size())
        {
            new_value = given_frame[i];
        }
        else
        {
            if (update_mode != BufferUpdateMode::Delete)
            {
                DBG ("We should not reach this point");
                //                    jassertfalse;
            }
        }

        switch (update_mode)
        {
            case BufferUpdateMode::Set:
                buffer.channels[selected_channel][buffer_indexes[i]] = new_value;
                break;
            case BufferUpdateMode::Add:
                buffer.channels[selected_channel][buffer_indexes[i]] += new_value;
                break;
            case BufferUpdateMode::Delete:
                buffer.channels[selected_channel][buffer_indexes[i]] = 0.0;
                break;
        }
    }
}

void Synthesis::updatePhases (SynthesisLiveValues::FP& fp_values,
                              std::vector<float>       freqs,
                              std::vector<int>         idx_freqs,
                              int                      hop_size,
                              bool                     append_to_generated)
{
    int idx_freq = 0;

    // For each selected frequency
    for (int i = 0; i < idx_freqs.size(); i++)
    {
        idx_freq = idx_freqs[i];

        // Update phase value
        fp_values.phases[idx_freq] +=
            (M_PI * (fp_values.last_freqs[idx_freq] + freqs[idx_freq]) / parameters.fs) * hop_size;

        // Keep phase inside 2 * pi
        fp_values.phases[idx_freq] = std::fmod (fp_values.phases[idx_freq], (2.0 * M_PI));
    }
}

void Synthesis::wrapPhases (std::vector<float>& phases)
{
    // For each phase
    for (int i = 0; i < phases.size(); i++)
    {
        // Keep phase inside 2 * pi
        phases[i] = std::fmod (phases[i], (2.0 * M_PI));
        //            if (phases[i] < 0.0) phases[i] += 2.0 * M_PI;
    }
}

// TODO - Evaluate if "idx_harmonics" will be necessary in the near future
void Synthesis::updateLastFreqs (SynthesisLiveValues::FP& fp_values, std::vector<float> freqs, std::vector<int> idx_freqs)
{
    int idx_freq = 0;

    // For each selected harmonic
    for (int i = 0; i < idx_freqs.size(); i++)
    {
        idx_freq = idx_freqs[i];

        // Update last freq value
        fp_values.last_freqs[idx_freq] = freqs[idx_freq];
    }
}

std::vector<float> Synthesis::generateSines (std::vector<float> iploc, std::vector<float> ipmag, std::vector<float> ipphase, int NS, int fs)
{
    for (int i = 0; i < iploc.size(); i++)
        iploc[i] = NS * iploc[i] / (float) fs;

    std::vector<float> real (NS);
    std::vector<float> imag (NS);

    genspecsines_C (iploc.data(), ipmag.data(), ipphase.data(), (int) iploc.size(), real.data(), imag.data(), NS);

    std::vector<std::complex<float>> generated_sines (NS);

    std::transform (real.begin(), real.end(), imag.begin(), generated_sines.begin(), [] (float dreal, float dimag) {
        return std::complex<float> (dreal, dimag);
    });

    // Perform the IFFT
    dsp::Complex<float>* Y_harmonics = new dsp::Complex<float>[NS];
    fft->perform (generated_sines.data(), Y_harmonics, true);

    // Get the real part, we don't get the second half because it's empty
    std::vector<float> y_harmonics (NS);
    for (int i = 0; i < NS; i++)
        y_harmonics[i] = Y_harmonics[i].real();

    // TODO - Avoid using deletes
    delete[] Y_harmonics;

    // Perform an FFT shift
    Tools::Audio::fftShift (y_harmonics);

    return y_harmonics;
}

std::vector<float> Synthesis::generateStocs (std::vector<float> stocs_morph, int H, int NS)
{
    // Interpolate to original size
    std::vector<float> mY = stocs_morph;

    // Generate phase random values
    int hN = int (NS / 2 + 1); // positive size of fft

    std::vector<float> pY_aux (hN);
    std::generate (pY_aux.begin(), pY_aux.end(), Tools::Generate::random (0.0, 1.0));

    for (int i = 0; i < pY_aux.size(); i++)
        pY_aux[i] = pY_aux[i] * 2 * M_PI;

    std::vector<std::complex<float>> pY (hN);
    for (int i = 0; i < pY.size(); i++)
        pY[i] = std::complex<float> (pY_aux[i]);

    // Initialize synthesis spectrum
    std::vector<std::complex<float>> Y (NS, 0.0);

    for (int i = 0; i < hN; i++)
    {
        // Generate positive freq
        std::complex<float> aux_comp     = std::complex<float> (0, -1) * std::complex<float> (pY[i]);
        std::complex<float> aux_exp_comp = std::exp<float> (aux_comp);

        pY[i] = std::powf (10, mY[i] / 20.0) * aux_exp_comp;
    }

    for (int i = hN; i < Y.size(); i++)
    {
        // Generate negative freq
        std::complex<float> aux_comp     = std::complex<float> (0, -1) * std::complex<float> (pY[i]);
        std::complex<float> aux_exp_comp = std::exp<float> (aux_comp);

        pY[i] = std::powf (10, mY[i] / 20.0) * aux_exp_comp;
    }

    // Perform the IFFT
    dsp::Complex<float>* Y_stocs = new dsp::Complex<float>[NS];
    fft->perform (pY.data(), Y_stocs, true);

    // Get the real part, we don't get the second half because it's empty
    std::vector<float> y_stocs (NS);
    for (int i = 0; i < NS; i++)
        y_stocs[i] = Y_stocs[i].real();

    // TODO - Avoid using deletes
    delete[] Y_stocs;

    // Perform an FFT shift
    Tools::Audio::fftShift (y_stocs);

    return y_stocs;
}

std::vector<std::complex<float>> Synthesis::genSpecSines (std::vector<float> iploc, std::vector<float> ipmag, std::vector<float> ipphase, int N, int fs)
{
    for (int i = 0; i < iploc.size(); i++)
        iploc[i] = N * iploc[i] / (float) fs;

    std::vector<float> real (N);
    std::vector<float> imag (N);

    genspecsines_C (iploc.data(), ipmag.data(), ipphase.data(), (int) iploc.size(), real.data(), imag.data(), N);

    std::vector<std::complex<float>> result (N);

    std::transform (real.begin(), real.end(), imag.begin(), result.begin(), [] (float dreal, float dimag) {
        return std::complex<float> (dreal, dimag);
    });

    return result;
}

void Synthesis::genspecsines_C (float* iploc, float* ipmag, float* ipphase, int n_peaks, float* real, float* imag, int size_spec)
{
    int   ii = 0, jj = 0, ploc_int;
    int   size_spec_half = (int) floor (size_spec / 2);
    float bin_remainder, loc, mag;

    for (ii = 0; ii < n_peaks; ii++)
    {
        loc           = iploc[ii];
        bin_remainder = floor (loc + 0.5) - loc;
        ploc_int      = (int) floor (loc + 0.5);

        if ((loc >= 5) && (loc < size_spec_half - 4))
        {
            mag = pow (10, (ipmag[ii] / 20.0));

            for (jj = -4; jj < 5; jj++)
            {
                real[ploc_int + jj] += mag * Constants::Bh_92_1001[(int) ((bin_remainder + jj) * 100) + Constants::BhSizeBy2] * cos (ipphase[ii]);
                imag[ploc_int + jj] += mag * Constants::Bh_92_1001[(int) ((bin_remainder + jj) * 100) + Constants::BhSizeBy2] * sin (ipphase[ii]);
            }
        }
        else if ((loc > 0) && (loc < 5))
        {
            mag = pow (10, (ipmag[ii] / 20.0));

            for (jj = -4; jj < 5; jj++)
            {
                if (ploc_int + jj < 0)
                {
                    real[-1 * (ploc_int + jj)] += mag * Constants::Bh_92_1001[(int) ((bin_remainder + jj) * 100) + Constants::BhSizeBy2] * cos (ipphase[ii]);
                    imag[-1 * (ploc_int + jj)] += -1 * mag * Constants::Bh_92_1001[(int) ((bin_remainder + jj) * 100) + Constants::BhSizeBy2] * sin (ipphase[ii]);
                }
                else if (ploc_int + jj == 0)
                {
                    real[(ploc_int + jj)] += 2 * mag * Constants::Bh_92_1001[(int) ((bin_remainder + jj) * 100) + Constants::BhSizeBy2] * cos (ipphase[ii]);
                }
                else
                {
                    real[(ploc_int + jj)] += mag * Constants::Bh_92_1001[(int) ((bin_remainder + jj) * 100) + Constants::BhSizeBy2] * cos (ipphase[ii]);
                    imag[ploc_int + jj] += mag * Constants::Bh_92_1001[(int) ((bin_remainder + jj) * 100) + Constants::BhSizeBy2] * sin (ipphase[ii]);
                }
            }
        }
        else if ((loc >= size_spec_half - 4) && (loc < size_spec_half - 1))
        {
            mag = pow (10, (ipmag[ii] / 20.0));

            for (jj = -4; jj < 5; jj++)
            {
                if (ploc_int + jj > size_spec_half)
                {
                    // TODO - Is this faster in two separated for loops?
                    real[size_spec - (ploc_int + jj)] += mag * Constants::Bh_92_1001[(int) ((bin_remainder + jj) * 100) + Constants::BhSizeBy2] * cos (ipphase[ii]);
                    imag[size_spec - (ploc_int + jj)] += -1 * mag * Constants::Bh_92_1001[(int) ((bin_remainder + jj) * 100) + Constants::BhSizeBy2] * sin (ipphase[ii]);
                }
                else if (ploc_int + jj == size_spec_half)
                {
                    real[(ploc_int + jj)] += 2 * mag * Constants::Bh_92_1001[(int) ((bin_remainder + jj) * 100) + Constants::BhSizeBy2] * cos (ipphase[ii]);
                }
                else
                {
                    real[(ploc_int + jj)] += mag * Constants::Bh_92_1001[(int) ((bin_remainder + jj) * 100) + Constants::BhSizeBy2] * cos (ipphase[ii]);
                    imag[ploc_int + jj] += mag * Constants::Bh_92_1001[(int) ((bin_remainder + jj) * 100) + Constants::BhSizeBy2] * sin (ipphase[ii]);
                }
            }
        }
    }

    for (ii = 1; ii < size_spec_half; ii++)
    {
        real[size_spec_half + ii] = real[size_spec_half - ii];
        imag[size_spec_half + ii] = -1 * imag[size_spec_half - ii];
    }
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
std::vector<std::complex<float>> Synthesis::genSpecSines_p (std::vector<float> ipfreq, std::vector<float> ipmag, std::vector<float> ipphase, int N, int fs)
{
    // Initialize output complex spectrum
    std::vector<std::complex<float>> Y (N);

    // Size of positive freq. spectrum
    int hN = int (N / 2);

    // Generate all sine spectral lobes
    for (int i = 0; i < ipfreq.size(); i++)
    {
        float loc = N * ipfreq[i] / fs;

        if (loc == 0 or loc > hN - 1)
            continue;

        float binremainder = std::round (loc) - loc;

        // Main lobe (real value) bins to read
        std::vector<float> lb   = getRange (binremainder - 4, binremainder + 4);
        std::vector<float> lmag = genBhLobe (lb);

        for (int i_lmag = 0; i_lmag < lmag.size(); i_lmag++)
            lmag[i_lmag] = lmag[i_lmag] * powf (10, ipmag[i] / 20.0);

        std::vector<int> b = getRange (int (round (loc) - 4), int (round (loc) + 4));

        for (int m = 0; m < 9; m++)
        {
            // Peak lobe crosses DC bin
            if (b[m] < 0)
            {
                std::complex<float> aux_comp     = std::complex<float> (0, -1) * std::complex<float> (ipphase[i]);
                std::complex<float> aux_exp_comp = std::exp<float> (aux_comp);

                Y[-b[m]] += lmag[m] * aux_exp_comp;
            }
            // Peak lobe croses Nyquist bin
            else if (b[m] > hN)
            {
                std::complex<float> aux_comp     = std::complex<float> (0, -1) * std::complex<float> (ipphase[i]);
                std::complex<float> aux_exp_comp = std::exp<float> (aux_comp);

                Y[b[m]] += lmag[m] * aux_exp_comp;
            }
            // Peak lobe in the limits of the spectrum
            else if (b[m] == 0 or b[m] == hN)
            {
                std::complex<float> aux_comp_1     = std::complex<float> (0, 1) * std::complex<float> (ipphase[i]);
                std::complex<float> aux_exp_comp_1 = std::exp<float> (aux_comp_1);

                std::complex<float> aux_comp_2     = std::complex<float> (0, -1) * std::complex<float> (ipphase[i]);
                std::complex<float> aux_exp_comp_2 = std::exp<float> (aux_comp_2);

                Y[b[m]] += lmag[m] * aux_exp_comp_1 + lmag[m] * aux_exp_comp_2;
            }
            // Peak lobe in positive freq. range
            else
            {
                std::complex<float> aux_comp     = std::complex<float> (0, 1) * std::complex<float> (ipphase[i]);
                std::complex<float> aux_exp_comp = std::exp<float> (aux_comp);

                Y[b[m]] += lmag[m] * aux_exp_comp;
            }
        }

        int indexes_len = int (Y.size() - (hN + 1));

        // Fill the negative part of the spectrum
        for (int i_aux = 0; i_aux < indexes_len; i_aux++)
        {
            Y[hN + 1 + i_aux] = conj (Y[hN - 1 - i_aux]);
        }
    }

    return Y;
}

float* Synthesis::generateSine (float frequency, float level, float samples_to_render, int sample_rate, float currentAngle)
{
    float cycles_per_sample = frequency / sample_rate;
    float angle_delta       = cycles_per_sample * 2.0 * MathConstants<float>::pi;

    float* out_audio = new float[samples_to_render];

    for (auto sample = 0; sample < samples_to_render; ++sample)
    {
        auto currentSample = (float) std::sin (currentAngle);
        currentAngle += angle_delta;
        out_audio[sample] = currentSample * level;
    }

    return out_audio;
}

/**
 * Generate the main lobe of a Blackman-Harris window
 *
 * @param x list of ints - bin positions to compute (real values)
 * @return y list of floats - main lobe os spectrum of a Blackman-Harris window
 */
std::vector<float> Synthesis::genBhLobe (std::vector<float> x)
{
    // Size of fft to use
    int N = 512;

    // Initialize f
    std::vector<float> f (x.size(), 0.0);

    // Frequency sampling
    for (int i = 0; i < x.size(); i++)
        f[i] = x[i] * M_PI * 2 / N;

    float df = 2 * M_PI / N;

    // Initialize y
    std::vector<float> y (x.size(), 0.0);

    // Window constants
    std::vector<float> consts = { 0.35875, 0.48829, 0.14128, 0.01168 };

    std::vector<int> m_range = getRange (0, 3);

    // Iterate over the four sincs to sum
    for (int m : m_range)
    {
        //            y += consts[m]/2 * (sinc(f-df*m, N) + sinc(f+df*m, N))
        std::vector<float> aux_1 (f.size(), 0.0);
        for (int i_aux = 0; i_aux < aux_1.size(); i_aux++)
            aux_1[i_aux] = f[i_aux] - df * m;

        std::vector<float> aux_2 (f.size(), 0.0);
        for (int i_aux = 0; i_aux < aux_2.size(); i_aux++)
            aux_2[i_aux] = f[i_aux] + df * m;

        std::vector<float> aux_1_sinc = sinc (aux_1, N);
        std::vector<float> aux_2_sinc = sinc (aux_2, N);

        for (int i = 0; i < y.size(); i++)
        {
            // Sum of scaled sinc functions
            y[i] += consts[m] / 2 * (aux_1_sinc[i] + aux_2_sinc[i]);
        }
    }

    // Normalize
    for (int i = 0; i < y.size(); i++)
        y[i] = y[i] / N / consts[0];

    return y;
}

/** FFT shift for double data */
void Synthesis::fftShift (float* data, int data_size)
{
    // even number of elements
    if (data_size % 2)
        std::rotate (&data[0], &data[data_size >> 1], &data[data_size]);
    // even number of elements
    else
        std::rotate (&data[0], &data[(data_size >> 1) + 1], &data[data_size]);
}

/**
 * Generate the main lobe of a sinc function (Dirichlet kernel)
 *
 * @param x list of ints - array of indexes to compute;
 * @param N int - size of FFT to simulate
 * @return y list of floats - samples of the main lobe of a sinc function
 */
std::vector<float> Synthesis::sinc (std::vector<float> x, int N)
{
    // Initialize y
    std::vector<float> y (x.size(), N);

    for (int i = 0; i < x.size(); i++)
        y[i] = std::sin (N * x[i] / 2) / std::sin (x[i] / 2);

    return y;
}

template <typename T>
inline std::vector<T> Synthesis::getRange (T start, T end)
{
    size_t         N = (int) floor (end - start) + 1;
    std::vector<T> vec (N);
    std::iota (vec.begin(), vec.end(), start);
    return vec;
}
}; // namespace Core
