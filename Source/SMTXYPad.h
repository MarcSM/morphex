/*
  ==============================================================================

    SMTXYPad.h
    Created: 4 Jul 2019 12:48:34pm
    Author:  Marc Sanchez Martinez

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"

#include "PluginProcessor.h"

class SMTXYPad:
    public Component,
    public Timer
{
    Rectangle<float> rectangle;
    float x_min, x_max, y_min, y_max, x_val, y_val;
    bool rightMouseButtonDown = false;
    
    Point<float> currentMouseXY;
    Point<float> mouseDownXY;
    
    class XYDot : public Component
    {
        Point<float> dotXY;
        Colour colour;
        
    public:
        
        XYDot() {}
        
        void setColour (Colour col)
        {
            colour = col;
            repaint();
        }
        
        void paint (Graphics& g)  override
        {
            g.fillAll (Colours::transparentBlack);
            g.setColour (colour);
            g.fillEllipse (getLocalBounds().toFloat());
        }
    };
    
    XYDot dot;
    
    // Value labels
    Label xValueLabel, yValueLabel;
    
public:
    
    SMTXYPad(SpectralMorphingToolAudioProcessor* inProcessor,
             AudioProcessorValueTreeState& stateToControl,
             const String& x_parameterID, const String& x_parameterLabel,
             const String& y_parameterID, const String& y_parameterLabel);
    
    ~SMTXYPad();
    
    void paint(Graphics& g) override;
    void drawRectangle(Graphics& g);
    void resized() override;
    
    void mouseDown (const MouseEvent& e) override;
    void mouseDrag (const MouseEvent& e) override;
    
    Point<int> constrainPosition (float x, float y);
    Point<int> getValueAsPosition (float x, float y);
    void setPositionAsValue (Point<float> position);
    void setValues (float x, float y, bool notify = true);
    
    void timerCallback() override;
    
private:
    
    int mParameterID;
    
    int m_invert_y;
    
    SpectralMorphingToolAudioProcessor* mProcessor;
    
    ScopedPointer<Slider> x_axis_slider;
    ScopedPointer<Slider> y_axis_slider;
    
    ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> x_axis_slider_attachment;
    ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> y_axis_slider_attachment;
    
    String mCurrentPresetName;
};
