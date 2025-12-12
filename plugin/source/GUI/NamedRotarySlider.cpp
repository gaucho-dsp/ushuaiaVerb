//
// Created for UshuaiaVerb
// Adapted from Maim
//

#include "ushuaiaVerb/GUI/NamedRotarySlider.h"

NamedRotarySlider::NamedRotarySlider(juce::AudioProcessorValueTreeState& p, 
                                     const juce::String& parameterName, 
                                     const juce::String& displayName)
    : displayName(displayName)
{
    attachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(p, parameterName, slider);
    nameFont = juce::Font(juce::Font::getDefaultSansSerifFontName(), 12.0f, juce::Font::plain);
    
    //configure slider with our custom look, original Maim style
    slider.setLookAndFeel(&lookAndFeel);
    slider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 20);
    slider.setScrollWheelEnabled(false); // Prevent accidental scrolling
    
    addAndMakeVisible(slider);
}

NamedRotarySlider::~NamedRotarySlider()
{
}

void NamedRotarySlider::resized()
{
    auto bounds = getLocalBounds();
    auto sliderBounds = bounds.withTrimmedTop(nameHeight);
    slider.setBounds(sliderBounds);
}

void NamedRotarySlider::paint(juce::Graphics &g)
{
    auto bounds = getLocalBounds();
    auto nameBounds = bounds.removeFromTop(nameHeight);
    
    //draw label text, simple original style
    g.setColour(juce::Colours::black);
    g.setFont(nameFont);
    g.drawText(displayName, nameBounds, juce::Justification::centred, true);
}

//mouse events removed,back to simple original design

void NamedRotarySlider::setFontSize(float size)
{
    nameFont = juce::Font(juce::Font::getDefaultSansSerifFontName(), size, juce::Font::plain);
    repaint();
}