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
 *
 * Based on the XYPad by Rory Walsh.
 */

#pragma once

#include <JuceHeader.h>

namespace Constants
{
constexpr auto GuiRefreshTimer = 100; // 0.1 seconds
} // namespace Constants

class PadXY : public Component, public Timer
{
public:
    PadXY (MorphexAudioProcessor& inProcessor,
           AudioProcessorValueTreeState&       stateToControl,
           Morphex::Parameter<float>           freqs_interp_factor_parameter,
           Morphex::Parameter<float>           mags_interp_factor_parameter) :
        m_processor (inProcessor)
    {
        const String& x_parameterID    = freqs_interp_factor_parameter.ID;
        const String& x_parameterLabel = freqs_interp_factor_parameter.label;
        const String& y_parameterID    = mags_interp_factor_parameter.ID;
        const String& y_parameterLabel = mags_interp_factor_parameter.label;

        // Default dimensions
        setSize (getWidth(), getHeight());

        // Default values
        m_xMin    = 0;
        m_xMax    = 1;
        m_yMin    = 0;
        m_yMax    = 1;
        m_xVal    = (m_xMin + m_xMax) / 2;
        m_yVal    = (m_xMin + m_xMax) / 2;
        m_invertY = false;

        // Add the circle
        m_circle.setColour (GUI::Color::Accent.overlaidWith (Colour (255, 255, 255).withAlpha (0.15f)));
        m_circle.setInterceptsMouseClicks (false, false);
        addAndMakeVisible (m_circle);

        // Initialize the sliders
        m_xAxisSlider = new Slider (x_parameterLabel);
        m_yAxisSlider = new Slider (y_parameterLabel);

        // Attach the sliders to the AudioProcessorValueTreeState
        m_xAxisSliderAttachment = new AudioProcessorValueTreeState::SliderAttachment (stateToControl, x_parameterID, *m_xAxisSlider);
        m_yAxisSliderAttachment = new AudioProcessorValueTreeState::SliderAttachment (stateToControl, y_parameterID, *m_yAxisSlider);

        // Define the range for the sliders
        m_xAxisSlider->setRange (m_xMin, m_xMax);
        m_xAxisSlider->setName (getName() + "_x");
        m_yAxisSlider->setRange (m_yMin, m_yMax);
        m_yAxisSlider->setName (getName() + "_y");

        // Start the timer callback
        startTimer (Constants::GuiRefreshTimer);
    }

    ~PadXY() {}

    void paint (Graphics& g) override
    {
        int borderWidth = 0;
        int cornerSize  = 4;

        // Background
        m_rectangle.setPosition (borderWidth / 2, borderWidth / 2);
        m_rectangle.setSize (getWidth() - borderWidth, getHeight() - borderWidth);
        g.setColour (Colours::transparentBlack);
        g.fillRoundedRectangle (m_rectangle, cornerSize);

        // Draw borders
        GUI::Paint::drawBorders (g, getLocalBounds(), GUI::Paint::BorderType::Glass);
    }

    void resized() override
    {
        // Update the background rectangle
        m_rectangle.setWidth (getWidth() * .98);
        m_rectangle.setHeight (getHeight() * .85);
        m_rectangle.setTop (getHeight() * .02);
        m_rectangle.setLeft (getWidth() * .02);

        // Update the circle
        float circle_diameter = getWidth() * 0.065;
        m_circle.setSize (circle_diameter, circle_diameter);
        const juce::Point<int> pos (getValueAsPosition (m_xVal, m_yVal));
        m_circle.setTopLeftPosition (pos.getX(), pos.getY());
    }

    void setValues (float x, float y, bool notify = true)
    {
        m_xAxisSlider->setValue (x, sendNotification);
        m_yAxisSlider->setValue (y, sendNotification);
    }

    class PadCircle : public Component
    {
        juce::Point<float> circleXY;
        Colour             colour;

    public:
        PadCircle() {}

        void setColour (Colour col)
        {
            colour = col;
            repaint();
        }

        void paint (Graphics& g) override
        {
            g.fillAll (Colours::transparentBlack);
            g.setColour (colour);

            Rectangle<int> cirlce_bounds = getLocalBounds();

            float line_thickness = cirlce_bounds.getWidth() * 0.2f;
            float circle_x       = cirlce_bounds.getX() + (line_thickness / 2.0f);
            float circle_y       = cirlce_bounds.getY() + (line_thickness / 2.0f);
            float circle_w       = cirlce_bounds.getWidth() - line_thickness;
            float circle_h       = cirlce_bounds.getHeight() - line_thickness;

            g.drawEllipse (circle_x, circle_y, circle_w, circle_h, line_thickness);
        }
    };

    PadCircle* getCircle()
    {
        return &m_circle;
    }

private:
    juce::Point<int> constrainPosition (float x, float y)
    {
        const float xPos = jlimit (m_rectangle.getX(), (m_rectangle.getWidth() + m_rectangle.getX()) - m_circle.getWidth(), x - m_circle.getWidth() / 2.f);
        const float yPos = jlimit (m_rectangle.getY(), (m_rectangle.getHeight() + m_rectangle.getY()) - m_circle.getHeight(), y - m_circle.getHeight() / 2.f);
        return juce::Point<int> (xPos, yPos);
    }

    juce::Point<int> getValueAsPosition (float x, float y)
    {
        if (m_invertY)
            y = m_yMax - y;

        const float xPos = jmap (x, m_rectangle.getX(), (m_rectangle.getWidth() + m_rectangle.getX()) - m_circle.getWidth());
        const float yPos = jmap (y, m_rectangle.getY(), (m_rectangle.getHeight() + m_rectangle.getY()) - m_circle.getHeight());
        return juce::Point<int> (xPos, yPos);
    }

    void setPositionAsValue (juce::Point<float> position)
    {
        const float xVal = jlimit (m_xMin, m_xMax, jmap (position.getX(), m_rectangle.getX(), m_rectangle.getWidth() - m_circle.getWidth(), m_xMin, m_xMax));
        float       yVal = jlimit (m_yMin, m_yMax, jmap (position.getY(), m_rectangle.getY(), m_rectangle.getHeight() - m_circle.getHeight(), m_yMin, m_yMax));

        if (m_invertY)
            yVal = m_yMax - yVal;

        setValues (xVal, yVal);
    }

    void mouseDown (const MouseEvent& e) override
    {
        m_circle.setTopLeftPosition (constrainPosition (e.getPosition().getX(), e.getPosition().getY()));
        m_mouseDownXY.setXY (m_circle.getPosition().getX() + m_circle.getWidth() * .5f, m_circle.getPosition().getY() + m_circle.getHeight() * .5f);
        setPositionAsValue (m_circle.getPosition().toFloat());
        repaint();
    }

    void mouseDrag (const MouseEvent& e) override
    {
        if (e.mouseWasDraggedSinceMouseDown())
        {
            m_circle.setTopLeftPosition (constrainPosition (m_mouseDownXY.getX() + e.getDistanceFromDragStartX(), m_mouseDownXY.getY() + e.getDistanceFromDragStartY()));
            setPositionAsValue (m_circle.getPosition().toFloat());
            repaint();

            m_currentMouseXY = m_circle.getPosition().toFloat();
        }
    }

    void timerCallback() override
    {
        // Check if a preset has been loaded
        if (m_currentPresetName != m_processor.getPresetManager().getCurrentPresetName())
        {
            // Update the labels
            setValues (m_xAxisSlider->getValue(), m_yAxisSlider->getValue());

            juce::Point<int> new_position (getValueAsPosition (m_xAxisSlider->getValue(),
                                                               m_yAxisSlider->getValue()));

            m_circle.setTopLeftPosition (new_position.getX(), new_position.getY());

            // Repaint the component
            repaint();

            // Save the current preset name
            m_currentPresetName = m_processor.getPresetManager().getCurrentPresetName();
        }
    }

    int mParameterID;

    PadCircle m_circle;

    Rectangle<float> m_rectangle;

    float m_xMin, m_xMax, m_yMin, m_yMax, m_xVal, m_yVal;
    bool  m_invertY;
    bool  m_rightMouseButtonDown = false;

    juce::Point<float> m_currentMouseXY;
    juce::Point<float> m_mouseDownXY;

    ScopedPointer<Slider> m_xAxisSlider;
    ScopedPointer<Slider> m_yAxisSlider;

    ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> m_xAxisSliderAttachment;
    ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> m_yAxisSliderAttachment;

    String m_currentPresetName;

    MorphexAudioProcessor& m_processor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PadXY)
};
