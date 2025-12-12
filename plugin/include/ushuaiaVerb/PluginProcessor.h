#pragma once

#include <juce_audio_processors/juce_audio_processors.h>

//==============================================================================
// Function to create the parameter layout
juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

//==============================================================================
//this is the main processor class for the ushuaia plugin

class AudioPluginAudioProcessor final : public juce::AudioProcessor
{
public:
    //==============================================================================
    AudioPluginAudioProcessor();
    ~AudioPluginAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;
    using AudioProcessor::processBlock;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    // Get the value tree state for GUI
    juce::AudioProcessorValueTreeState& getValueTreeState() { return parameters; }

private:
    //==============================================================================
    // Parameter management
    juce::AudioProcessorValueTreeState parameters;
    
    // Parameter pointers for quick access
    std::atomic<float>* sizeParam = nullptr;
    std::atomic<float>* dampingParam = nullptr;
    std::atomic<float>* widthParam = nullptr;
    std::atomic<float>* predelayParam = nullptr;
    std::atomic<float>* lowcutParam = nullptr;
    std::atomic<float>* highcutParam = nullptr;
    std::atomic<float>* lowshelfParam = nullptr;
    std::atomic<float>* highshelfParam = nullptr;
    std::atomic<float>* inputgainParam = nullptr;
    std::atomic<float>* outputgainParam = nullptr;
    std::atomic<float>* mixParam = nullptr;
    
    //internal dsp state
    int cycle = 0;
    int predelaySamples = 0;
    int vlfPredelaySamples = 0;
    
    // UshuaiaVerb processor
    std::unique_ptr<class ushuaiaVerbAudioProcessor> reverbProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioPluginAudioProcessor)
};
