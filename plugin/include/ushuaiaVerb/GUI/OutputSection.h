//
// Created for UshuaiaVerb
// Output controls section (adapted from Maim's PostSection)
//

#ifndef USHUAIAVERB_OUTPUTSECTION_H
#define USHUAIAVERB_OUTPUTSECTION_H

#include <juce_audio_processors/juce_audio_processors.h>
#include "StageWindow.h"
#include "NamedRotarySlider.h"

class OutputSection : public StageWindow
{
public:
    OutputSection(juce::AudioProcessorValueTreeState& p);
    ~OutputSection();
    
    void paint(juce::Graphics& g) override;
    void resized() override;
    
private:
    NamedRotarySlider inputGainSlider;
    NamedRotarySlider outputGainSlider;
    NamedRotarySlider mixSlider;
    
    std::array<NamedRotarySlider*, 3> sliders;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(OutputSection)
};

#endif //USHUAIAVERB_OUTPUTSECTION_H