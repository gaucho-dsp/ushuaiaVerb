//
// Created for UshuaiaVerb
// Main area combining all GUI sections
//

#ifndef USHUAIAVERB_MAINAREA_H
#define USHUAIAVERB_MAINAREA_H

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_graphics/juce_graphics.h>

#include "TitlePanel.h"
#include "ReverbSection.h"
#include "ToneSection.h"
#include "OutputSection.h"

class MainArea : public juce::Component
{
public:
    MainArea(juce::AudioProcessorValueTreeState& p);
    ~MainArea() override;
    
    void paint(juce::Graphics& g) override;
    void resized() override;
    
private:
    TitlePanel titlePanel;
    ReverbSection reverbSection;
    ToneSection toneSection;
    OutputSection outputSection;
    
    juce::AudioProcessorValueTreeState& parameters;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainArea)
};

#endif //USHUAIAVERB_MAINAREA_H