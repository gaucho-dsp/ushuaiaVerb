//
// Created by Facundo Franchino on 03/02/2025.
//
#ifndef USHUAIAVERB_H
#define USHUAIAVERB_H

#include <set>
#include <string>
#include <math.h>

enum {
  mixParam=0, //mix (wet/dry) control
  numParameters=1 //number of parameters in the plugin
};

const int predelay=1014;//main pre-delay prior to reflections starting
const int vlfpredelay=1100;//bass reverb (VLF=very low frequencies) pre-delay to prevent excesssive low freq build up,for a clearer reverb

/*the following values will define short delay times (early reflections).
small and fast echoes prior to the main reverb tail happening.
shortX value is delay length in samples.
convrt it to milliseconds using time(ms)=(samples/44100)*1000
*/
const int shortA = 78; const int shortB = 760;
const int shortC = 982; const int shortD = 528;
const int shortE = 445; const int shortF = 1128;
const int shortG = 130; const int shortH = 708;
const int shortI = 22; const int shortJ = 2144;
const int shortK = 354; const int shortL = 1169;
const int shortM = 11; const int shortN = 2782;
const int shortO = 58; const int shortP = 1515;

/*the following values will define longer delay times
they spread out reflections over time to create a lush revb tail,
longer delays create sense of larger spaces and shape the rev decay
used in the main reverb tail, post-early reflections
delayX value is delay length in samples
convrt it to milliseconds using time(ms)=(samples/44100)*1000
*/

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

const int uNumPrograms= 0; //no presets for now
const int uNumInputs=2; //stereo in
const int uNumOutputs=2; //stereo out
const unsigned long uUniqueId= 'ushv'; //unique identifier for ushuaiaVerb


/*now let's declare the interface of the class, how the plugin interacts with the DAW
and how parameters, audio processing and metadata will be handled
*/

//class declaration
class ushuaiaVerbAudioProcessor :public juce::AudioProcessor{
  public:
    ushuaiaVerbAudioProcessor();
    ~ushuaiaVerbAudioProcessor() override;//destructor, it cleans up resources if needed

    //plugin emtadata
    const juce::String getName() const override;
    juce::AudioProcessor::BusesProperties getBusesProperties() const override;
    bool acceptsMidi() const override { return false; }
    bool producesMidi() const override { return false; }
    bool isMidiEffect() const override { return false; }
    double getTailLengthSeconds() const override {
      return 2.0; } //default rev tail of 2s

    //audio processing
    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
    void processBlock(juce::AudioBuffer<float>& buffer) override;

    //parametr handling
    float getParameter (int id) const override;
    void setParameter (int id, float value) override;

    //preset and state mgmt
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void *data, int sizeInBytes) override;

    //now onto writing the private member section of the class
  private:
    juce::String programName;
    double gainOutL;
    double gainOutR;

/*now lets store delay buffers in separate arrays
the size of each array is based on the corresponding shortX delay time
plus 5 extra samples to avoid buffer overflows. each array represents a delay line!
reverb processed audio by reading pasr samples from these bufffers.
L and R represent stereo processing obvs
*/

//separating buffers for L and R channels to create a wide stereo field
//stereo decorrelation, it creates a wider rev by making LR reflections slightly different
    //left channel early reflections
    double eAL[shortA+5]; //early reflection delay line A (L)
    double eBL[shortB+5]; //y asi sucesivamente
    double eCL[shortC+5];
    double eDL[shortD+5];
    double eEL[shortE+5];
    double eFL[shortF+5];
    double eGL[shortG+5];
    double eHL[shortH+5];
    double eIL[shortI+5];
    double eJL[shortJ+5];
    double eKL[shortK+5];
    double eLL[shortL+5];
    double eML[shortM+5];
    double eNL[shortN+5];
    double eOL[shortO+5];
    double ePL[shortP+5];

    //right channel early reflections
    double eAR[shortA+5]; //early reflection delay line A (R)
    double eBR[shortB+5]; //y de nuevo asi sucesviamente
    double eCR[shortC+5];
    double eDR[shortD+5];
    double eER[shortE+5];
    double eFR[shortF+5];
    double eGR[shortG+5];
    double eHR[shortH+5];
    double eIR[shortI+5];
    double eJR[shortJ+5];
    double eKR[shortK+5];
    double eLR[shortL+5];
    double eMR[shortM+5];
    double eNR[shortN+5];
    double eOR[shortO+5];
    double ePR[shortP+5];

//tracks buffer positions for early reflection delay lines L/R

//left
    int shortAL;
    int shortBL;
    int shortCL;
    int shortDL;
    int shortEL;
    int shortFL;
    int shortGL;
    int shortHL;
    int shortIL;
    int shortJL;
    int shortKL;
    int shortLL;
    int shortML;
    int shortNL;
    int shortOL;
    int shortPL;
//right
    int shortAR;
    int shortBR;
    int shortCR;
    int shortDR;
    int shortER;
    int shortFR;
    int shortGR;
    int shortHR;
    int shortIR;
    int shortJR;
    int shortKR;
    int shortLR;
    int shortMR;
    int shortNR;
    int shortOR;
    int shortPR;

//AGAIN-separating buffers for L and R channels to create a wide stereo field
//stereo decorrelation, it creates a wider rev by making LR reflections slightly different
//now instead of doing it for ER, now we do it for the reverb tail, otherwise it'd sound small and boxy
//this is the main reverb tail delay buffers LR, they control extended decay

    //left channel rev tail
    double aAL[delayA+5];
    double aBL[delayB+5];
    double aCL[delayC+5];
    double aDL[delayD+5];
    double aEL[delayE+5];
    double aFL[delayF+5];
    double aGL[delayG+5];
    double aHL[delayH+5];
    double aIL[delayI+5];
    double aJL[delayJ+5];
    double aKL[delayK+5];
    double aLL[delayL+5];
    double aML[delayM+5];
    double aNL[delayN+5];
    double aOL[delayO+5];
    double aPL[delayP+5];
    double aQL[delayQ+5];
    double aRL[delayR+5];
    double aSL[delayS+5];
    double aTL[delayT+5];
    double aUL[delayU+5];
    double aVL[delayV+5];
    double aWL[delayW+5];
    double aXL[delayX+5];
    double aYL[delayY+5];

    //right channel rev tail
    double aAR[delayA+5];
    double aBR[delayB+5];
    double aCR[delayC+5];
    double aDR[delayD+5];
    double aER[delayE+5];
    double aFR[delayF+5];
    double aGR[delayG+5];
    double aHR[delayH+5];
    double aIR[delayI+5];
    double aJR[delayJ+5];
    double aKR[delayK+5];
    double aLR[delayL+5];
    double aMR[delayM+5];
    double aNR[delayN+5];
    double aOR[delayO+5];
    double aPR[delayP+5];
    double aQR[delayQ+5];
    double aRR[delayR+5];
    double aSR[delayS+5];
    double aTR[delayT+5];
    double aUR[delayU+5];
    double aVR[delayV+5];
    double aWR[delayW+5];
    double aXR[delayX+5];
    double aYR[delayY+5];

    //lets also store pre delay buffers
    double aZL[predelay+5]; //left
    double aZR[predelay+5]; //right

    //and low freq reflections too
    double aVLFL[vlfpredelay+5]; //left
    double aVLFR[vlfpredelay+5]; //right

//now lets store feedback values for the rev tail and reflections
    //left
    double feedbackAL;
    double feedbackBL;
    double feedbackCL;
    double feedbackDL;
    double feedbackEL;

    //right
    double feedbackER;
    double feedbackJR;
    double feedbackOR;
    double feedbackTR;
    double feedbackYR;

    double lastRefL[7]; //store last 7 reflection samples for L channel
    double lastRefR[7]; //store last 7 reflection samples for R channel

//onto circular buffer position counters LR for rev delay lines

    int countAL;
    int countBL;
    int countCL;
    int countDL;
    int countEL;
    int countFL;
    int countGL;
    int countHL;
    int countIL;
    int countJL;
    int countKL;
    int countLL;
    int countML;
    int countNL;
    int countOL;
    int countPL;
    int countQL;
    int countRL;
    int countSL;
    int countTL;
    int countUL;
    int countVL;
    int countWL;
    int countXL;
    int countYL;

    //right
    int countAR;
    int countBR;
    int countCR;
    int countDR;
    int countER;
    int countFR;
    int countGR;
    int countHR;
    int countIR;
    int countJR;
    int countKR;
    int countLR;
    int countMR;
    int countNR;
    int countOR;
    int countPR;
    int countQR;
    int countRR;
    int countSR;
    int countTR;
    int countUR;
    int countVR;
    int countWR;
    int countXR;
    int countYR;

    int countZ; //pre-delay buffer position tracker
    int countVLF; //very low freq buffer pos tracker
    int cycle; //hw often processing updates values











#endif //USHUAIAVERB_H



