//
// Created for UshuaiaVerb
// Adapted from Maim's LookAndFeel
//

#ifndef USHUAIAVERB_USHUAIALOOKANDFEEL_H
#define USHUAIAVERB_USHUAIALOOKANDFEEL_H

#pragma once

#include <juce_graphics/juce_graphics.h>
#include <juce_gui_basics/juce_gui_basics.h>
#include "UshuaiaColours.h"

class UshuaiaLookAndFeel : public juce::LookAndFeel_V4
{
public:
    UshuaiaLookAndFeel();
    ~UshuaiaLookAndFeel() override;

    void drawRotarySlider (juce::Graphics& g,
        int x,
        int y,
        int width,
        int height,
        float sliderPos,
        const float rotaryStartAngle,
        const float rotaryEndAngle,
        juce::Slider&) override;

    void drawLinearSlider(juce::Graphics& g,
        int x,
        int y,
        int width,
        int height,
        float sliderPos,
        float minSliderPos,
        float maxSliderPos,
        const juce::Slider::SliderStyle style,
        juce::Slider& s
        ) override;

    void drawToggleButton(juce::Graphics& g, juce::ToggleButton& button, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override;



private:
    const float pi = 3.14159265359f;
};

#endif //USHUAIAVERB_USHUAIALOOKANDFEEL_H