/*
  ==============================================================================

    SMTPanelBase.cpp
    Created: 14 Jun 2019 7:38:37pm
    Author:  Marc Sanchez Martinez

  ==============================================================================
*/

#include "SMTPanelBase.h"
#include "SMTInterfaceDefines.h"

SMTPanelBase::SMTPanelBase(SpectralMorphingToolAudioProcessor* inProcessor)
//:   mProcessor(inProcessor)
{
//    mProcessor = inProcessor;
}

SMTPanelBase::~SMTPanelBase()
{
    
}

void SMTPanelBase::paint(Graphics& g)
{
    g.setColour(Colours::black);
    g.drawRoundedRectangle(0, 0, getWidth(), getHeight(), 5, 5);
}
