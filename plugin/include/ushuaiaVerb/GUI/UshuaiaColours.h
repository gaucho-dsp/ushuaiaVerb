//
// Created for UshuaiaVerb
// Color palette adapted from Maim
//

#ifndef USHUAIAVERB_USHUAIACOLOURS_H
#define USHUAIAVERB_USHUAIACOLOURS_H

#include <juce_graphics/juce_graphics.h>

namespace UshuaiaColours {
    // Main backgrounds
    const juce::Colour PANEL_BACKGROUND_COLOR = juce::Colour(0xffd4d0c8);  
    const juce::Colour CONTROL_BACKGROUND = juce::Colour(0xffd4d0c8);      // Same as panel
    const juce::Colour SECTION_BACKGROUND = juce::Colour(0xffd4d0c8);      // Same as panel
    
    // Bevel system - original Maim colors
    const juce::Colour BEVEL_WHITE = juce::Colour(0xffececec);     // Original Maim white
    const juce::Colour BEVEL_LIGHT = juce::Colour(0xffdfdfdf);     // Original Maim light
    const juce::Colour BEVEL_MEDIUM = juce::Colour(0xffc0c0c0);    // Medium grey
    const juce::Colour BEVEL_DARK = juce::Colour(0xff808080);      // Original Maim dark
    const juce::Colour BEVEL_BLACK = juce::Colour(0xff0a0a0a);     // Original Maim black
    
    // Original Maim accent colors
    const juce::Colour SPLASH_COLOR_DARK = juce::Colours::darkblue;    // Original Maim
    const juce::Colour SPLASH_COLOR_MEDIUM = juce::Colours::blue;       // Medium blue  
    const juce::Colour SPLASH_COLOR_LIGHT = juce::Colours::lightblue;   // Original Maim
    const juce::Colour SPLASH_COLOR_BRIGHT = juce::Colours::lightblue;  // Bright accent
    
    // Original Maim contrast colors
    const juce::Colour CONTRAST_COLOR_DARK = juce::Colours::darkorange;   // Original Maim
    const juce::Colour CONTRAST_COLOR_MEDIUM = juce::Colours::orange;     // Medium orange
    const juce::Colour CONTRAST_COLOR_LIGHT = juce::Colours::orange;      // Original Maim
    const juce::Colour CONTRAST_COLOR_BRIGHT = juce::Colours::orange;     // Bright accent
    
    // Text colors for bright theme
    const juce::Colour TEXT_COLOR_PRIMARY = juce::Colours::black;        // Black text
    const juce::Colour TEXT_COLOR_SECONDARY = juce::Colour(0xff404040);  // Dark grey text
    const juce::Colour TEXT_COLOR_DISABLED = juce::Colour(0xff808080);   // Disabled text
    
    // Special UI elements for bright theme
    const juce::Colour KNOB_TRACK_BACKGROUND = juce::Colour(0xffa0a0a0); // Light track
    const juce::Colour KNOB_TRACK_FILLED = SPLASH_COLOR_DARK;            // Blue active track
    const juce::Colour KNOB_SHADOW = juce::Colour(0x40000000);           // Light drop shadow
};

#endif //USHUAIAVERB_USHUAIACOLOURS_H