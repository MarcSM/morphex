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

#include <JuceHeader.h>

class SoundPanel : public juce::Component,
public juce::DragAndDropTarget,
public juce::Timer
{
public:
    SoundPanel (MorphexAudioProcessor& processor, int m_numSound) :
        m_numSound (m_numSound),
        m_instrument (processor.m_synth.m_instrument),
        m_soundNumberPanel (m_numSound),
        m_soundNamePanel (this)
    {
        if (m_numSound == 1)
            m_morphLocation = MorphLocation::UpLeft;
        else if (m_numSound == 4)
            m_morphLocation = MorphLocation::DownRight;
        else
            m_morphLocation = MorphLocation::Total;

        if (m_morphLocation != MorphLocation::Total)
            this->updateCurrentSound();
        else
            m_sound = new morphex::Sound();

        addAndMakeVisible (m_soundNumberPanel);
        addAndMakeVisible (m_soundNamePanel);
        addAndMakeVisible (m_items.add (new GridItemPanel (juce::Colours::blue, "2")));
        addAndMakeVisible (m_items.add (new GridItemPanel (juce::Colours::green, "3")));

        // Start the timer callback
        startTimer (100);
    }

    ~SoundPanel() {}

    void paint (juce::Graphics& g) override
    {
        g.setColour (juce::Colours::white);
        g.setFont (14.0f);

        // Draw borders
        GUI::Paint::drawBorders (g, getLocalBounds());
    }

    void resized() override
    {
        juce::Grid grid;

        float grid_margin = getWidth() * 0.04f;

        grid.rowGap = juce::Grid::Px (grid_margin);
        ;
        grid.columnGap = juce::Grid::Px (grid_margin);
        ;

        using Track = juce::Grid::TrackInfo;
        using juce::operator""_fr;


        grid.templateRows = { Track (1_fr),
                              Track (1_fr),
                              Track (1_fr) };

        grid.templateColumns = { Track (1_fr), Track (1_fr), Track (1_fr) };

        grid.autoColumns = Track (1_fr);
        grid.autoRows    = Track (1_fr);

        if (m_morphLocation == MorphLocation::UpLeft or m_numSound == 3)
        {
            grid.items.addArray ({
                juce::GridItem (m_soundNumberPanel),
                juce::GridItem (m_soundNamePanel).withArea (1, 2, 1, 4),
                juce::GridItem (m_items[0]).withArea (2, 1, 4, 1),
                juce::GridItem (m_items[1]).withArea (2, 2, 4, 4),
            });
        }
        else
        {
            grid.items.addArray ({
                juce::GridItem (m_soundNamePanel).withArea (1, 1, 1, 3),
                juce::GridItem (m_soundNumberPanel),
                juce::GridItem (m_items[0]).withArea (2, 1, 4, 3),
                juce::GridItem (m_items[1]).withArea (2, 3, 4, 3),
            });
        }

        grid.justifyContent = juce::Grid::JustifyContent::spaceBetween;
        grid.justifyItems   = juce::Grid::JustifyItems::stretch;
        grid.alignContent   = juce::Grid::AlignContent::spaceAround;

        juce::Rectangle<int> grid_bounds (grid_margin, grid_margin, getWidth() - (grid_margin * 2.0f), getHeight() - (grid_margin * 2.0f));

        grid.performLayout (grid_bounds);
    }

    bool isInterestedInDragSource (const SourceDetails&) override { return true; }

    void itemDragEnter (const SourceDetails&) override
    {
        m_isDragOver = true;
        repaint();
    }

    void itemDragExit (const SourceDetails&) override
    {
        m_isDragOver = false;
        repaint();
    }

    void itemDropped (const SourceDetails& dragSourceDetails) override
    {
        std::string sound_file_path = dragSourceDetails.description.toString().toStdString();

        if (sound_file_path != "directory" and m_morphLocation != MorphLocation::Total)
        {
            m_instrument.loadSound (sound_file_path, m_morphLocation);

            this->updateCurrentSound();
        }

        m_isDragOver = false;
        repaint();
    }

    void paintOverChildren (juce::Graphics& g) override
    {
        if (m_isDragOver)
        {
            g.setColour (juce::Colours::white.withAlpha (0.75f));
            g.drawRect (getLocalBounds(), 2);
        }

        if (m_numSound == 2 or m_numSound == 3)
        {
            g.setColour (juce::Colours::white);
            g.drawText ("Disabled", getLocalBounds(), juce::Justification::centred, false);

            GUI::Paint::drawDisabled (g, getLocalBounds());
        }
    }

private:
    struct SoundNumberPanel : public Component
    {
        SoundNumberPanel (int m_numSound) :
            m_numSound (m_numSound) {}

        void paint (juce::Graphics& g) override
        {
            g.setColour (GUI::Color::Logo);
            g.setFont (font_6);

            if (m_numSound == 1 or m_numSound == 3)
            {
                g.drawText (juce::String (m_numSound), getLocalBounds(), juce::Justification::topLeft, false);
            }
            else
            {
                g.drawText (juce::String (m_numSound), getLocalBounds(), juce::Justification::topRight, false);
            }
        }

        int m_numSound;
    };

    struct SoundNamePanel : public Component
    {
        SoundNamePanel (SoundPanel* sound_panel) :
            sound_panel (sound_panel) {}

        void paint (juce::Graphics& g) override
        {
            g.setColour (GUI::Color::Logo);
            g.setFont (font_2);

            if (sound_panel->m_sound->loaded)
            {
                if (sound_panel->m_numSound == 1 or sound_panel->m_numSound == 3)
                {
                    g.drawText (sound_panel->m_sound->name, getLocalBounds(), juce::Justification::topRight, false);
                }
                else
                {
                    g.drawText (sound_panel->m_sound->name, getLocalBounds(), juce::Justification::topLeft, false);
                }
            }
        }

        SoundPanel* sound_panel;
    };

    struct GridItemPanel : public Component
    {
        GridItemPanel (juce::Colour colourToUse, const juce::String& textToUse) :
            colour (colourToUse), text (textToUse) {}

        void paint (juce::Graphics& g) override {}

        juce::Colour colour;
        juce::String text;
    };

    void timerCallback() override
    {
        if (m_morphLocation < MorphLocation::Total and this->m_currentSoundPath != m_instrument.getMorphSound (m_morphLocation)->path)
        {
            this->updateCurrentSound();
            repaint();
        }
    }

    void updateCurrentSound()
    {
        m_sound                  = m_instrument.getMorphSound (m_morphLocation);
        this->m_currentSoundPath = m_sound->path;
    }

    std::string m_currentSoundPath;

    int           m_numSound;
    MorphLocation m_morphLocation;

    morphex::Instrument& m_instrument;
    morphex::Sound*      m_sound;

    SoundNumberPanel          m_soundNumberPanel;
    SoundNamePanel            m_soundNamePanel;
    juce::OwnedArray<GridItemPanel> m_items;
    juce::OwnedArray<juce::TextButton>    m_buttons;

    bool m_isDragOver = false;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SoundPanel)
};
