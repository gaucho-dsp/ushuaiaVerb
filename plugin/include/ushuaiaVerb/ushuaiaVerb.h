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
    double getTailLengthSeconds() const override { return 2.0; } //default rev tail of 2s

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













#endif //USHUAIAVERB_H



