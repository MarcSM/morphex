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

#include "synth.h"

namespace Constants
{
constexpr auto MaxVoices = 10;
} // namespace Constants

namespace Core
{
MorphexSynth::MorphexSynth (AudioProcessorValueTreeState& parameters) :
    m_parameters (parameters),
    m_reverb (parameters)
{
    m_instrument.mode               = Instrument::Mode::Morphing;
    m_instrument.interpolation_mode = Instrument::Interpolation::Manual;

    m_instrument.generate.harmonic   = true;
    m_instrument.generate.sinusoidal = false;
    m_instrument.generate.stochastic = false;
    m_instrument.generate.attack     = false;
    m_instrument.generate.residual   = false;

    //    m_instrument.mode               = Instrument::Mode::FullRange;
    //    m_instrument.interpolation_mode = Instrument::Interpolation::None;
    //
    //    m_instrument.generate.harmonic   = true;
    //    m_instrument.generate.sinusoidal = true;
    //    m_instrument.generate.stochastic = true;
    //    m_instrument.generate.attack     = true;
    //    m_instrument.generate.residual   = false;

    //    // For testing the full range instrument case
    //    const bool loadDefaultSounds = false;
    //
    //    if (loadDefaultSounds)
    //    {
    //        const auto full_path = PLUGIN_DATA_COLLECTIONS_FACTORY_DIRECTORY.toStdString();
    //
    //        m_instrument.loadAllSoundsFromFolder (full_path);
    //    }

    // Add some voices to our synth, to play the sounds..
    for (int i = 0; i < Constants::MaxVoices; i++)
    {
        addVoice (new Voice (m_instrument, parameters));
    }

    // TODO - NOTE: If we enter on release stated, there is no way back, the
    // sound goes always down, even if a new sound is selected in a pitchbend from
    // the instrument also on stop note, jump to the release section of the sound
    // (all 4 morph sounds)

    // TODO - Maybe we should move the moprhing code from Voice to SynthSound
    // and in Voice manipulate the output audio buffer (already morphed) to
    // decorrelate the signal (or maybe add some "spread")

    // Add a sound for them to play
    clearSounds();
    addSound (new SynthSound());
    setNoteStealingEnabled (true);
}

MorphexSynth::~MorphexSynth() {}

void MorphexSynth::setCurrentPlaybackSampleRate (double sampleRate)
{
    Voice* morph_voice;

    // Set new sample rate to ADSR for each voice instance
    for (int i = 0; i < getNumVoices(); i++)
    {
        if ((morph_voice = dynamic_cast<Voice*> (getVoice (i))))
        {
            morph_voice->setADSRSampleRate (sampleRate);
        }
    }

    // Call base class method
    Synthesiser::setCurrentPlaybackSampleRate (sampleRate);
}

void MorphexSynth::renderNextBlock (AudioBuffer<float>& outputAudio,
                                    const MidiBuffer&   inputMidi,
                                    int                 startSample,
                                    int                 numSamples)
{
    // Call base class method
    Synthesiser::renderNextBlock (outputAudio, inputMidi, startSample, numSamples);

    auto& reverbDryWet = *m_parameters.getRawParameterValue (Morphex::PARAMETERS<float>[Morphex::Parameters::ReverbDryWet].ID);

    // DSP
    m_reverb.process (outputAudio, outputAudio.getNumSamples());

    //    // Output gain
    //    for (int channel = 0; channel < outputAudio.getNumChannels(); ++channel)
    //    {
    //        auto* buffer = outputAudio.getWritePointer (channel);
    //
    //        // Output gain
    //        float output_gain = *mParameters->getRawParameterValue
    //        (Morphex::PARAMETERS<float>[Morphex::Parameters::OutputGain].ID);
    //        mOutputGain[channel]->process (buffer, output_gain,
    //        outputAudio.getNumSamples());
    //    }
}

void MorphexSynth::reset()
{
    // Reset the instrument
    m_instrument.reset();
}
} // namespace Core