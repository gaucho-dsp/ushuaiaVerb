//
// Created by Facundo Franchino on 07/03/2025.
//

#include "ushuaiaVerb/ushuaiaVerb.h"
#include "ushuaiaVerb/ushuaiaVerbProc.h"
#include <cstring>

ushuaiaVerbAudioProcessor::ushuaiaVerbAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
    : AudioProcessor(BusesProperties()
                     .withInput("Input", juce::AudioChannelSet::stereo(), true)
                     .withOutput("Output", juce::AudioChannelSet::stereo(), true)
                     )
#endif
{
    programName = "Default";
    gainOutL = gainOutR = 1.0;
}

ushuaiaVerbAudioProcessor::~ushuaiaVerbAudioProcessor()
{
}

const juce::String ushuaiaVerbAudioProcessor::getName() const
{
    return "UshuaiaVerb";
}


void ushuaiaVerbAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    juce::ignoreUnused(sampleRate, samplesPerBlock);
    //clc all delay buffers
    std::fill(std::begin(eAL), std::end(eAL), 0.0); std::fill(std::begin(eAR), std::end(eAR), 0.0);
    std::fill(std::begin(eBL), std::end(eBL), 0.0); std::fill(std::begin(eBR), std::end(eBR), 0.0);
    std::fill(std::begin(eCL), std::end(eCL), 0.0); std::fill(std::begin(eCR), std::end(eCR), 0.0);
    std::fill(std::begin(eDL), std::end(eDL), 0.0); std::fill(std::begin(eDR), std::end(eDR), 0.0);
    std::fill(std::begin(eEL), std::end(eEL), 0.0); std::fill(std::begin(eER), std::end(eER), 0.0);
    std::fill(std::begin(eFL), std::end(eFL), 0.0); std::fill(std::begin(eFR), std::end(eFR), 0.0);
    std::fill(std::begin(eGL), std::end(eGL), 0.0); std::fill(std::begin(eGR), std::end(eGR), 0.0);
    std::fill(std::begin(eHL), std::end(eHL), 0.0); std::fill(std::begin(eHR), std::end(eHR), 0.0);
    std::fill(std::begin(eIL), std::end(eIL), 0.0); std::fill(std::begin(eIR), std::end(eIR), 0.0);
    std::fill(std::begin(eJL), std::end(eJL), 0.0); std::fill(std::begin(eJR), std::end(eJR), 0.0);
    std::fill(std::begin(eKL), std::end(eKL), 0.0); std::fill(std::begin(eKR), std::end(eKR), 0.0);
    std::fill(std::begin(eLL), std::end(eLL), 0.0); std::fill(std::begin(eLR), std::end(eLR), 0.0);
    std::fill(std::begin(eML), std::end(eML), 0.0); std::fill(std::begin(eMR), std::end(eMR), 0.0);
    std::fill(std::begin(eNL), std::end(eNL), 0.0); std::fill(std::begin(eNR), std::end(eNR), 0.0);
    std::fill(std::begin(eOL), std::end(eOL), 0.0); std::fill(std::begin(eOR), std::end(eOR), 0.0);
    std::fill(std::begin(ePL), std::end(ePL), 0.0); std::fill(std::begin(ePR), std::end(ePR), 0.0);

    //initialise delay pointers
    shortAL = shortBL = shortCL = shortDL = shortEL = shortFL = shortGL = shortHL = 1;
    shortIL = shortJL = shortKL = shortLL = shortML = shortNL = shortOL = shortPL = 1;
    shortAR = shortBR = shortCR = shortDR = shortER = shortFR = shortGR = shortHR = 1;
    shortIR = shortJR = shortKR = shortLR = shortMR = shortNR = shortOR = shortPR = 1;

    // Clear main tank buffers
    std::fill(std::begin(aAL), std::end(aAL), 0.0); std::fill(std::begin(aAR), std::end(aAR), 0.0);
    // ... (repeat for all aBL..aYL arrays, truncated for brevity but needed in full code) ...
    // wee note, in real production code, use a helper function or loop if possible, 
    // but with named arrays must list them.
    std::fill(std::begin(aBL), std::end(aBL), 0.0); std::fill(std::begin(aBR), std::end(aBR), 0.0);
    std::fill(std::begin(aCL), std::end(aCL), 0.0); std::fill(std::begin(aCR), std::end(aCR), 0.0);
    std::fill(std::begin(aDL), std::end(aDL), 0.0); std::fill(std::begin(aDR), std::end(aDR), 0.0);
    std::fill(std::begin(aEL), std::end(aEL), 0.0); std::fill(std::begin(aER), std::end(aER), 0.0);
    std::fill(std::begin(aFL), std::end(aFL), 0.0); std::fill(std::begin(aFR), std::end(aFR), 0.0);
    std::fill(std::begin(aGL), std::end(aGL), 0.0); std::fill(std::begin(aGR), std::end(aGR), 0.0);
    std::fill(std::begin(aHL), std::end(aHL), 0.0); std::fill(std::begin(aHR), std::end(aHR), 0.0);
    std::fill(std::begin(aIL), std::end(aIL), 0.0); std::fill(std::begin(aIR), std::end(aIR), 0.0);
    std::fill(std::begin(aJL), std::end(aJL), 0.0); std::fill(std::begin(aJR), std::end(aJR), 0.0);
    std::fill(std::begin(aKL), std::end(aKL), 0.0); std::fill(std::begin(aKR), std::end(aKR), 0.0);
    std::fill(std::begin(aLL), std::end(aLL), 0.0); std::fill(std::begin(aLR), std::end(aLR), 0.0);
    std::fill(std::begin(aML), std::end(aML), 0.0); std::fill(std::begin(aMR), std::end(aMR), 0.0);
    std::fill(std::begin(aNL), std::end(aNL), 0.0); std::fill(std::begin(aNR), std::end(aNR), 0.0);
    std::fill(std::begin(aOL), std::end(aOL), 0.0); std::fill(std::begin(aOR), std::end(aOR), 0.0);
    std::fill(std::begin(aPL), std::end(aPL), 0.0); std::fill(std::begin(aPR), std::end(aPR), 0.0);
    std::fill(std::begin(aQL), std::end(aQL), 0.0); std::fill(std::begin(aQR), std::end(aQR), 0.0);
    std::fill(std::begin(aRL), std::end(aRL), 0.0); std::fill(std::begin(aRR), std::end(aRR), 0.0);
    std::fill(std::begin(aSL), std::end(aSL), 0.0); std::fill(std::begin(aSR), std::end(aSR), 0.0);
    std::fill(std::begin(aTL), std::end(aTL), 0.0); std::fill(std::begin(aTR), std::end(aTR), 0.0);
    std::fill(std::begin(aUL), std::end(aUL), 0.0); std::fill(std::begin(aUR), std::end(aUR), 0.0);
    std::fill(std::begin(aVL), std::end(aVL), 0.0); std::fill(std::begin(aVR), std::end(aVR), 0.0);
    std::fill(std::begin(aWL), std::end(aWL), 0.0); std::fill(std::begin(aWR), std::end(aWR), 0.0);
    std::fill(std::begin(aXL), std::end(aXL), 0.0); std::fill(std::begin(aXR), std::end(aXR), 0.0);
    std::fill(std::begin(aYL), std::end(aYL), 0.0); std::fill(std::begin(aYR), std::end(aYR), 0.0);

    //clc auxiliary buffers
    std::fill(std::begin(aZL), std::end(aZL), 0.0);
    std::fill(std::begin(aZR), std::end(aZR), 0.0);
    std::fill(std::begin(aVLFL), std::end(aVLFL), 0.0);
    std::fill(std::begin(aVLFR), std::end(aVLFR), 0.0);

    //initialise tank counters
    countAL=countBL=countCL=countDL=countEL=countFL=countGL=countHL=1;
    countIL=countJL=countKL=countLL=countML=countNL=countOL=countPL=1;
    countQL=countRL=countSL=countTL=countUL=countVL=countWL=countXL=1;
    countYL=1;
    countAR=countBR=countCR=countDR=countER=countFR=countGR=countHR=1;
    countIR=countJR=countKR=countLR=countMR=countNR=countOR=countPR=1;
    countQR=countRR=countSR=countTR=countUR=countVR=countWR=countXR=1;
    countYR=1;

    countZ = 1;
    countVLF = 1;
    cycle = 0;

    //initialose state
    feedbackAL=feedbackBL=feedbackCL=feedbackDL=feedbackEL=0.0;
    feedbackER=feedbackJR=feedbackOR=feedbackTR=feedbackYR=0.0;
    
    //clc Pearson filters
    for (auto& stage : pearsonFilters) {
        std::fill(stage.begin(), stage.end(), 0.0);
    }
    std::fill(std::begin(pearsonFilter), std::end(pearsonFilter), 0.0);
    
    //clear output smoothing buffers
    std::fill(std::begin(lastRefL), std::end(lastRefL), 0.0);
    std::fill(std::begin(lastRefR), std::end(lastRefR), 0.0);
    
    //initialize vibrato state
    vibratoL = 0.0; vibratoR = M_PI_4; // Offset right channel
    vibA_L = vibA_R = vibB_L = vibB_R = vib_L = 0.0;
    
    //initialize subharmonic state
    subL_A = subR_A = subL_B = subR_B = subL_C = subR_C = 0.0;
    subBufferL_A = subBufferR_A = subBufferL_B = subBufferR_B = 0.0;
    subBufferL_C = subBufferR_C = 0.0;
    
    //initialize random seed for dither
    fpdL = 1.0; while (fpdL < 16386) fpdL = rand() * UINT32_MAX;
    fpdR = 1.0; while (fpdR < 16386) fpdR = rand() * UINT32_MAX;
}

void ushuaiaVerbAudioProcessor::releaseResources()
{
}

void ushuaiaVerbAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ignoreUnused(midiMessages);
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());

    //only process if we have stereo input/output
    if (totalNumInputChannels >= 2 && totalNumOutputChannels >= 2)
    {
        float* inL = buffer.getWritePointer(0);
        float* inR = buffer.getWritePointer(1);
        float* outL = buffer.getWritePointer(0);
        float* outR = buffer.getWritePointer(1);
        
        //pass *this reference so the function can see our delay buffers
        processUshuaiaVerb(*this, inL, inR, outL, outR, buffer.getNumSamples(), getSampleRate());
    }
}

//parameter Handling
void ushuaiaVerbAudioProcessor::setParameterValue(int id, float value) {
    juce::ignoreUnused(id, value);
    //placeholder
}

void ushuaiaVerbAudioProcessor::getStateInformation(juce::MemoryBlock& destData) {
    juce::ignoreUnused(destData);
}

void ushuaiaVerbAudioProcessor::setStateInformation(const void* data, int sizeInBytes) {
    juce::ignoreUnused(data, sizeInBytes);
}

// Additional JUCE methods
juce::AudioProcessorEditor* ushuaiaVerbAudioProcessor::createEditor() {
    return nullptr; // Or create a custom editor if needed
}

bool ushuaiaVerbAudioProcessor::hasEditor() const {
    return false; // Set to true if you have an editor
}

bool ushuaiaVerbAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const {
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;
    
    if (layouts.getMainInputChannelSet() != juce::AudioChannelSet::stereo())
        return false;
    
    return true;
}

// Legacy/VST2 compatibility methods
int ushuaiaVerbAudioProcessor::canDo(char* text) {
    if (_canDo.find(text) != _canDo.end()) return 1;
    return -1;
}

int ushuaiaVerbAudioProcessor::getVendorVersion() const { return 1000; }

void ushuaiaVerbAudioProcessor::setProgramName(const juce::String& newName) {
    programName = newName;
}

const juce::String ushuaiaVerbAudioProcessor::getProgramName() {
    return programName;
}

void ushuaiaVerbAudioProcessor::getProgramName(char* name) {
    std::strncpy(name, programName.toRawUTF8(), 64);
}

void ushuaiaVerbAudioProcessor::setProgramName(char* name) {
    programName = name;
}


bool ushuaiaVerbAudioProcessor::getEffectName(char* name) {
    std::strncpy(name, "UshuaiaVerb", 64);
    return true;
}

bool ushuaiaVerbAudioProcessor::getProductString(char* text) {
    std::strncpy(text, "ushuaiaVerb", 64);
    return true;
}

bool ushuaiaVerbAudioProcessor::getVendorString(char* text) {
    std::strncpy(text, "gaucho dsp", 64);
    return true;
}

