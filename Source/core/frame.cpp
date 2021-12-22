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
const Frame::FftComponent& Frame::getHarmonicComponent() const
{
    return m_harmonic;
}

const Frame::FftComponent& Frame::getSinusoidalComponent() const
{
    return m_sinusoidal;
}

const std::vector<float>& Frame::getStochasticMagnitudes() const
{
    return m_stochastic;
}

const std::vector<float>& Frame::getAttackWaveform() const
{
    return m_attack;
}

const std::vector<float>& Frame::getResidualWaveform() const
{
    return m_residual;
}

size_t Frame::getMaxNumOfHarmonics() const
{
    auto maxNumOfHarmonics = std::max ({ m_harmonic.freqs.size(),
        m_harmonic.mags.size(),
        m_harmonic.phases.size() });

    return maxNumOfHarmonics;
};

size_t Frame::getMaxNumOfSinusoids() const
{
    auto maxNumOfSinusoids = std::max ({ m_sinusoidal.freqs.size(),
        m_sinusoidal.mags.size(),
        m_sinusoidal.phases.size() });

    return maxNumOfSinusoids;
};

void Frame::setHarmonicComponent (Frame::FftComponent component)
{
    m_harmonic = component;
}

void Frame::setSinusoidalComponent (Frame::FftComponent component)
{
    m_sinusoidal = component;
}

void Frame::setStochasticMagnitudes (std::vector<float> values)
{
    m_stochastic = values;
}

void Frame::setAttackWaveform (std::vector<float> waveform)
{
    m_attack = waveform;
}

void Frame::setResidualWaveform (std::vector<float> waveform)
{
    m_residual = waveform;
}

}; // namespace morphex
