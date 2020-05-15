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
class SoundPanel    : public Component
{
public:
    
    SoundPanel (SpectralMorphingToolAudioProcessor* inProcessor, int i_sound_num)
    :   i_sound_num (i_sound_num),
        instrument (&inProcessor->mMorphexSynth.instrument)
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

        // Draw borders
        GUI::Paint::drawBorders(g, getLocalBounds());

        g.setColour (Colours::white);
        g.setFont (14.0f);
//        g.drawText ("SoundPanel", getLocalBounds(),
//                    Justification::centred, true);   // draw some placeholder text
        
//        g.drawText (instrument->name, getLocalBounds(),
//                    Justification::centred, true);   // draw some placeholder text
        
        g.drawText (sound->name, getLocalBounds(),
                    Justification::centred, true);   // draw some placeholder text
    }

    void resized() override
    {
        // This method is where you should set the bounds of any child
        // components that your component contains..
        
        FlexBox fb;
        fb.flexWrap = FlexBox::Wrap::wrap;
        fb.justifyContent = FlexBox::JustifyContent::center;
        fb.alignContent = FlexBox::AlignContent::center;

        for (auto* b : buttons)
        {
            fb.items.add (FlexItem (*b).withMinWidth (50.0f).withMinHeight (50.0f));
        }
        
        fb.performLayout (getLocalBounds().toFloat());
    }

private:

    int i_sound_num;
    
    Instrument* instrument;
    Sound* sound;
//    Instrument& instrument;
    
    OwnedArray<TextButton> buttons;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SoundPanel)
};
