#include "ushuaiaVerb/PluginProcessor.h"
#include "ushuaiaVerb/PluginEditor.h"

//==============================================================================
AudioPluginAudioProcessorEditor::AudioPluginAudioProcessorEditor (AudioPluginAudioProcessor& p)
    : AudioProcessorEditor (&p)
    , audioProcessor(p)
    , mainArea(p.getValueTreeState())
{
    //set look and feel
    setLookAndFeel(&ushuaiaLookAndFeel);
    
    //add and make visible the main area
    addAndMakeVisible(mainArea);
    
    //set the plugin window size (800x500)
    setSize(800, 500);
    
    //make the window resizable
    setResizable(false, false);
}

AudioPluginAudioProcessorEditor::~AudioPluginAudioProcessorEditor()
{
    setLookAndFeel(nullptr);
}

//==============================================================================
void AudioPluginAudioProcessorEditor::paint (juce::Graphics& g)
{
    //fill da background
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
}

void AudioPluginAudioProcessorEditor::resized()
{
    mainArea.setBounds(getLocalBounds());
}
