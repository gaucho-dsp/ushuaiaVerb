//
// Created by Facundo Franchino on 22/03/2025.
//
#include <JuceHeader.h>
#include "ushuaiaVerb/PluginProcessor.h"
#include "ushuaiaVerb.h"

void AudioPluginAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    const int numSamples = buffer.getNumSamples();
    const int numChannels = buffer.getNumChannels();
    jassert(numChannels == 2);//stereo only for now

    float* inL = buffer.getWritePointer(0);
    float* inR = buffer.getWritePointer(1);
    float* outL = buffer.getWritePointer(0);//in-place processing
    float* outR = buffer.getWritePointer(1);

    //=====sample rate scaling for cycle detection =====
    double overallScale = getSampleRate() / 44100.0;

    int cycleEnd = static_cast<int>(std::floor(overallScale));
    cycleEnd = std::clamp(cycleEnd, 1, 4);

    if (cycle > cycleEnd - 1)
        cycle = cycleEnd - 1;//safety clamp

    //=====pre-delay and VLF delay in samples =====
    int adjPredelay   = predelaySamples;//derived from smoothed parameter
    int adjSubDelay   = vlfPredelaySamples;//same idea


