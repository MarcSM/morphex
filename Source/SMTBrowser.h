/*
  ==============================================================================

    SMTFileBrowser.h
    Created: 13 Jul 2019 11:16:49am
    Author:  Marc Sanchez Martinez

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"

#include "SMTFileBrowser.h"
#include "SMTSoundPanel.h"

class SMTBrowser:
    public Component
{
    Rectangle<float> rectangle;
    
    Point<float> currentMouseXY;
    Point<float> mouseDownXY;
    
public:
    
    SMTBrowser(SpectralMorphingToolAudioProcessor* inProcessor);
    
    ~SMTBrowser();
    
    void paint(Graphics& g) override;
    
    void resized() override;
    
    void fileClicked (const File&, const MouseEvent&);
    
private:
    
    String mCollectionsDirectory;
    String mPluginDirectory;

    ScopedPointer<SMTFileBrowser> m_fileBrowser;
    ScopedPointer<WildcardFileFilter> m_wcFileFilter;
};
