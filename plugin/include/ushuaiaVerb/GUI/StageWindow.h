//
// Created for UshuaiaVerb
// Adapted from Maim
//

#ifndef USHUAIAVERB_STAGEWINDOW_H
#define USHUAIAVERB_STAGEWINDOW_H

#include <juce_graphics/juce_graphics.h>
#include <juce_gui_basics/juce_gui_basics.h>
#include "UshuaiaColours.h"

class StageWindow : public juce::Component
{
public:
    StageWindow();
    ~StageWindow() override;
    
    void paint(juce::Graphics& g) override;
    
protected:
    void drawBevel(juce::Graphics& g, juce::Rectangle<int> bounds, bool raised = true);
    
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(StageWindow)
};

#endif //USHUAIAVERB_STAGEWINDOW_H