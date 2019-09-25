/*
  =============================================================================

    SMTBrowser.cp
    Created: 13 Jul 2019 11:16:49a
    Author:  Marc Sanchez Martine

  =============================================================================
*/

#include "SMTBrowser.h"

#include "SMTConstants.h"

SMTBrowser::SMTBrowser(SpectralMorphingToolAudioProcessor* inProcessor)
{
    if (!File(PLUGIN_DATA_COLLECTIONS_DIRECTORY).exists())
    {
        File(PLUGIN_DATA_COLLECTIONS_DIRECTORY).createDirectory();
    }
    
    int flags = SMTFileBrowser::openMode |
                SMTFileBrowser::canSelectFiles |
                SMTFileBrowser::filenameBoxIsReadOnly;
    
    m_wcFileFilter = new WildcardFileFilter("*.had", "*", "somedescription");

    // Create the browser component
    m_fileBrowser = new SMTFileBrowser(inProcessor, flags, PLUGIN_DATA_COLLECTIONS_DIRECTORY, m_wcFileFilter, NULL);
    
    // Add browser compoenent
    addAndMakeVisible(m_fileBrowser);
}

SMTBrowser::~SMTBrowser()
{
   
}

void SMTBrowser::paint(Graphics& g)
{
    m_fileBrowser->setTopLeftPosition(0, 0);
    m_fileBrowser->setSize(getWidth(), getHeight());
}

void SMTBrowser::resized()
{
    rectangle.setWidth (getWidth()*.98);
    rectangle.setHeight (getHeight()*.85);
    rectangle.setTop (getHeight()*.02);
    rectangle.setLeft (getWidth()*.02);
}
