/*
  ==============================================================================

    SMTXYPad.cpp
    Created: 4 Jul 2019 12:48:34pm
    Author:  Marc Sanchez Martinez

  ==============================================================================
*/

#include "SMTXYPad.h"

#include "SMTInterfaceDefines.h"
#include "SMTParameters.h"

SMTXYPad::SMTXYPad(SpectralMorphingToolAudioProcessor* inProcessor,
                   AudioProcessorValueTreeState& stateToControl,
                   const String& x_parameterID, const String& x_parameterLabel,
                   const String& y_parameterID, const String& y_parameterLabel):
    x_min(0),
    x_max(1),
    y_min(0),
    y_max(1),
    x_val( (x_min + x_max) / 2),
    y_val( (x_min + x_max) / 2),
    dot(),
    xValueLabel(),
    yValueLabel(),
    mProcessor(inProcessor)
{
    // Default dimensions
    setSize(100, 100);
    
    m_invert_y = true;
    
    // Add the dot
    addAndMakeVisible(dot);
    
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
    
    // Set the x slider label
    xValueLabel.setBounds (0, rectangle.getHeight(), getWidth() / 2, 20);
    xValueLabel.setColour (Label::textColourId, Colours::white);
    xValueLabel.setFont (Font (12, 1));
    xValueLabel.setText (String (x_val, 3), dontSendNotification);
    xValueLabel.setJustificationType (Justification::centred);
    addAndMakeVisible (xValueLabel);
    
    // Set the y slider label
    yValueLabel.setBounds (getWidth()*.5, rectangle.getHeight(), getWidth() / 2, 20);
    yValueLabel.setColour (Label::textColourId, Colours::white);
    yValueLabel.setFont (Font (12, 1));
    yValueLabel.setText (String (y_val, 3), dontSendNotification);
    yValueLabel.setJustificationType (Justification::centred);
    addAndMakeVisible (yValueLabel);
    
    // Start the timer callback
    startTimer(XYPAD_UI_REFRESH_TIMER_CALLBACK); // 0.5 seconds
}

SMTXYPad::~SMTXYPad()
{
    
}

void SMTXYPad::mouseDown (const MouseEvent& e)
{
    dot.setTopLeftPosition(constrainPosition (e.getPosition().getX(), e.getPosition().getY()));
    mouseDownXY.setXY(dot.getPosition().getX() + dot.getWidth()*.5f, dot.getPosition().getY() + dot.getHeight()*.5f);
    setPositionAsValue(dot.getPosition().toFloat());
    repaint();
}

void SMTXYPad::mouseDrag (const MouseEvent& e)
{
    if (e.mouseWasDraggedSinceMouseDown())
    {
        dot.setTopLeftPosition (constrainPosition (mouseDownXY.getX() + e.getDistanceFromDragStartX(), mouseDownXY.getY() + e.getDistanceFromDragStartY()));
        setPositionAsValue (dot.getPosition().toFloat());
        repaint();
        
        currentMouseXY = dot.getPosition().toFloat();
    }
}

Point<int> SMTXYPad::constrainPosition (float x, float y)
{
    const float xPos = jlimit (rectangle.getX(), (rectangle.getWidth() + rectangle.getX()) - dot.getWidth(), x - dot.getWidth() / 2.f);
    const float yPos = jlimit (rectangle.getY(), (rectangle.getHeight() + rectangle.getY()) - dot.getHeight(), y - dot.getHeight() / 2.f);
    return Point<int> (xPos, yPos);
}

Point<int> SMTXYPad::getValueAsPosition(float x, float y)
{
    if (m_invert_y) y = y_max - y;
    
    const float xPos = jmap (x, rectangle.getX(), (rectangle.getWidth() + rectangle.getX()) - dot.getWidth());
    const float yPos = jmap (y, rectangle.getY(), (rectangle.getHeight() + rectangle.getY()) - dot.getHeight());
    return Point<int> (xPos, yPos);
}

void SMTXYPad::setPositionAsValue (Point<float> position)
{
    const float xVal = jlimit (x_min, x_max, jmap (position.getX(), rectangle.getX(), rectangle.getWidth() - dot.getWidth(), x_min, x_max));
    float yVal = jlimit (y_min, y_max, jmap (position.getY(), rectangle.getY(), rectangle.getHeight() - dot.getHeight(), y_min, y_max));
    
    if (m_invert_y) yVal = y_max - yVal;
    
    setValues (xVal, yVal);
}

void SMTXYPad::setValues(float x, float y, bool notify)
{
    x_axis_slider->setValue(x, sendNotification);
    y_axis_slider->setValue(y, sendNotification);
    
    xValueLabel.setText (String (x, 3), dontSendNotification);
    yValueLabel.setText (String (y, 3), dontSendNotification);
}

void SMTXYPad::paint(Graphics& g)
{
    int borderWidth = 0;
    int cornerSize = 4;
    
    // Text label
    g.setColour (Colours::white);

    // Background
    rectangle.setPosition(borderWidth / 2, borderWidth / 2);
    rectangle.setSize(getWidth() - borderWidth, getHeight() - borderWidth);
    g.setColour (Colours::black);
    g.fillRoundedRectangle (rectangle, cornerSize);
}

void SMTXYPad::resized()
{
    // Update the background rectangle
    rectangle.setWidth (getWidth()*.98);
    rectangle.setHeight (getHeight()*.85);
    rectangle.setTop (getHeight()*.02);
    rectangle.setLeft (getWidth()*.02);
    
    // Update the dot
    dot.setSize(20, 20);
    const Point<int> pos (getValueAsPosition (x_val, y_val));
    dot.setTopLeftPosition(pos.getX(), pos.getY());
    dot.setInterceptsMouseClicks (false, false);
    dot.setColour (Colours::white);
    
    // Update the labels
    xValueLabel.setBounds (0, rectangle.getHeight(), getWidth() / 2, 20);
    yValueLabel.setBounds (getWidth()*.5, rectangle.getHeight(), getWidth() / 2, 20);
}

void SMTXYPad::timerCallback()
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
        
        dot.setTopLeftPosition(new_position.getX(), new_position.getY());
        
        // Repaint the component
        repaint();
        
        // Save the current preset name
        mCurrentPresetName = mProcessor->getPresetManager()->getCurrentPresetName();
    }
}
