//
// Created by Facundo Franchino on 03/02/2025.
//
#ifndef USHUAIAVERB_H
#define USHUAIAVERB_H

#include <juce_audio_processors/juce_audio_processors.h>
#include <set>
#include <string>
#include <cmath>
#include <array>
#include <vector>

enum {
  mixParam=0, //mix (wet/dry) control
  numParameters=1 //number of parameters in the plugin
};

const int predelay=1014;
const int vlfpredelay=11000;

//short delay times (early reflections)
const int shortA = 78; const int shortB = 760;
const int shortC = 982; const int shortD = 528;
const int shortE = 445; const int shortF = 1128;
const int shortG = 130; const int shortH = 708;
const int shortI = 22; const int shortJ = 2144;
const int shortK = 354; const int shortL = 1169;
const int shortM = 11; const int shortN = 2782;
const int shortO = 58; const int shortP = 1515;

//long delay times (reverb tank)
const int delayA = 871; const int delayB = 1037;
const int delayC = 1205; const int delayD = 297;
const int delayE = 467; const int delayF = 884;
const int delayG = 173; const int delayH = 1456;
const int delayI = 799; const int delayJ = 361;
const int delayK = 1432; const int delayL = 338;
const int delayM = 186; const int delayN = 1408;
const int delayO = 1014; const int delayP = 23;
const int delayQ = 807; const int delayR = 501;
const int delayS = 1468; const int delayT = 1102;
const int delayU = 11; const int delayV = 1119;
const int delayW = 1315; const int delayX = 94; const int delayY = 1270;

const int uNumPrograms= 0; 
const int uNumInputs=2; 
const int uNumOutputs=2; 
const unsigned long uUniqueId= 'ushv'; 

class ushuaiaVerbAudioProcessor : public juce::AudioProcessor {
  public:
    ushuaiaVerbAudioProcessor();
    ~ushuaiaVerbAudioProcessor() override;

    //standard JUCE methods
    const juce::String getName() const override;
    bool acceptsMidi() const override { return false; }
    bool producesMidi() const override { return false; }
    bool isMidiEffect() const override { return false; }
    double getTailLengthSeconds() const override { return 2.0; } 
    bool isBusesLayoutSupported(const BusesLayout& layouts) const override;

    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
    void processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) override;
    
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //helper for parameter access in DSP function
    float getParameterValue(int index) const {
        if (index == mixParam) return mixValue; //use our mix value
        return 0.0f;
    }
    
    void setMixValue(float mix) { mixValue = mix; }

    //param /state methods
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void *data, int sizeInBytes) override;
    void setParameterValue(int index, float value);
    
    //program methods
    const juce::String getProgramName(int index) override { juce::ignoreUnused(index); return {}; }
    void changeProgramName(int index, const juce::String& newName) override { juce::ignoreUnused(index, newName); }
    int getNumPrograms() override { return 1; }
    int getCurrentProgram() override { return 0; }
    void setCurrentProgram(int index) override { juce::ignoreUnused(index); }
    
    //legacy /extra methods
    void getProgramName(char* name);
    void setProgramName(char* name);
    
    //VST2/compatibility helpers
    int canDo(char* text);
    int getVendorVersion() const;
    void setProgramName(const juce::String& newName);
    const juce::String getProgramName();
    bool getEffectName(char* name);
    bool getProductString(char* text);
    bool getVendorString(char* text);

// =============================================================================
//pub data members (required for processUshuaiaVerb access)
// =============================================================================
    
    //gain
    double gainOutL;
    double gainOutR;

    //early reflection buffers
    double eAL[shortA+5], eBL[shortB+5], eCL[shortC+5], eDL[shortD+5];
    double eEL[shortE+5], eFL[shortF+5], eGL[shortG+5], eHL[shortH+5];
    double eIL[shortI+5], eJL[shortJ+5], eKL[shortK+5], eLL[shortL+5];
    double eML[shortM+5], eNL[shortN+5], eOL[shortO+5], ePL[shortP+5];
    
    double eAR[shortA+5], eBR[shortB+5], eCR[shortC+5], eDR[shortD+5];
    double eER[shortE+5], eFR[shortF+5], eGR[shortG+5], eHR[shortH+5];
    double eIR[shortI+5], eJR[shortJ+5], eKR[shortK+5], eLR[shortL+5];
    double eMR[shortM+5], eNR[shortN+5], eOR[shortO+5], ePR[shortP+5];

    // ER counters
    int shortAL, shortBL, shortCL, shortDL, shortEL, shortFL, shortGL, shortHL;
    int shortIL, shortJL, shortKL, shortLL, shortML, shortNL, shortOL, shortPL;
    int shortAR, shortBR, shortCR, shortDR, shortER, shortFR, shortGR, shortHR;
    int shortIR, shortJR, shortKR, shortLR, shortMR, shortNR, shortOR, shortPR;

    //main tank buffers
    double aAL[delayA+5], aBL[delayB+5], aCL[delayC+5], aDL[delayD+5], aEL[delayE+5];
    double aFL[delayF+5], aGL[delayG+5], aHL[delayH+5], aIL[delayI+5], aJL[delayJ+5];
    double aKL[delayK+5], aLL[delayL+5], aML[delayM+5], aNL[delayN+5], aOL[delayO+5];
    double aPL[delayP+5], aQL[delayQ+5], aRL[delayR+5], aSL[delayS+5], aTL[delayT+5];
    double aUL[delayU+5], aVL[delayV+5], aWL[delayW+5], aXL[delayX+5], aYL[delayY+5];

    double aAR[delayA+5], aBR[delayB+5], aCR[delayC+5], aDR[delayD+5], aER[delayE+5];
    double aFR[delayF+5], aGR[delayG+5], aHR[delayH+5], aIR[delayI+5], aJR[delayJ+5];
    double aKR[delayK+5], aLR[delayL+5], aMR[delayM+5], aNR[delayN+5], aOR[delayO+5];
    double aPR[delayP+5], aQR[delayQ+5], aRR[delayR+5], aSR[delayS+5], aTR[delayT+5];
    double aUR[delayU+5], aVR[delayV+5], aWR[delayW+5], aXR[delayX+5], aYR[delayY+5];

    //predelay buffers
    double aZL[predelay+5], aZR[predelay+5]; 
    double aVLFL[vlfpredelay+5], aVLFR[vlfpredelay+5]; 

    //feedback
    double feedbackAL, feedbackBL, feedbackCL, feedbackDL, feedbackEL;
    double feedbackER, feedbackJR, feedbackOR, feedbackTR, feedbackYR;

    double lastRefL[7], lastRefR[7];

    //tank counters
    int countAL, countBL, countCL, countDL, countEL, countFL, countGL, countHL;
    int countIL, countJL, countKL, countLL, countML, countNL, countOL, countPL;
    int countQL, countRL, countSL, countTL, countUL, countVL, countWL, countXL, countYL;

    int countAR, countBR, countCR, countDR, countER, countFR, countGR, countHR;
    int countIR, countJR, countKR, countLR, countMR, countNR, countOR, countPR;
    int countQR, countRR, countSR, countTR, countUR, countVR, countWR, countXR, countYR;

    int countZ, countVLF, cycle;

    enum {
        pear_total = 40
    };

    double pearsonFilter[pear_total] = {0.0}; 
    
    static constexpr int pearStages = 5; 
    std::vector<double> pearsonFilters[pearStages] = {
         std::vector<double>(pear_total), std::vector<double>(pear_total),
         std::vector<double>(pear_total), std::vector<double>(pear_total),
         std::vector<double>(pear_total)
    };

    double vibratoL = 0.0, vibratoR = 0.0;
    double vibA_L = 0.0, vibA_R = 0.0, vibB_L = 0.0, vibB_R = 0.0;
    double vib_L = 0.0; // Added missing state var from kCathedral2 logic

    double subL_A = 0.0, subR_A = 0.0, subL_B = 0.0, subR_B = 0.0, subL_C = 0.0, subR_C = 0.0;
    double subBufferL_A = 0.0, subBufferR_A = 0.0, subBufferL_B = 0.0, subBufferR_B = 0.0, subBufferL_C = 0.0, subBufferR_C = 0.0;

    uint32_t fpdL, fpdR;
    std::unique_ptr<juce::AudioProcessorValueTreeState> parameters;

  private:
    juce::String programName;
    std::set<std::string> _canDo;
    float mixValue = 0.5f;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ushuaiaVerbAudioProcessor)
};

#endif //USHUAIAVERB_H