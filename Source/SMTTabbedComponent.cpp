/*
  ==============================================================================

    SMTTabbedComponent.cpp
    Created: 11 Aug 2019 11:09:13am
    Author:  Sanchez Martinez, Marc

  ==============================================================================
*/

#include "SMTTabbedComponent.h"

SMTTabbedComponent::SMTTabbedComponent(TabbedButtonBar::Orientation orientation):
    TabbedComponent(orientation)
{
    
}

SMTTabbedComponent::~SMTTabbedComponent()
{
    
}
    
void SMTTabbedComponent::paint (Graphics& g)
{
    int borderWidth = 0;
    int cornerSize = 4;
    
    // Background
    Rectangle<float> rectangle;
    
    rectangle.setPosition(borderWidth / 2, borderWidth / 2);
    rectangle.setSize(getWidth() - borderWidth, getHeight() - borderWidth);
    
    g.setColour (Colours::black);
    g.fillRoundedRectangle (rectangle, cornerSize);
    
    // Text label
    g.setColour (Colours::white);
}
