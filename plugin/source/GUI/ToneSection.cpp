//
// Created for UshuaiaVerb
// Tone/EQ controls section
//

#include "ushuaiaVerb/GUI/ToneSection.h"

ToneSection::ToneSection(juce::AudioProcessorValueTreeState& p)
    : lowCutSlider(p, "lowcut", "Low Cut")
    , highCutSlider(p, "highcut", "High Cut")
    , lowShelfSlider(p, "lowshelf", "Low Shelf")
    , highShelfSlider(p, "highshelf", "High Shelf")
    , sliders{&lowCutSlider, &highCutSlider, &lowShelfSlider, &highShelfSlider}
{
    for (auto* slider : sliders)
    {
        addAndMakeVisible(slider);
    }
}

ToneSection::~ToneSection()
{
}

void ToneSection::paint(juce::Graphics& g)
{
    StageWindow::paint(g);
    
    g.setColour(juce::Colours::darkblue);
    g.setFont(juce::Font(16.0f).boldened());
    g.drawText("TONE", getLocalBounds().withHeight(25).reduced(5), juce::Justification::centred);
}

void ToneSection::resized()
{
    auto bounds = getLocalBounds().reduced(10).withTrimmedTop(25);
    int sliderWidth = 70;
    int sliderHeight = 90;
    int spacing = 15;  // Increased spacing for better distribution
    
    int totalWidth = 4 * sliderWidth + 3 * spacing;
    int xStart = bounds.getCentreX() - totalWidth / 2;
    
    for (int i = 0; i < sliders.size(); ++i)
    {
        int x = xStart + i * (sliderWidth + spacing);
        sliders[i]->setBounds(x, bounds.getY(), sliderWidth, sliderHeight);
    }
}