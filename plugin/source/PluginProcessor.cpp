#include "ushuaiaVerb/PluginProcessor.h"
#include "ushuaiaVerb/PluginEditor.h"
#include "ushuaiaVerb/ushuaiaVerbProc.h"
#include "ushuaiaVerb/ushuaiaVerb.h"
#define JucePlugin_Name "UshuaiaVerb"

//==============================================================================
juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;
    
    //reverb parameters
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        "size", "Size", 
        juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f), 0.5f));
    
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        "damping", "Damping", 
        juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f), 0.5f));
    
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        "width", "Width", 
        juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f), 1.0f));
    
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        "predelay", "PreDelay", 
        juce::NormalisableRange<float>(0.0f, 200.0f, 1.0f), 0.0f));
    
    //tone parameters
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        "lowcut", "Low Cut", 
        juce::NormalisableRange<float>(20.0f, 500.0f, 1.0f, 0.3f), 20.0f));
    
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        "highcut", "High Cut", 
        juce::NormalisableRange<float>(1000.0f, 20000.0f, 1.0f, 0.3f), 12000.0f));
    
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        "lowshelf", "Low Shelf", 
        juce::NormalisableRange<float>(-12.0f, 12.0f, 0.1f), 0.0f));
    
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        "highshelf", "High Shelf", 
        juce::NormalisableRange<float>(-12.0f, 12.0f, 0.1f), 0.0f));
    
    //output parameters
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        "inputgain", "Input Gain", 
        juce::NormalisableRange<float>(-24.0f, 24.0f, 0.1f), 0.0f));
    
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        "outputgain", "Output Gain", 
        juce::NormalisableRange<float>(-24.0f, 24.0f, 0.1f), 0.0f));
    
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        "mix", "Mix", 
        juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f), 0.5f));
    
    return layout;
}

//==============================================================================
AudioPluginAudioProcessor::AudioPluginAudioProcessor()
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
       parameters(*this, nullptr, juce::Identifier("UshuaiaVerbParameters"), createParameterLayout())
{
    //initialise our UshuaiaVerb instance
    reverbProcessor = std::make_unique<ushuaiaVerbAudioProcessor>();
    
    //get parameter pointers
    sizeParam = parameters.getRawParameterValue("size");
    dampingParam = parameters.getRawParameterValue("damping");
    widthParam = parameters.getRawParameterValue("width");
    predelayParam = parameters.getRawParameterValue("predelay");
    lowcutParam = parameters.getRawParameterValue("lowcut");
    highcutParam = parameters.getRawParameterValue("highcut");
    lowshelfParam = parameters.getRawParameterValue("lowshelf");
    highshelfParam = parameters.getRawParameterValue("highshelf");
    inputgainParam = parameters.getRawParameterValue("inputgain");
    outputgainParam = parameters.getRawParameterValue("outputgain");
    mixParam = parameters.getRawParameterValue("mix");
}

AudioPluginAudioProcessor::~AudioPluginAudioProcessor() = default;

//==============================================================================
const juce::String AudioPluginAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool AudioPluginAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool AudioPluginAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool AudioPluginAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double AudioPluginAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int AudioPluginAudioProcessor::getNumPrograms()
{
    return 1;
}

int AudioPluginAudioProcessor::getCurrentProgram()
{
    return 0;
}

void AudioPluginAudioProcessor::setCurrentProgram (int index)
{
    juce::ignoreUnused (index);
}

const juce::String AudioPluginAudioProcessor::getProgramName (int index)
{
    juce::ignoreUnused (index);
    return {};
}

void AudioPluginAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
    juce::ignoreUnused (index, newName);
}

//==============================================================================
void AudioPluginAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    //initialise our reverb processor
    if (reverbProcessor)
        reverbProcessor->prepareToPlay(sampleRate, samplesPerBlock);
}

void AudioPluginAudioProcessor::releaseResources()
{
}

bool AudioPluginAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}

void AudioPluginAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer,
                                              juce::MidiBuffer& midiMessages)
{
    juce::ignoreUnused (midiMessages);
    juce::ScopedNoDenormals noDenormals;

    const int totalNumInputChannels  = getTotalNumInputChannels();
    const int totalNumOutputChannels = getTotalNumOutputChannels();

    // clear output channels not receiving input
    for (int i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    //use our actual reverb processor
    if (reverbProcessor && totalNumInputChannels >= 2 && totalNumOutputChannels >= 2)
    {
        //update reverb parameters
        reverbProcessor->setMixValue(*mixParam);
        
        //process the audio
        juce::MidiBuffer tempMidi;
        reverbProcessor->processBlock(buffer, tempMidi);
    }
}

//==============================================================================
bool AudioPluginAudioProcessor::hasEditor() const
{
    return true;
}

juce::AudioProcessorEditor* AudioPluginAudioProcessor::createEditor()
{
    return new AudioPluginAudioProcessorEditor (*this);
}

//==============================================================================
void AudioPluginAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    auto state = parameters.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void AudioPluginAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));
    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName(parameters.state.getType()))
            parameters.replaceState(juce::ValueTree::fromXml(*xmlState));
}

//==============================================================================
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new AudioPluginAudioProcessor();
}
