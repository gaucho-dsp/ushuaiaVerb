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