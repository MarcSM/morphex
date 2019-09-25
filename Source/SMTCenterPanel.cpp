/*
  ==============================================================================

    SMTCenterPanel.cpp
    Created: 14 Jun 2019 7:38:52pm
    Author:  Marc Sanchez Martinez

  ==============================================================================
*/

#include "SMTConstants.h"
#include "SMTCenterPanel.h"
#include "SMTParameters.h"
#include "SMTHelperFunctions.h"

SMTCenterPanel::SMTCenterPanel(SpectralMorphingToolAudioProcessor* inProcessor)
:   SMTPanelBase(inProcessor)
{
    setSize(CENTER_PANEL_WIDTH, CENTER_PANEL_HEIGHT);
    
    // Browser
    const int browser_margin = 10;
    
    const int browser_width = getWidth() - (browser_margin * 2);
    const int browser_height = 200 - (browser_margin * 2);
    
    const int browser_x = browser_margin;
    const int browser_y = browser_margin;
    
    
    mBrowser = new SMTBrowser(inProcessor);
    
    mBrowser->setBounds(browser_x, browser_y,
                        browser_width, browser_height);
    
    addAndMakeVisible(mBrowser);
    
    // Stochastic Parameters
    mAdditionalParameters = new SMTAdditionalParameters(inProcessor);
    
    // ADSR Parameters
    mADSRParameters = new SMTADSRParameters(inProcessor);
    
    // Tabbed Component
    mTabbedComponent = new SMTTabbedComponent (TabbedButtonBar::TabsAtTop);
    
    mTabbedComponent->setTabBarDepth(30);
    mTabbedComponent->addTab(TRANS("Sounds"), Colours::black, mBrowser, true);
    mTabbedComponent->addTab(TRANS("ADSR"), Colours::black, mADSRParameters, true);
    mTabbedComponent->addTab(TRANS("Stochastic"), Colours::black, mAdditionalParameters, true);
    mTabbedComponent->setCurrentTabIndex(0);
    
    mTabbedComponent->setBounds (browser_x, browser_y,
                                 browser_width, browser_height);
    
    addAndMakeVisible(mTabbedComponent);
    
    
    // XY Pad
    const int pad_margin = 10;
    
    const int pad_width = getWidth() - (pad_margin * 2);
    const int pad_height = getWidth() - (pad_margin * 2);
    
    const int pad_x = pad_margin;
    const int pad_y = browser_y + browser_height + pad_margin;
    
    mXYPad = new SMTXYPad(mProcessor,
                          mProcessor->parameters,
                          SMTParameterID[SMTParameter::kParameter_freqs_interp_factor],
                          SMTParameterLabel[SMTParameter::kParameter_freqs_interp_factor],
                          SMTParameterID[SMTParameter::kParameter_mags_interp_factor],
                          SMTParameterLabel[SMTParameter::kParameter_mags_interp_factor]);

    mXYPad->setBounds(pad_x, pad_y,
                      pad_width, pad_height);
    
    addAndMakeVisible(mXYPad);
    
    repaint();
}

SMTCenterPanel::~SMTCenterPanel()
{
    mBrowser = nullptr;
    mADSRParameters = nullptr;
    mAdditionalParameters = nullptr;
    mTabbedComponent = nullptr;
}

void SMTCenterPanel::paint (Graphics& g)
{
    SMTPanelBase::paint(g);
}
