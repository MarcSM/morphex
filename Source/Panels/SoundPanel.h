/*
  ==============================================================================

    SoundPanel.h
    Created: 12 May 2020 1:18:25pm
    Author:  Marc Sanchez Martinez

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class SoundPanel :  public Component,
                    public DragAndDropTarget,
                    public Timer
{
public:
    
    SoundPanel (SpectralMorphingToolAudioProcessor* inProcessor, int i_sound_num)
    :   i_sound_num (i_sound_num),
        instrument (&inProcessor->mMorphexSynth.instrument),
        soundNumberPanel (i_sound_num),
        soundNamePanel (this)
    {
        if (i_sound_num == 1) morph_location = MorphLocation::Left;
        else if (i_sound_num == 4) morph_location = MorphLocation::Right;
        else morph_location = MorphLocation::NUM_MORPH_LOCATIONS;
        
        if (morph_location != MorphLocation::NUM_MORPH_LOCATIONS) this->updateCurrentSound();
        else this->sound = new Sound();
        
        addAndMakeVisible (soundNumberPanel);
        addAndMakeVisible (soundNamePanel);
        addAndMakeVisible (items.add (new GridItemPanel (Colours::blue, "2")));
        addAndMakeVisible (items.add (new GridItemPanel (Colours::green, "3")));
        
        // Start the timer callback
        startTimer (GUI_REFRESH_TIMER_CALLBACK_SOUND);
    }

    ~SoundPanel() {}

    void paint (Graphics& g) override
    {
        g.setColour (Colours::white);
        g.setFont (14.0f);
        
        // Draw borders
        GUI::Paint::drawBorders (g, getLocalBounds());
    }

    void resized() override
    {
        Grid grid;
        
        float grid_margin = getWidth() * 0.04f;

        grid.rowGap    = Grid::Px (grid_margin);;
        grid.columnGap = Grid::Px (grid_margin);;
        
        using Track = Grid::TrackInfo;
        
        grid.templateRows = {Track (1_fr),
                             Track (1_fr),
                             Track (1_fr)};
        
        grid.templateColumns = {Track (1_fr), Track (1_fr), Track (1_fr)};
        
        grid.autoColumns = Track (1_fr);
        grid.autoRows    = Track (1_fr);

        if (morph_location == MorphLocation::Left or i_sound_num == 3)
        {
            grid.items.addArray
            ({
                GridItem (soundNumberPanel),
                GridItem (soundNamePanel).withArea (1, 2, 1, 4),
                GridItem (items[0]).withArea (2, 1, 4, 1),
                GridItem (items[1]).withArea (2, 2, 4, 4),
            });
        }
        else
        {
            grid.items.addArray
            ({
                GridItem (soundNamePanel).withArea (1, 1, 1, 3),
                GridItem (soundNumberPanel),
                GridItem (items[0]).withArea (2, 1, 4, 3),
                GridItem (items[1]).withArea (2, 3, 4, 3),
            });
        }
        
        grid.justifyContent = Grid::JustifyContent::spaceBetween;
        grid.justifyItems = Grid::JustifyItems::stretch;
        grid.alignContent = Grid::AlignContent::spaceAround;
        
        Rectangle<int> grid_bounds (grid_margin, grid_margin,
                                    getWidth() - ( grid_margin * 2.0f ),
                                    getHeight() - ( grid_margin * 2.0f ));
        
        grid.performLayout (grid_bounds);
    }
    
    bool isInterestedInDragSource (const SourceDetails&) override { return true; }
    
    void itemDragEnter (const SourceDetails&) override
    {
        isDragOver = true;
        repaint();
    }
    
    void itemDragExit (const SourceDetails&) override
    {
        isDragOver = false;
        repaint();
    }
    
    void itemDropped (const SourceDetails& dragSourceDetails) override
    {
        std::string sound_file_path = dragSourceDetails.description.toString().toStdString();
        
        if (sound_file_path != "directory" and morph_location != MorphLocation::NUM_MORPH_LOCATIONS)
        {
            instrument->loadSound (sound_file_path, morph_location);
            
            this->updateCurrentSound();
        }
        
        isDragOver = false;
        repaint();
    }
    
    void paintOverChildren (Graphics& g) override
    {
        if (isDragOver)
        {
            g.setColour (Colours::white.withAlpha(0.75f));
            g.drawRect (getLocalBounds(), 2);
        }
        
        if ( i_sound_num == 2 or i_sound_num == 3 )
        {
            g.setColour (Colours::white);
            g.drawText ("Disabled", getLocalBounds(), Justification::centred, false);
            
            GUI::Paint::drawDisabled (g, getLocalBounds());
        }
    }

private:
    
    struct SoundNumberPanel : public Component
    {
        SoundNumberPanel (int i_sound_num) : i_sound_num (i_sound_num) {}
        
        void paint (Graphics& g) override
        {
            g.setColour (GUI::Color::Logo);
            g.setFont (font_6);
            
            if ( i_sound_num == 1 or i_sound_num == 3 )
            {
                g.drawText (String (i_sound_num), getLocalBounds(), Justification::topLeft, false);
            }
            else
            {
                g.drawText (String (i_sound_num), getLocalBounds(), Justification::topRight, false);
            }
        }
        
        int i_sound_num;
    };
    
    struct SoundNamePanel : public Component
    {
        SoundNamePanel (SoundPanel* sound_panel) : sound_panel (sound_panel) {}
        
        void paint (Graphics& g) override
        {
            g.setColour (GUI::Color::Logo);
            g.setFont (font_2);
            
            if (sound_panel->sound->loaded)
            {
                if ( sound_panel->i_sound_num == 1 or sound_panel->i_sound_num == 3 )
                {
                    g.drawText (sound_panel->sound->name, getLocalBounds(), Justification::topRight, false);
                }
                else
                {
                    g.drawText (sound_panel->sound->name, getLocalBounds(), Justification::topLeft, false);
                }
            }
        }
        
        SoundPanel* sound_panel;
    };
    
    struct GridItemPanel : public Component
    {
        GridItemPanel (Colour colourToUse, const String& textToUse) : colour (colourToUse), text (textToUse) {}
        
        void paint (Graphics& g) override {}
        
        Colour colour;
        String text;
    };
    
    void timerCallback() override
    {
        if (morph_location < MorphLocation::NUM_MORPH_LOCATIONS and
            this->current_sound_path != instrument->getMorphSound (morph_location)->path)
        {
            this->updateCurrentSound();
            repaint();
        }
    }
    
    void updateCurrentSound()
    {
        this->sound = instrument->getMorphSound (morph_location);
        this->current_sound_path = this->sound->path;
    }
    
    std::string current_sound_path;
    
    int i_sound_num;
    MorphLocation morph_location;
    
    Instrument* instrument;
    Sound* sound;
    
    SoundNumberPanel soundNumberPanel;
    SoundNamePanel soundNamePanel;
    OwnedArray<GridItemPanel> items;
    OwnedArray<TextButton> buttons;
    
    bool isDragOver = false;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SoundPanel)
};
