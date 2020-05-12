/*
  ==============================================================================

    SMTHelperFunctions.h
    Created: 14 Jun 2019 8:52:15pm
    Author:  Marc Sanchez Martinez

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"

#include "InterfaceDefines.h"

#include "SMTConstants.h"

//#include "SMTParameterSlider.h"

inline void paintComponentLabel(Graphics& g, Component* inComponent)
{
    // Initialize variables
    int x = 0;
    int y = 0;
    int w = 0;
    int h = 0;
    
//    // Check if component is SMTParameterSlider
//    if (SMTParameterSlider* slider = dynamic_cast<SMTParameterSlider*> (inComponent))
//    {
//        Slider::SliderStyle slider_style = slider->getSliderStyle();
//        
//        if (    slider_style == Slider::Rotary ||
//                slider_style == Slider::RotaryVerticalDrag ||
//                slider_style == Slider::RotaryHorizontalDrag ||
//                slider_style == Slider::RotaryHorizontalVerticalDrag    )
//        {
//            x = inComponent->getX() - (inComponent->getWidth() * 0.25f);
//            y = inComponent->getY() + inComponent->getHeight();
//            w = inComponent->getWidth() * 1.5f;
//            h = 20;
//        }
//        else
//        {
//            x = inComponent->getX() - (inComponent->getWidth() * 0.25f);
//            y = inComponent->getY() - inComponent->getHeight();
//            w = inComponent->getWidth() * 1.5f;
//            h = 20;
//        }
//    }
//    else
//    {
//        // TODO - Not reached yet
//    }
    
    const String label = inComponent->getName();
    
    g.setColour(SMTColour_Text);
    g.setFont(font_2);
    
    g.drawFittedText(label, x, y, w, h, Justification::centred, 1);
}

// Method to compare which one is the more close.
// We find the closest by taking the difference
// between the target and both values. It assumes
// that val2 is greater than val1 and target lies
// between these two.
inline float getClosest(float val1, float val2, float target)
{
    if (target - val1 >= val2 - target)
        return val2;
    else
        return val1;
}

// Returns element closest to target in arr[]
inline float findClosest(float arr[], int n, float target)
{
    // Corner cases
    if (target <= arr[0])
        return arr[0];
    if (target >= arr[n - 1])
        return arr[n - 1];
    
    // Doing binary search
    int i = 0, j = n, mid = 0;
    while (i < j) {
        mid = (i + j) / 2;
        
        if (arr[mid] == target)
            return arr[mid];
        
        // If target is less than array element, then search in left
        if (target < arr[mid]) {
            
            // If target is greater than previous to mid, return closest of two
            if (mid > 0 && target > arr[mid - 1])
                return getClosest(arr[mid - 1], arr[mid], target);
            
            // Repeat for left half
            j = mid;
        }
        
        // If target is greater than mid
        else {
            if (mid < n - 1 && target < arr[mid + 1])
                return getClosest(arr[mid], arr[mid + 1], target);
            // update i
            i = mid + 1;
        }
    }
    
    // Only single element left after search
    return arr[mid];
}

template<typename T>

inline std::vector<T> getRange(T start, T end) {
    size_t N = (int)floor(end - start) + 1;
    std::vector<T> vec(N);
    std::iota(vec.begin(), vec.end(), start);
    return vec;
}

/*
 * Remove First Occurrence of given  substring from main string.
 */
inline void removeSubStr(std::string & mainStr, const std::string & toRemove)
{
    // Search for the substring in string
    size_t pos = mainStr.find(toRemove);
    
    if (pos != std::string::npos)
    {
        // If found then remove it from string
        mainStr.erase(pos, toRemove.length());
    }
}
