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

#include <vector>

namespace morphex
{
class Frame
{
public:
    struct FftComponent
    {
        std::vector<float> freqs;
        std::vector<float> mags;
        std::vector<float> phases;
    };

    const FftComponent&       getHarmonicComponent() const;
    const FftComponent&       getSinusoidalComponent() const;
    const std::vector<float>& getStochasticMagnitudes() const;
    const std::vector<float>& getAttackWaveform() const;
    const std::vector<float>& getResidualWaveform() const;
    size_t                    getMaxNumOfHarmonics() const;
    size_t                    getMaxNumOfSinusoids() const;

    void setHarmonicComponent (FftComponent component);
    void setSinusoidalComponent (FftComponent component);
    void setStochasticMagnitudes (std::vector<float> values);
    void setAttackWaveform (std::vector<float> waveform);
    void setResidualWaveform (std::vector<float> waveform);
    
    bool hasHarmonic() const { return m_harmonic.freqs.size() > 0; };
    bool hasSinusoidal() const { return m_sinusoidal.freqs.size() > 0; };
    bool hasPhases() const { return m_harmonic.phases.size() > 0; };
    bool hasSinusoidalPhases() const { return m_sinusoidal.phases.size() > 0; };
    bool hasStochastic() const { return m_stochastic.size() > 0; };
    bool hasAttack() const { return m_attack.size() > 0; };
    bool hasResidual() const { return m_residual.size() > 0; };

private:
    FftComponent       m_harmonic;
    FftComponent       m_sinusoidal;
    std::vector<float> m_stochastic;
    std::vector<float> m_attack;
    std::vector<float> m_residual;
};
}; // namespace morphex
