/*
  ==============================================================================

    SoundPanel.h
    Created: 12 May 2020 1:18:25pm
    Author:  Marc Sanchez Martinez

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class SoundPanel :  public Component,
                    public DragAndDropTarget
{
public:
    
    SoundPanel (SpectralMorphingToolAudioProcessor* inProcessor, int i_sound_num)
    :   instrument (&inProcessor->mMorphexSynth.instrument),
        i_sound_num (i_sound_num),
        soundNumberPanel (i_sound_num),
        soundNamePanel (this)
    {
        // In your constructor, you should add any child components, and
        // initialise any special settings that your component needs.
        
        if (i_sound_num == 1)
        {
            sound = instrument->getMorphSounds()[MorphLocation::Left];
        }
        else if (i_sound_num == 4)
        {
            sound = instrument->getMorphSounds()[MorphLocation::Right];
        }
        else
        {
            sound = new Sound();
        }
        
//        soundNamePanel = new SoundNamePanel (sound)
        
        addAndMakeVisible (soundNumberPanel);
        addAndMakeVisible (soundNamePanel);
        addAndMakeVisible (items.add (new GridItemPanel (Colours::blue, "2")));
        addAndMakeVisible (items.add (new GridItemPanel (Colours::green, "3")));
    }

    ~SoundPanel()
    {
    }

    void paint (Graphics& g) override
    {
        /* This demo code just fills the component's background and
           draws some placeholder text to get you started.

           You should replace everything in this method with your own
           drawing code..
        */

//        g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));   // clear the background

        

        g.setColour (Colours::white);
        g.setFont (14.0f);
//        g.drawText ("SoundPanel", getLocalBounds(),
//                    Justification::centred, true);   // draw some placeholder text
        
//        g.drawText (instrument->name, getLocalBounds(),
//                    Justification::centred, true);   // draw some placeholder text
        
//        g.drawText (sound->name, getLocalBounds(),
//                    Justification::centred, true);   // draw some placeholder text
        
        // Draw borders
        GUI::Paint::drawBorders(g, getLocalBounds());
    }

    void resized() override
    {
        // This method is where you should set the bounds of any child
        // components that your component contains..
        
        Grid grid;
        
        float grid_margin = getWidth() * 0.04f;
        
//        Grid::Px row_gap = Grid::Px (grid_margin);
//        Grid::Px column_gap = Grid::Px (grid_margin);

        grid.rowGap    = Grid::Px (grid_margin);;
        grid.columnGap = Grid::Px (grid_margin);;
        
//        grid.rowGap    = 20_px;
//        grid.columnGap = 20_px;
        
        using Track = Grid::TrackInfo;
        
        grid.templateRows = { Track (1_fr), Track (1_fr), Track (1_fr) };
        
        grid.templateColumns = { Track (1_fr),
            Track (1_fr),
            Track (1_fr) };
        
        
        grid.autoColumns = Track (1_fr);
        grid.autoRows    = Track (1_fr);
        
//        grid.autoFlow = Grid::AutoFlow::column;

        if (i_sound_num == 1 or i_sound_num == 3)
        {
            grid.items.addArray ({
                GridItem (soundNumberPanel),
                GridItem (soundNamePanel).withArea (1, 2, 1, 4),
                GridItem (items[0]).withArea (2, 1, 4, 1),
                GridItem (items[1]).withArea (2, 2, 4, 4),
            });
        }
        else
        {
            grid.items.addArray ({
                GridItem (soundNamePanel).withArea (1, 1, 1, 3),
                GridItem (soundNumberPanel),
                GridItem (items[0]).withArea (2, 1, 4, 3),
                GridItem (items[1]).withArea (2, 3, 4, 3),
            });
        }
        
        grid.justifyContent = Grid::JustifyContent::spaceBetween;
        grid.justifyItems = Grid::JustifyItems::stretch;
        grid.alignContent = Grid::AlignContent::spaceAround;
//        grid.justifyContent = Grid::JustifyContent::spaceAround;

        
        Rectangle<int> grid_bounds(grid_margin, grid_margin,
                                   getWidth() - ( grid_margin * 2.0f ),
                                   getHeight() - ( grid_margin * 2.0f ));
        grid.performLayout (grid_bounds);
//        grid.performLayout (getLocalBounds());
    }
    
    bool isInterestedInDragSource (const SourceDetails&) override   { return true; }
    
    void itemDragEnter (const SourceDetails&) override
    {
//        isDragOver = true;
        repaint();
    }
    
    void itemDragExit (const SourceDetails&) override
    {
//        isDragOver = false;
        repaint();
    }
    
    void itemDropped (const SourceDetails& dragSourceDetails) override
    {
        DBG("itemDropped");
        DBG(dragSourceDetails.description.toString());
        //        setFile (dragSourceDetails.description.toString());
        //        isDragOver = false;
        repaint();
    }

private:
    
    struct SoundNumberPanel  : public Component
    {
        SoundNumberPanel (int i_sound_num)
        :   i_sound_num( i_sound_num )
        {}
        
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
    
    struct SoundNamePanel  : public Component
    {
        SoundNamePanel (SoundPanel* sound_panel)
        :   sound_panel( sound_panel )
        {}
        
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
    
    struct GridItemPanel  : public Component
    {
        GridItemPanel (Colour colourToUse, const String& textToUse)
        : colour (colourToUse),
        text (textToUse)
        {}
        
        void paint (Graphics& g) override
        {
//            g.fillAll (colour.withAlpha (0.5f));
//
//            g.setColour (Colours::black);
//            g.drawText (text, getLocalBounds().withSizeKeepingCentre (100, 100), Justification::centred, false);
        }
        
        Colour colour;
        String text;
    };
    
    int i_sound_num;
    
    Instrument* instrument;
    Sound* sound;
//    Instrument& instrument;
    
    SoundNumberPanel soundNumberPanel;
    SoundNamePanel soundNamePanel;
    OwnedArray<GridItemPanel> items;
    OwnedArray<TextButton> buttons;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SoundPanel)
};
