//
// Created for UshuaiaVerb
// Adapted from Maim
//

#include "ushuaiaVerb/GUI/StageWindow.h"

StageWindow::StageWindow()
{
}

StageWindow::~StageWindow()
{
}

void StageWindow::paint(juce::Graphics& g)
{
    auto bounds = getLocalBounds();
    
    // Draw background
    g.setColour(UshuaiaColours::PANEL_BACKGROUND_COLOR);
    g.fillRect(bounds);
    
    // Draw bevel frame
    drawBevel(g, bounds, false);
}

void StageWindow::drawBevel(juce::Graphics& g, juce::Rectangle<int> bounds, bool raised)
{
    // Outer edge
    g.setColour(raised ? UshuaiaColours::BEVEL_WHITE : UshuaiaColours::BEVEL_BLACK);
    g.drawLine(bounds.getX(), bounds.getY(), bounds.getRight() - 1, bounds.getY(), 1.0f);
    g.drawLine(bounds.getX(), bounds.getY(), bounds.getX(), bounds.getBottom() - 1, 1.0f);
    
    g.setColour(raised ? UshuaiaColours::BEVEL_BLACK : UshuaiaColours::BEVEL_WHITE);
    g.drawLine(bounds.getRight() - 1, bounds.getY(), bounds.getRight() - 1, bounds.getBottom() - 1, 1.0f);
    g.drawLine(bounds.getX(), bounds.getBottom() - 1, bounds.getRight() - 1, bounds.getBottom() - 1, 1.0f);
    
    // Inner edge
    bounds = bounds.reduced(1);
    g.setColour(raised ? UshuaiaColours::BEVEL_LIGHT : UshuaiaColours::BEVEL_DARK);
    g.drawLine(bounds.getX(), bounds.getY(), bounds.getRight() - 1, bounds.getY(), 1.0f);
    g.drawLine(bounds.getX(), bounds.getY(), bounds.getX(), bounds.getBottom() - 1, 1.0f);
    
    g.setColour(raised ? UshuaiaColours::BEVEL_DARK : UshuaiaColours::BEVEL_LIGHT);
    g.drawLine(bounds.getRight() - 1, bounds.getY(), bounds.getRight() - 1, bounds.getBottom() - 1, 1.0f);
    g.drawLine(bounds.getX(), bounds.getBottom() - 1, bounds.getRight() - 1, bounds.getBottom() - 1, 1.0f);
}