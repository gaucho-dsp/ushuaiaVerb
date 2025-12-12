//
// Created for UshuaiaVerb
// Title panel with plugin name and info
//

#include "ushuaiaVerb/GUI/TitlePanel.h"

TitlePanel::TitlePanel()
{
}

TitlePanel::~TitlePanel()
{
}

void TitlePanel::paint(juce::Graphics& g)
{
    StageWindow::paint(g);
    
    auto bounds = getLocalBounds().reduced(10);
    
    // Draw title
    g.setColour(juce::Colours::darkblue);
    g.setFont(juce::Font(28.0f).boldened());
    g.drawText("USHUAIA", bounds.removeFromTop(35), juce::Justification::centred);
    
    // Draw subtitle
    g.setColour(juce::Colours::darkblue);
    g.setFont(juce::Font(18.0f));
    g.drawText("VERB", bounds.removeFromTop(25), juce::Justification::centred);
    
    // Draw version/info
    g.setColour(juce::Colours::darkgrey);
    g.setFont(juce::Font(11.0f));
    g.drawText("5x5 Householder Lattice", bounds.removeFromTop(20), juce::Justification::centred);
    g.drawText("gaucho dsp", bounds.removeFromTop(15), juce::Justification::centred);
    g.drawText("v1.0.0", bounds, juce::Justification::centred);
}

void TitlePanel::resized()
{
    // Nothing specific to resize
}