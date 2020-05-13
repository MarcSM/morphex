/*
 ==============================================================================
 
 TopPanel.h
 Created: 12 May 2020 11:18:41am
 Author:  Marc Sanchez Martinez
 
 ==============================================================================
 */

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
 */
class TopPanel    : public Component
{
public:
    TopPanel()
    {
        addAndMakeVisible (rightPanel);
        addAndMakeVisible (leftPanel);
        addAndMakeVisible (centerPanel);
    }
    
    ~TopPanel()
    {
    }
    
    void paint (Graphics& g) override
    {
        
    }
    
    void resized() override
    {
        // This method is where you should set the bounds of any child
        // components that your component contains..
        
        FlexBox fb;
        
        float centerPanelWidth = getWidth() / 3.0f;
        float soundPanelWidth = ( getWidth() - centerPanelWidth ) / 2.0f;
        
        FlexItem left  (soundPanelWidth, getHeight(), leftPanel);
        FlexItem center  (centerPanelWidth, getHeight(), centerPanel);
        FlexItem right (soundPanelWidth, getHeight(), rightPanel);
        
        fb.items.addArray ( { left, center, right } );
        fb.performLayout (getLocalBounds().toFloat());
    }
    
private:
    
    struct LeftSidePanel    : public Component
    {
        LeftSidePanel ()
        {
            // Morphex Logo
            //morphex_long_logo = ImageCache::getFromMemory(BinaryData::morphex_long_logo_svg, BinaryData::morphex_long_logo_svgSize);
            morphex_long_logo = Drawable::createFromImageData (BinaryData::morphex_long_logo_svg, BinaryData::morphex_long_logo_svgSize);
            //addAndMakeVisible (morphex_long_logo.get());
            //morphex_long_logo->setBounds (0,0,100,100);
            
            //morphex_long_logo_bounds = new Rectangle<float>(10, 10, getWidth(), getHeight());
            //morphex_long_logo_bounds->setBounds(10, 10, 100, 100);

            morphex_long_logo_margin = 15.0f;
        }
        
        void paint (Graphics& g) override
        {
            // g.drawImageAt(morphex_long_logo, morphex_long_logo_margin, morphex_long_logo_margin);
            
            
//            auto tr = morphex_long_logo->getTransformToScaleToFit(getLocalBounds().toFloat().reduced(3.5), false, Justification::centred);
//            morphex_long_logo.applyTransform(tr);
            
//            auto area = getLocalBounds();
//            morphex_long_logo->setBounds(area);
            //morphex_long_logo->draw(g, 1.f, AffineTransform());
            
//            juce::Rectangle<float> morphex_long_logo_bounds(10, 10, 100, 100);
            
//            g.drawRect(morphex_long_logo_bounds.expanded(1.f), 1.f);
            // svg file:
//            morphex_long_logo->drawWithin(g, morphex_long_logo_bounds, RectanglePlacement::stretchToFit, 1.f);
            
//            juce::Rectangle<float> picBounds(10, 10, getWidth(), getHeight());
            juce::Rectangle<float> picBounds(morphex_long_logo_margin, morphex_long_logo_margin,
                                             getWidth() - 2 * morphex_long_logo_margin,
                                             getHeight() - 2 * morphex_long_logo_margin);
            juce::Rectangle<float> svgBounds =
            ((DrawableComposite*) morphex_long_logo.get())->getContentArea();
            juce::RectanglePlacement ourPlacement(RectanglePlacement::xLeft);
//            juce::RectanglePlacement ourPlacement(RectanglePlacement::stretchToFit);
            juce::AffineTransform xform = ourPlacement.getTransformToFit (svgBounds, picBounds);
            morphex_long_logo->draw(g, 1.f, xform);
            
            // drawToFit(g, morphex_long_logo, morphex_long_logo_bounds);
        }
        
//        static void drawToFit (juce::Graphics& g, juce::Drawable& svg, juce::Rectangle<float> targetArea)
//        {
//            svg.draw (g, 1.0f,
//                      juce::RectanglePlacement (juce::RectanglePlacement::stretchToFit)
//                      .getTransformToFit ((std::unique_ptr<DrawableComposite>) svg).getContentArea(), targetArea));
//        }
        
//        static void drawToFit (juce::Graphics& g, juce::DrawableComposite& svg, juce::Rectangle<float> targetArea)
//        {
//            svg.draw (g, 1.0f,
//                      juce::RectanglePlacement (juce::RectanglePlacement::stretchToFit)
//                      .getTransformToFit (svg.getContentArea(), targetArea));
//        }
        
        void resized() override
        {
            //morphex_long_logo_bounds->setBounds(10, 10, 100, 100);
            
            //auto area = getLocalBounds();
            //morphex_long_logo->setBounds(0,0,100,100);
            
            /*
            morphex_long_logo = morphex_long_logo.rescaled (getWidth() - 2 * morphex_long_logo_margin - 130,
                                                            getHeight() - 2 * morphex_long_logo_margin,
                                                            Graphics::mediumResamplingQuality);
            */
        }
        
        std::unique_ptr<Drawable> morphex_long_logo;
        //Rectangle<float>* morphex_long_logo_bounds;
//        std::unique_ptr<Rectangle<float>> morphex_long_logo_bounds;
//        Image morphex_long_logo;
        float morphex_long_logo_margin;
    };
    
    struct RightSidePanel    : public Component
    {
        RightSidePanel ()
        {
            
        }
        
        void paint (Graphics& g) override
        {
            /*
             g.setColour (Colours::grey);
             g.drawRect (getLocalBounds(), 1);   // draw an outline around the component
             
             g.setColour (Colours::white);
             g.setFont (14.0f);
             g.drawText ("Options", getLocalBounds(),
             Justification::centred, true);   // draw some placeholder
             */
        }
        
        void resized() override
        {
            
        }
    };
    
    
    struct CenterPanel    : public Component
    {
        CenterPanel()
        {
            
        }
        
        void paint (Graphics& g) override
        {
            g.fillAll (GUI::Color::BrowserBackground);
            
            //g.setColour (Colours::grey);
            //g.drawRect (getLocalBounds(), 1);   // draw an outline around the component
            
            g.setColour (Colours::white);
            g.setFont (14.0f);
            g.drawText ("Preset Manager", getLocalBounds(),
                        Justification::centred, true);   // draw some placeholder text
        }
        
        void resized() override
        {
            
        }
    };
    
    LeftSidePanel leftPanel;
    RightSidePanel rightPanel;
    CenterPanel centerPanel;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TopPanel)
};
