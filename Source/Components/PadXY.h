/*
  ==============================================================================

    PadXY.h
    Created: 14 May 2020 2:25:06pm
    Author:  Marc Sanchez Martinez

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class PadXY    :    public Component, public Timer
{
public:
    PadXY(SpectralMorphingToolAudioProcessor* inProcessor,
          AudioProcessorValueTreeState& stateToControl,
          const String& x_parameterID, const String& x_parameterLabel,
          const String& y_parameterID, const String& y_parameterLabel)
    : circle(), mProcessor (inProcessor)
    {
        // In your constructor, you should add any child components, and
        // initialise any special settings that your component needs.
        
        // Default dimensions
        setSize(getWidth(), getHeight());
        
        // Default values
        x_min = 0; x_max = 1;
        y_min = 0; y_max = 1;
        x_val = (x_min + x_max) / 2;
        y_val = (x_min + x_max) / 2;
        invert_y = true;
        
        // Add the circle
        addAndMakeVisible(circle);
        
        // Initialize the sliders
        x_axis_slider = new Slider(x_parameterLabel);
        y_axis_slider = new Slider(y_parameterLabel);
        
        // Attach the sliders to the AudioProcessorValueTreeState
        x_axis_slider_attachment =
        new AudioProcessorValueTreeState::
        SliderAttachment(stateToControl, x_parameterID, *x_axis_slider);
        y_axis_slider_attachment =
        new AudioProcessorValueTreeState::
        SliderAttachment(stateToControl, y_parameterID, *y_axis_slider);
        
        // Define the range for the sliders
        x_axis_slider->setRange (x_min, x_max);
        x_axis_slider->setName (getName() + "_x");
        y_axis_slider->setRange (y_min, y_max);
        y_axis_slider->setName (getName() + "_y");
        
        // Start the timer callback
        startTimer(XYPAD_UI_REFRESH_TIMER_CALLBACK); // 0.5 seconds
    }

    ~PadXY()
    {
    }

    void paint (Graphics& g) override
    {
        int borderWidth = 0;
        int cornerSize = 4;
        
        // Background
        rectangle.setPosition(borderWidth / 2, borderWidth / 2);
        rectangle.setSize(getWidth() - borderWidth, getHeight() - borderWidth);
        g.setColour (Colours::transparentBlack);
        g.fillRoundedRectangle (rectangle, cornerSize);
        
//        // Background gradient
//        ColourGradient cg (GUI::Color::Accent.withAlpha(0.5f),
//                           getWidth() / 2.0f, getHeight() / 2.0f,
//                           GUI::Color::Accent.withAlpha(0.0f),
//                           getWidth(), getHeight(), true);
//
//        g.setFillType (cg);
//        g.fillRect (0, 0, getWidth(), getHeight());
        
//        // Circle glow
//        float glow_radius = circle.getWidth() * 2.0f;
//        float glow_x = circle.getX() + ( circle.getWidth() / 2.0f );
//        float glow_y = circle.getY() + ( circle.getHeight() / 2.0f );
//        float glow_w = glow_radius * 2.0f;
//        float glow_h = glow_radius * 2.0f;
//
//        ColourGradient cg (GUI::Color::Accent.withAlpha(0.3f),
//                           glow_x, glow_y,
//                           GUI::Color::Accent.withAlpha(0.0f),
//                           glow_x + (glow_radius * 0.4f),
//                           glow_y + (glow_radius * 0.4f), true);
//
//        g.setFillType (cg);
//        g.fillRect (glow_x - glow_radius, glow_y - glow_radius, glow_w, glow_h);
    }

    void resized() override
    {
        // Update the background rectangle
        rectangle.setWidth (getWidth() * .98);
        rectangle.setHeight (getHeight() * .85);
        rectangle.setTop (getHeight() * .02);
        rectangle.setLeft (getWidth() * .02);
        
        // Update the circle
        circle.setSize (20, 20);
        const Point<int> pos (getValueAsPosition (x_val, y_val));
        circle.setTopLeftPosition (pos.getX(), pos.getY());
        circle.setInterceptsMouseClicks (false, false);
        circle.setColour (GUI::Color::Accent);
        
        // Update the circle glow
        //circle_glow_bounds.setTopLeftPosition( circle );
    }
    
    void setValues(float x, float y, bool notify = true)
    {
        x_axis_slider->setValue(x, sendNotification);
        y_axis_slider->setValue(y, sendNotification);
    }
    
    class PadCircle : public Component
    {
        Point<float> circleXY;
        Colour colour;
        
    public:
        
        PadCircle() {}
        
        void setColour (Colour col)
        {
            colour = col;
            repaint();
        }
        
        void paint (Graphics& g)  override
        {
            //            g.setColour (Colours::yellow);
            //            g.drawEllipse (getWidth() - 70, 10, 60, 60, 3);
            
            g.fillAll (Colours::transparentBlack);
            g.setColour (colour);
            
            Rectangle<int> cirlce_bounds = getLocalBounds();
            
            float line_thickness = 4.0f;
            float circle_x = cirlce_bounds.getX() + (line_thickness / 2.0f);
            float circle_y = cirlce_bounds.getY() + (line_thickness / 2.0f);
            float circle_w = cirlce_bounds.getWidth() - line_thickness;
            float circle_h = cirlce_bounds.getHeight() - line_thickness;
            
            g.drawEllipse (circle_x, circle_y, circle_w, circle_h, line_thickness);
            
            //            g.drawEllipse (0,0,20,20, 5);
            //            g.drawEllipse (getLocalBounds().toFloat(), 5);
            //            g.drawEllipse (getWidth() - 70, 10, 60, 60, 3);
        }
    };
    
    PadCircle* getCircle()
    {
        return &circle;
    }
    
private:
    
    Point<int> constrainPosition (float x, float y)
    {
        const float xPos = jlimit (rectangle.getX(), (rectangle.getWidth() + rectangle.getX()) - circle.getWidth(), x - circle.getWidth() / 2.f);
        const float yPos = jlimit (rectangle.getY(), (rectangle.getHeight() + rectangle.getY()) - circle.getHeight(), y - circle.getHeight() / 2.f);
        return Point<int> (xPos, yPos);
    }
    
    Point<int> getValueAsPosition(float x, float y)
    {
        if (invert_y) y = y_max - y;
        
        const float xPos = jmap (x, rectangle.getX(), (rectangle.getWidth() + rectangle.getX()) - circle.getWidth());
        const float yPos = jmap (y, rectangle.getY(), (rectangle.getHeight() + rectangle.getY()) - circle.getHeight());
        return Point<int> (xPos, yPos);
    }
    
    void setPositionAsValue (Point<float> position)
    {
        const float xVal = jlimit (x_min, x_max, jmap (position.getX(), rectangle.getX(), rectangle.getWidth() - circle.getWidth(), x_min, x_max));
        float yVal = jlimit (y_min, y_max, jmap (position.getY(), rectangle.getY(), rectangle.getHeight() - circle.getHeight(), y_min, y_max));
        
        if (invert_y) yVal = y_max - yVal;
        
        setValues (xVal, yVal);
    }
    
    void mouseDown (const MouseEvent& e) override
    {
        circle.setTopLeftPosition(constrainPosition (e.getPosition().getX(), e.getPosition().getY()));
        mouseDownXY.setXY(circle.getPosition().getX() + circle.getWidth()*.5f, circle.getPosition().getY() + circle.getHeight()*.5f);
        setPositionAsValue(circle.getPosition().toFloat());
        repaint();
    }
    
    void mouseDrag (const MouseEvent& e) override
    {
        if (e.mouseWasDraggedSinceMouseDown())
        {
            circle.setTopLeftPosition (constrainPosition (mouseDownXY.getX() + e.getDistanceFromDragStartX(), mouseDownXY.getY() + e.getDistanceFromDragStartY()));
            setPositionAsValue (circle.getPosition().toFloat());
            repaint();
            
            currentMouseXY = circle.getPosition().toFloat();
        }
    }
    
    void timerCallback() override
    {
        // Check if a preset has been loaded
        if (mCurrentPresetName != mProcessor->getPresetManager()->getCurrentPresetName())
        {
            // Update the labels
            setValues(x_axis_slider->getValue(), y_axis_slider->getValue());
            
            Point<int> new_position (
                                     getValueAsPosition (
                                                         x_axis_slider->getValue(),
                                                         y_axis_slider->getValue()
                                                         )
                                     );
            
            circle.setTopLeftPosition(new_position.getX(), new_position.getY());
            
            // Repaint the component
            repaint();
            
            // Save the current preset name
            mCurrentPresetName = mProcessor->getPresetManager()->getCurrentPresetName();
        }
    }
    
    
    int mParameterID;
    
    PadCircle circle;
    Rectangle<float> rectangle;
    Rectangle<float> circle_glow_bounds;

    float x_min, x_max, y_min, y_max, x_val, y_val;
    bool invert_y;
    bool rightMouseButtonDown = false;
    
    Point<float> currentMouseXY;
    Point<float> mouseDownXY;
    
    ScopedPointer<Slider> x_axis_slider;
    ScopedPointer<Slider> y_axis_slider;
    
    ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> x_axis_slider_attachment;
    ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> y_axis_slider_attachment;
    
    String mCurrentPresetName;
    
    SpectralMorphingToolAudioProcessor* mProcessor;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PadXY)
};
