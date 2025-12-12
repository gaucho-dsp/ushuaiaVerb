//
// Created for UshuaiaVerb
// Tone/EQ controls section
//

#ifndef USHUAIAVERB_TONESECTION_H
#define USHUAIAVERB_TONESECTION_H

#include <juce_audio_processors/juce_audio_processors.h>
#include "StageWindow.h"
#include "NamedRotarySlider.h"

class ToneSection : public StageWindow
{
public:
    ToneSection(juce::AudioProcessorValueTreeState& p);
    ~ToneSection();
    
    void paint(juce::Graphics& g) override;
    void resized() override;
    
private:
    NamedRotarySlider lowCutSlider;
    NamedRotarySlider highCutSlider;
    NamedRotarySlider lowShelfSlider;
    NamedRotarySlider highShelfSlider;
    
    std::array<NamedRotarySlider*, 4> sliders;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ToneSection)
};

#endif //USHUAIAVERB_TONESECTION_H