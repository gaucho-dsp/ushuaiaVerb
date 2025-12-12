//
// Created for UshuaiaVerb
// Main area combining all GUI sections
//

#include "ushuaiaVerb/GUI/MainArea.h"

MainArea::MainArea(juce::AudioProcessorValueTreeState& p)
    : reverbSection(p)
    , toneSection(p)
    , outputSection(p)
    , parameters(p)
{
    addAndMakeVisible(titlePanel);
    addAndMakeVisible(reverbSection);
    addAndMakeVisible(toneSection);
    addAndMakeVisible(outputSection);
}

MainArea::~MainArea()
{
}

void MainArea::paint(juce::Graphics& g)
{
    auto bounds = getLocalBounds();
    
    // Main background with gradient
    juce::ColourGradient backgroundGradient(
        UshuaiaColours::PANEL_BACKGROUND_COLOR.brighter(0.1f),
        bounds.getCentreX(), bounds.getY(),
        UshuaiaColours::PANEL_BACKGROUND_COLOR.darker(0.1f),
        bounds.getCentreX(), bounds.getBottom(),
        false);
    g.setGradientFill(backgroundGradient);
    g.fillAll();
    
    // Outer border with depth
    g.setColour(UshuaiaColours::BEVEL_BLACK);
    g.drawRect(bounds, 2);
    
    // Inner highlight for 3D effect
    g.setColour(UshuaiaColours::BEVEL_WHITE.withAlpha(0.2f));
    g.drawRect(bounds.reduced(2), 1);
    
    // Add some atmospheric pattern/texture (optional)
    g.setColour(UshuaiaColours::SPLASH_COLOR_DARK.withAlpha(0.03f));
    for (int i = 0; i < bounds.getWidth(); i += 40) {
        for (int j = 0; j < bounds.getHeight(); j += 40) {
            g.drawEllipse(i, j, 2, 2, 0.5f);
        }
    }
}

void MainArea::resized()
{
    int margin = 10;
    int outputSectionWidth = 110;
    
    auto activeArea = getLocalBounds()
        .withTrimmedTop(margin)
        .withTrimmedLeft(margin)
        .withTrimmedRight(margin)
        .withTrimmedBottom(margin);
    
    // Output section on the right
    outputSection.setBounds(activeArea.withWidth(outputSectionWidth).withRightX(activeArea.getRight()));
    
    // Main content area on the left
    auto tilesArea = activeArea.withTrimmedRight(outputSectionWidth + margin);
    
    // Top row
    auto topRow = tilesArea.withHeight(230);
    titlePanel.setBounds(topRow.withWidth(150));
    reverbSection.setBounds(topRow.withTrimmedLeft(titlePanel.getWidth() + margin));
    
    // Bottom row, tone section
    auto bottomRow = tilesArea.withTrimmedTop(topRow.getHeight() + margin);
    toneSection.setBounds(bottomRow);
}