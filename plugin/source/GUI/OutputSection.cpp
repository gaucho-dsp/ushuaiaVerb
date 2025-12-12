//
// Created for UshuaiaVerb
// Output controls section
//

#include "ushuaiaVerb/GUI/OutputSection.h"

OutputSection::OutputSection(juce::AudioProcessorValueTreeState& p)
    : inputGainSlider(p, "inputgain", "Input")
    , outputGainSlider(p, "outputgain", "Output")
    , mixSlider(p, "mix", "Mix")
    , sliders{&inputGainSlider, &outputGainSlider, &mixSlider}
{
    for (auto* slider : sliders)
    {
        addAndMakeVisible(slider);
    }
}

OutputSection::~OutputSection()
{
}

void OutputSection::paint(juce::Graphics& g)
{
    StageWindow::paint(g);
    
    g.setColour(UshuaiaColours::CONTRAST_COLOR_DARK);
    g.setFont(juce::Font(16.0f).boldened());
    g.drawText("OUTPUT", getLocalBounds().withHeight(25).reduced(5), juce::Justification::centred);
}

void OutputSection::resized()
{
    auto bounds = getLocalBounds().reduced(10).withTrimmedTop(30);
    int sliderHeight = 80;
    int spacing = 15;
    
    for (int i = 0; i < sliders.size(); ++i)
    {
        int y = bounds.getY() + i * (sliderHeight + spacing);
        sliders[i]->setBounds(bounds.getX(), y, bounds.getWidth(), sliderHeight);
    }
}