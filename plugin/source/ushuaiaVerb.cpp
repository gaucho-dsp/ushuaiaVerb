//
// Created by Facundo Franchino on 07/03/2025.
//

#include "ushuaiaVerb.h"

//constructor
ushuaiaVerbAudioProcessor::ushuaiaVerbAudioProcessor()
    : AudioProcessor(BusesProperties()
        .withInput("Input", juce::AudioChannelSet::stereo(), true)
        .withOutput("Output", juce::AudioChannelSet::stereo(), true))
{
  //gain values initialisation
  gainOutL=gainOutR=1.0;

  //now clear all early reflection delay buffers (left and right)
  std::fill(std::begin(eAL), std::end(eAL), 0.0);
  std::fill(std::begin(eAR), std::end(eAR), 0.0);
  std::fill(std::begin(eBL), std::end(eBL), 0.0);
  std::fill(std::begin(eBR), std::end(eBR), 0.0);
  std::fill(std::begin(eCL), std::end(eCL), 0.0);
  std::fill(std::begin(eCR), std::end(eCR), 0.0);
  std::fill(std::begin(eDL), std::end(eDL), 0.0);
  std::fill(std::begin(eDR), std::end(eDR), 0.0);
  std::fill(std::begin(eEL), std::end(eEL), 0.0);
  std::fill(std::begin(eER), std::end(eER), 0.0);
  std::fill(std::begin(eFL), std::end(eFL), 0.0);
  std::fill(std::begin(eFR), std::end(eFR), 0.0);
  std::fill(std::begin(eGL), std::end(eGL), 0.0);
  std::fill(std::begin(eGR), std::end(eGR), 0.0);
  std::fill(std::begin(eHL), std::end(eHL), 0.0);
  std::fill(std::begin(eHR), std::end(eHR), 0.0);
  std::fill(std::begin(eIL), std::end(eIL), 0.0);
  std::fill(std::begin(eIR), std::end(eIR), 0.0);
  std::fill(std::begin(eJL), std::end(eJL), 0.0);
  std::fill(std::begin(eJR), std::end(eJR), 0.0);
  std::fill(std::begin(eKL), std::end(eKL), 0.0);
  std::fill(std::begin(eKR), std::end(eKR), 0.0);
  std::fill(std::begin(eLL), std::end(eLL), 0.0);
  std::fill(std::begin(eLR), std::end(eLR), 0.0);
  std::fill(std::begin(eML), std::end(eML), 0.0);
  std::fill(std::begin(eMR), std::end(eMR), 0.0);
  std::fill(std::begin(eNL), std::end(eNL), 0.0);
  std::fill(std::begin(eNR), std::end(eNR), 0.0);
  std::fill(std::begin(eOL), std::end(eOL), 0.0);
  std::fill(std::begin(eOR), std::end(eOR), 0.0);
  std::fill(std::begin(ePL), std::end(ePL), 0.0);
  std::fill(std::begin(ePR), std::end(ePR), 0.0);

  //initialise buffer positions for early reflection delay lines (left and right)
  shortAL = shortBL = shortCL = shortDL = shortEL = shortFL = shortGL = shortHL = 1;
  shortIL = shortJL = shortKL = shortLL = shortML = shortNL = shortOL = shortPL = 1;
  shortAR = shortBR = shortCR = shortDR = shortER = shortFR = shortGR = shortHR = 1;
  shortIR = shortJR = shortKR = shortLR = shortMR = shortNR = shortOR = shortPR = 1;
}

//initialise all delay line buffers used for the main rev tail and predelays

//clc all main rev delay buffers (left and right)
std::fill(std::begin(aAL), std::end(aAL), 0.0);
std::fill(std::begin(aAR), std::end(aAR), 0.0);
std::fill(std::begin(aBL), std::end(aBL), 0.0);
std::fill(std::begin(aBR), std::end(aBR), 0.0);
std::fill(std::begin(aCL), std::end(aCL), 0.0);
std::fill(std::begin(aCR), std::end(aCR), 0.0);
std::fill(std::begin(aDL), std::end(aDL), 0.0);
std::fill(std::begin(aDR), std::end(aDR), 0.0);
std::fill(std::begin(aEL), std::end(aEL), 0.0);
std::fill(std::begin(aER), std::end(aER), 0.0);
std::fill(std::begin(aFL), std::end(aFL), 0.0);
std::fill(std::begin(aFR), std::end(aFR), 0.0);
std::fill(std::begin(aGL), std::end(aGL), 0.0);
std::fill(std::begin(aGR), std::end(aGR), 0.0);
std::fill(std::begin(aHL), std::end(aHL), 0.0);
std::fill(std::begin(aHR), std::end(aHR), 0.0);
std::fill(std::begin(aIL), std::end(aIL), 0.0);
std::fill(std::begin(aIR), std::end(aIR), 0.0);
std::fill(std::begin(aJL), std::end(aJL), 0.0);
std::fill(std::begin(aJR), std::end(aJR), 0.0);
std::fill(std::begin(aKL), std::end(aKL), 0.0);
std::fill(std::begin(aKR), std::end(aKR), 0.0);
std::fill(std::begin(aLL), std::end(aLL), 0.0);
std::fill(std::begin(aLR), std::end(aLR), 0.0);
std::fill(std::begin(aML), std::end(aML), 0.0);
std::fill(std::begin(aMR), std::end(aMR), 0.0);
std::fill(std::begin(aNL), std::end(aNL), 0.0);
std::fill(std::begin(aNR), std::end(aNR), 0.0);
std::fill(std::begin(aOL), std::end(aOL), 0.0);
std::fill(std::begin(aOR), std::end(aOR), 0.0);
std::fill(std::begin(aPL), std::end(aPL), 0.0);
std::fill(std::begin(aPR), std::end(aPR), 0.0);
std::fill(std::begin(aQL), std::end(aQL), 0.0);
std::fill(std::begin(aQR), std::end(aQR), 0.0);
std::fill(std::begin(aRL), std::end(aRL), 0.0);
std::fill(std::begin(aRR), std::end(aRR), 0.0);
std::fill(std::begin(aSL), std::end(aSL), 0.0);
std::fill(std::begin(aSR), std::end(aSR), 0.0);
std::fill(std::begin(aTL), std::end(aTL), 0.0);
std::fill(std::begin(aTR), std::end(aTR), 0.0);
std::fill(std::begin(aUL), std::end(aUL), 0.0);
std::fill(std::begin(aUR), std::end(aUR), 0.0);
std::fill(std::begin(aVL), std::end(aVL), 0.0);
std::fill(std::begin(aVR), std::end(aVR), 0.0);
std::fill(std::begin(aWL), std::end(aWL), 0.0);
std::fill(std::begin(aWR), std::end(aWR), 0.0);
std::fill(std::begin(aXL), std::end(aXL), 0.0);
std::fill(std::begin(aXR), std::end(aXR), 0.0);
std::fill(std::begin(aYL), std::end(aYL), 0.0);
std::fill(std::begin(aYR), std::end(aYR), 0.0);


//clc predelay buffers
std::fill(std::begin(aZL), std::end(aZL), 0.0);
std::fill(std::begin(aZR), std::end(aZR), 0.0);

//clc very low freq predelay buffers
std::fill(std::begin(aVLFL), std::end(aVLFL), 0.0);
std::fill(std::begin(aVLFR), std::end(aVLFR), 0.0);