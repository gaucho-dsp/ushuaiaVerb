//
// Created for UshuaiaVerb
// Main reverb controls section
//

#include "ushuaiaVerb/GUI/ReverbSection.h"

ReverbSection::ReverbSection(juce::AudioProcessorValueTreeState& p)
    : sizeSlider(p, "size", "Size")
    , dampingSlider(p, "damping", "Damping")
    , widthSlider(p, "width", "Width")
    , predelaySlider(p, "predelay", "PreDelay")
    , sliders{&sizeSlider, &dampingSlider, &widthSlider, &predelaySlider}
{
    for (auto* slider : sliders)
    {
        addAndMakeVisible(slider);
    }
}

ReverbSection::~ReverbSection()
{
}

void ReverbSection::paint(juce::Graphics& g)
{
    StageWindow::paint(g);
    
    g.setColour(juce::Colours::darkblue);
    g.setFont(juce::Font(16.0f).boldened());
    g.drawText("REVERB", getLocalBounds().withHeight(25).reduced(5), juce::Justification::centred);
}

void ReverbSection::resized()
{
    auto bounds = getLocalBounds().reduced(10).withTrimmedTop(25);
    int sliderWidth = 70;
    int sliderHeight = 90;
    int spacing = 15;
    
    // Calculate total width needed for 4 sliders in a row
    int totalWidth = 4 * sliderWidth + 3 * spacing;
    int xStart = bounds.getCentreX() - totalWidth / 2;
    
    // Arrange all 4 sliders in a single row, centered
    for (int i = 0; i < sliders.size(); ++i)
    {
        int x = xStart + i * (sliderWidth + spacing);
        sliders[i]->setBounds(x, bounds.getY(), sliderWidth, sliderHeight);
    }
}