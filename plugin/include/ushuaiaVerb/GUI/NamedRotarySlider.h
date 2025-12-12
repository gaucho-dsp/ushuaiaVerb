//
// Created for UshuaiaVerb
// Adapted from Maim
//

#ifndef USHUAIAVERB_NAMEDROTARYSLIDER_H
#define USHUAIAVERB_NAMEDROTARYSLIDER_H

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_graphics/juce_graphics.h>

#include "UshuaiaLookAndFeel.h"

class NamedRotarySlider : public juce::Component
{
public:
    NamedRotarySlider(juce::AudioProcessorValueTreeState& p, const juce::String& parameterName, const juce::String& displayName);
    ~NamedRotarySlider();
    
    void resized() override;
    void paint(juce::Graphics &g) override;
    void setFontSize(float size);
    
    juce::Slider slider {
        juce::Slider::RotaryVerticalDrag,
        juce::Slider::TextBoxBelow
    };
    
private:
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> attachment;
    const juce::String displayName;
    int nameHeight = 22;
    juce::Font nameFont;
    UshuaiaLookAndFeel lookAndFeel;
};

#endif //USHUAIAVERB_NAMEDROTARYSLIDER_H