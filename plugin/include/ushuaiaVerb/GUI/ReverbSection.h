//
// Created for UshuaiaVerb
// Main reverb controls section
//

#ifndef USHUAIAVERB_REVERBSECTION_H
#define USHUAIAVERB_REVERBSECTION_H

#include <juce_audio_processors/juce_audio_processors.h>
#include "StageWindow.h"
#include "NamedRotarySlider.h"

class ReverbSection : public StageWindow
{
public:
    ReverbSection(juce::AudioProcessorValueTreeState& p);
    ~ReverbSection();
    
    void paint(juce::Graphics& g) override;
    void resized() override;
    
private:
    NamedRotarySlider sizeSlider;
    NamedRotarySlider dampingSlider;
    NamedRotarySlider widthSlider;
    NamedRotarySlider predelaySlider;
    
    std::array<NamedRotarySlider*, 4> sliders;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ReverbSection)
};

#endif //USHUAIAVERB_REVERBSECTION_H