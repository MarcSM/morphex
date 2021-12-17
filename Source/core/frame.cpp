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

#include "frame.h"

namespace morphex
{
void Frame::Frame()
{
}

const FftComponent& Frame::getHarmonicComponent() const;
{
    return harmonic;
}

const FftComponent& Frame::getSinusoidalComponent() const;
{
    return sinusoidal;
}

const std::vector<float>& Frame::getStochasticMagnitudes() const;
{
    return stochastic;
}

const std::vector<float>& Frame::getAttackWaveform() const;
{
    return attack;
}

const std::vector<float>& Frame::getResidualWaveform() const;
{
    return residual;
}

size_t Frame::getMaxNumOfHarmonics() const
{
    auto maxNumOfHarmonics = std::max ({ harmonic.freqs.size(),
                                         harmonic.mags.size(),
                                         harmonic.phases.size() });

    return maxNumOfHarmonics;
};

size_t Frame::getMaxNumOfSinusoids() const
{
    auto maxNumOfSinusoids = std::max ({ sinusoidal.freqs.size(),
                                         sinusoidal.mags.size(),
                                         sinusoidal.phases.size() });

    return maxNumOfSinusoids;
};

void setHarmonicComponent (FftComponent component)
{
    harmonic = component;
}

void setSinusoidalComponent (FftComponent component)
{
    sinusoidal = component;
}

void setStochasticMagnitudes (std::vector<float> values)
{
    stochastic = values;
}

void setAttackWaveform (std::vector<float> waveform)
{
    attack = waveform;
}

void setResidualWaveform (std::vector<float> waveform)
{
    residual = waveform;
}

}; // namespace morphex