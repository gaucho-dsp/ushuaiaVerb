//
// Created by Facundo Franchino on 22/03/2025.
//

#include <juce_audio_processors/juce_audio_processors.h>
#include "ushuaiaVerb/ushuaiaVerb.h"
#include "ushuaiaVerb/ushuaiaVerbProc.h"
#include <cmath>

//constants needed for dsp logic (golden ratio and PI)
const double goldenRatio = 0.618033988749894848204586;
const double pear = 0.388;

//helper constants for logic that requires integer wrapping logic
//matches strict kCathedral2 behavior (s/o to chris johnson)
const int kCathedral_cycleMax = 4; 

//da engine
//changed the signature to accept 'ushuaiaVerbAudioProcessor& p' 
// so we can access the buffers declared in the header.
void processUshuaiaVerb(ushuaiaVerbAudioProcessor& p, float* inL, float* inR, float* outL, float* outR,
                        int numSamples, double sampleRate)
{
    //calculate cycle / oversampling
    double overallScale = sampleRate / 44100.0;
    int cycleEnd = static_cast<int>(std::floor(overallScale));
    if (cycleEnd < 1) cycleEnd = 1;
    if (cycleEnd > 4) cycleEnd = 4;

    if (p.cycle > cycleEnd - 1) p.cycle = cycleEnd - 1;

    // Pearson filter scaling based on sample rate
    double pearScaled = (pear * 0.388) / (double)cycleEnd;

    //fetch parameters
    // (assuming ID 0 is mix as per enum in header)
    double wet = p.getParameterValue(mixParam) * 2.0;
    double dry = 2.0 - wet;
    if (wet > 1.0) wet = 1.0;
    if (wet < 0.0) wet = 0.0;
    if (dry > 1.0) dry = 1.0;
    if (dry < 0.0) dry = 0.0;

    //dsp loop
    for (int i = 0; i < numSamples; ++i)
    {
        double inputL = inL[i];
        double inputR = inR[i];

        //denormalisation protection
        if (std::abs(inputL) < 1.18e-23) inputL = p.fpdL * 1.18e-17;
        if (std::abs(inputR) < 1.18e-23) inputR = p.fpdR * 1.18e-17;

        double drySampleL = inputL;
        double drySampleR = inputR;

        p.cycle++;

        //reverb tick (runs 1/N samples depending on SR)
        if (p.cycle == cycleEnd) 
        {
            //predelay
            p.aZL[p.countZ] = inputL;
            p.aZR[p.countZ] = inputR;
            p.countZ++; 
            if (p.countZ < 0 || p.countZ > predelay) p.countZ = 0;
            
            inputL = p.aZL[p.countZ - ((p.countZ > predelay) ? predelay + 1 : 0)];
            inputR = p.aZR[p.countZ - ((p.countZ > predelay) ? predelay + 1 : 0)];

            //SubTight (non-linear bass compression)
            double outSampleL = inputL * 0.00187;
            double outSampleR = inputR * 0.00187;
            
            //left channel subtight
            double scale = 0.5 + std::abs(outSampleL * 0.5);
            outSampleL = (p.subL_A + (sin(p.subL_A - outSampleL) * scale));
            p.subL_A = outSampleL * scale;
            scale = 0.5 + std::abs(outSampleL * 0.5);
            outSampleL = (p.subL_B + (sin(p.subL_B - outSampleL) * scale));
            p.subL_B = outSampleL * scale;
            scale = 0.5 + std::abs(outSampleL * 0.5);
            outSampleL = (p.subL_C + (sin(p.subL_C - outSampleL) * scale));
            p.subL_C = outSampleL * scale;

            //right channel subtight
            scale = 0.5 + std::abs(outSampleR * 0.5);
            outSampleR = (p.subR_A + (sin(p.subR_A - outSampleR) * scale));
            p.subR_A = outSampleR * scale;
            scale = 0.5 + std::abs(outSampleR * 0.5);
            outSampleR = (p.subR_B + (sin(p.subR_B - outSampleR) * scale));
            p.subR_B = outSampleR * scale;
            scale = 0.5 + std::abs(outSampleR * 0.5);
            outSampleR = (p.subR_C + (sin(p.subR_C - outSampleR) * scale));
            p.subR_C = outSampleR * scale;

            //final subtight mix
            outSampleL = -outSampleL; outSampleR = -outSampleR;
            if (outSampleL > 0.25) outSampleL = 0.25; if (outSampleL < -0.25) outSampleL = -0.25;
            if (outSampleR > 0.25) outSampleR = 0.25; if (outSampleR < -0.25) outSampleR = -0.25;
            inputL -= (outSampleL * 16.0);
            inputR -= (outSampleR * 16.0);

            //early reflections (4x4 golden ratio matrix)
            //left ER
            double earlyAL = inputL - (p.eAL[(p.shortAL + 1) - ((p.shortAL + 1 > shortA) ? shortA + 1 : 0)] * goldenRatio);
            double earlyBL = inputL - (p.eBL[(p.shortBL + 1) - ((p.shortBL + 1 > shortB) ? shortB + 1 : 0)] * goldenRatio);
            double earlyCL = inputL - (p.eCL[(p.shortCL + 1) - ((p.shortCL + 1 > shortC) ? shortC + 1 : 0)] * goldenRatio);
            double earlyDL = inputL - (p.eDL[(p.shortDL + 1) - ((p.shortDL + 1 > shortD) ? shortD + 1 : 0)] * goldenRatio);
            
            // right ER
            double earlyDR = inputR - (p.eDR[(p.shortDR + 1) - ((p.shortDR + 1 > shortD) ? shortD + 1 : 0)] * goldenRatio);
            double earlyHR = inputR - (p.eHR[(p.shortHR + 1) - ((p.shortHR + 1 > shortH) ? shortH + 1 : 0)] * goldenRatio);
            double earlyLR = inputR - (p.eLR[(p.shortLR + 1) - ((p.shortLR + 1 > shortL) ? shortL + 1 : 0)] * goldenRatio);
            double earlyPR = inputR - (p.ePR[(p.shortPR + 1) - ((p.shortPR + 1 > shortP) ? shortP + 1 : 0)] * goldenRatio);

            // update ER buffers
            p.eAL[p.shortAL] = earlyAL; earlyAL *= goldenRatio;
            p.eBL[p.shortBL] = earlyBL; earlyBL *= goldenRatio;
            p.eCL[p.shortCL] = earlyCL; earlyCL *= goldenRatio;
            p.eDL[p.shortDL] = earlyDL; earlyDL *= goldenRatio;
            p.eDR[p.shortDR] = earlyDR; earlyDR *= goldenRatio;
            p.eHR[p.shortHR] = earlyHR; earlyHR *= goldenRatio;
            p.eLR[p.shortLR] = earlyLR; earlyLR *= goldenRatio;
            p.ePR[p.shortPR] = earlyPR; earlyPR *= goldenRatio;

            // increment ER counters
            p.shortAL++; if (p.shortAL > shortA) p.shortAL = 0;
            p.shortBL++; if (p.shortBL > shortB) p.shortBL = 0;
            p.shortCL++; if (p.shortCL > shortC) p.shortCL = 0;
            p.shortDL++; if (p.shortDL > shortD) p.shortDL = 0;
            p.shortDR++; if (p.shortDR > shortD) p.shortDR = 0;
            p.shortHR++; if (p.shortHR > shortH) p.shortHR = 0;
            p.shortLR++; if (p.shortLR > shortL) p.shortLR = 0;
            p.shortPR++; if (p.shortPR > shortP) p.shortPR = 0;

            // ER stage 2 (cascade)
            earlyAL += p.eAL[p.shortAL - ((p.shortAL > shortA) ? shortA + 1 : 0)];
            earlyBL += p.eBL[p.shortBL - ((p.shortBL > shortB) ? shortB + 1 : 0)];
            earlyCL += p.eCL[p.shortCL - ((p.shortCL > shortC) ? shortC + 1 : 0)];
            earlyDL += p.eDL[p.shortDL - ((p.shortDL > shortD) ? shortD + 1 : 0)];
            earlyDR += p.eDR[p.shortDR - ((p.shortDR > shortD) ? shortD + 1 : 0)];
            earlyHR += p.eHR[p.shortHR - ((p.shortHR > shortH) ? shortH + 1 : 0)];
            earlyLR += p.eLR[p.shortLR - ((p.shortLR > shortL) ? shortL + 1 : 0)];
            earlyPR += p.ePR[p.shortPR - ((p.shortPR > shortP) ? shortP + 1 : 0)];

            // ER stage 3 (4x4 mixing)
            double earlyEL = (earlyAL - (earlyBL + earlyCL + earlyDL)) - (p.eEL[(p.shortEL + 1) - ((p.shortEL + 1 > shortE) ? shortE + 1 : 0)] * goldenRatio);
            double earlyFL = (earlyBL - (earlyAL + earlyCL + earlyDL)) - (p.eFL[(p.shortFL + 1) - ((p.shortFL + 1 > shortF) ? shortF + 1 : 0)] * goldenRatio);
            double earlyGL = (earlyCL - (earlyAL + earlyBL + earlyDL)) - (p.eGL[(p.shortGL + 1) - ((p.shortGL + 1 > shortG) ? shortG + 1 : 0)] * goldenRatio);
            double earlyHL = (earlyDL - (earlyAL + earlyBL + earlyCL)) - (p.eHL[(p.shortHL + 1) - ((p.shortHL + 1 > shortH) ? shortH + 1 : 0)] * goldenRatio);
            
            double earlyCR = (earlyDR - (earlyHR + earlyLR + earlyPR)) - (p.eCR[(p.shortCR + 1) - ((p.shortCR + 1 > shortC) ? shortC + 1 : 0)] * goldenRatio);
            double earlyGR = (earlyHR - (earlyDR + earlyLR + earlyPR)) - (p.eGR[(p.shortGR + 1) - ((p.shortGR + 1 > shortG) ? shortG + 1 : 0)] * goldenRatio);
            double earlyKR = (earlyLR - (earlyDR + earlyHR + earlyPR)) - (p.eKR[(p.shortKR + 1) - ((p.shortKR + 1 > shortK) ? shortK + 1 : 0)] * goldenRatio);
            double earlyOR = (earlyPR - (earlyDR + earlyHR + earlyLR)) - (p.eOR[(p.shortOR + 1) - ((p.shortOR + 1 > shortO) ? shortO + 1 : 0)] * goldenRatio);
            
            p.eEL[p.shortEL] = earlyEL; earlyEL *= goldenRatio;
            p.eFL[p.shortFL] = earlyFL; earlyFL *= goldenRatio;
            p.eGL[p.shortGL] = earlyGL; earlyGL *= goldenRatio;
            p.eHL[p.shortHL] = earlyHL; earlyHL *= goldenRatio;
            p.eCR[p.shortCR] = earlyCR; earlyCR *= goldenRatio;
            p.eGR[p.shortGR] = earlyGR; earlyGR *= goldenRatio;
            p.eKR[p.shortKR] = earlyKR; earlyKR *= goldenRatio;
            p.eOR[p.shortOR] = earlyOR; earlyOR *= goldenRatio;

            p.shortEL++; if (p.shortEL > shortE) p.shortEL = 0;
            p.shortFL++; if (p.shortFL > shortF) p.shortFL = 0;
            p.shortGL++; if (p.shortGL > shortG) p.shortGL = 0;
            p.shortHL++; if (p.shortHL > shortH) p.shortHL = 0;
            p.shortCR++; if (p.shortCR > shortC) p.shortCR = 0;
            p.shortGR++; if (p.shortGR > shortG) p.shortGR = 0;
            p.shortKR++; if (p.shortKR > shortK) p.shortKR = 0;
            p.shortOR++; if (p.shortOR > shortO) p.shortOR = 0;

            //cascade again
            earlyEL += p.eEL[p.shortEL - ((p.shortEL > shortE) ? shortE + 1 : 0)];
            earlyFL += p.eFL[p.shortFL - ((p.shortFL > shortF) ? shortF + 1 : 0)];
            earlyGL += p.eGL[p.shortGL - ((p.shortGL > shortG) ? shortG + 1 : 0)];
            earlyHL += p.eHL[p.shortHL - ((p.shortHL > shortH) ? shortH + 1 : 0)];
            earlyCR += p.eCR[p.shortCR - ((p.shortCR > shortC) ? shortC + 1 : 0)];
            earlyGR += p.eGR[p.shortGR - ((p.shortGR > shortG) ? shortG + 1 : 0)];
            earlyKR += p.eKR[p.shortKR - ((p.shortKR > shortK) ? shortK + 1 : 0)];
            earlyOR += p.eOR[p.shortOR - ((p.shortOR > shortO) ? shortO + 1 : 0)];

            // ER stage 4
            double earlyIL = (earlyEL - (earlyFL + earlyGL + earlyHL)) - (p.eIL[(p.shortIL + 1) - ((p.shortIL + 1 > shortI) ? shortI + 1 : 0)] * goldenRatio);
            double earlyJL = (earlyFL - (earlyEL + earlyGL + earlyHL)) - (p.eJL[(p.shortJL + 1) - ((p.shortJL + 1 > shortJ) ? shortJ + 1 : 0)] * goldenRatio);
            double earlyKL = (earlyGL - (earlyEL + earlyFL + earlyHL)) - (p.eKL[(p.shortKL + 1) - ((p.shortKL + 1 > shortK) ? shortK + 1 : 0)] * goldenRatio);
            double earlyLL = (earlyHL - (earlyEL + earlyFL + earlyGL)) - (p.eLL[(p.shortLL + 1) - ((p.shortLL + 1 > shortL) ? shortL + 1 : 0)] * goldenRatio);
            
            double earlyBR = (earlyCR - (earlyGR + earlyKR + earlyOR)) - (p.eBR[(p.shortBR + 1) - ((p.shortBR + 1 > shortB) ? shortB + 1 : 0)] * goldenRatio);
            double earlyFR = (earlyGR - (earlyCR + earlyKR + earlyOR)) - (p.eFR[(p.shortFR + 1) - ((p.shortFR + 1 > shortF) ? shortF + 1 : 0)] * goldenRatio);
            double earlyJR = (earlyKR - (earlyCR + earlyGR + earlyOR)) - (p.eJR[(p.shortJR + 1) - ((p.shortJR + 1 > shortJ) ? shortJ + 1 : 0)] * goldenRatio);
            double earlyNR = (earlyOR - (earlyCR + earlyGR + earlyKR)) - (p.eNR[(p.shortNR + 1) - ((p.shortNR + 1 > shortN) ? shortN + 1 : 0)] * goldenRatio);
            
            p.eIL[p.shortIL] = earlyIL; earlyIL *= goldenRatio;
            p.eJL[p.shortJL] = earlyJL; earlyJL *= goldenRatio;
            p.eKL[p.shortKL] = earlyKL; earlyKL *= goldenRatio;
            p.eLL[p.shortLL] = earlyLL; earlyLL *= goldenRatio;
            p.eBR[p.shortBR] = earlyBR; earlyBR *= goldenRatio;
            p.eFR[p.shortFR] = earlyFR; earlyFR *= goldenRatio;
            p.eJR[p.shortJR] = earlyJR; earlyJR *= goldenRatio;
            p.eNR[p.shortNR] = earlyNR; earlyNR *= goldenRatio;
            
            p.shortIL++; if(p.shortIL > shortI) p.shortIL=0;
            p.shortJL++; if(p.shortJL > shortJ) p.shortJL=0;
            p.shortKL++; if(p.shortKL > shortK) p.shortKL=0;
            p.shortLL++; if(p.shortLL > shortL) p.shortLL=0;
            p.shortBR++; if(p.shortBR > shortB) p.shortBR=0;
            p.shortFR++; if(p.shortFR > shortF) p.shortFR=0;
            p.shortJR++; if(p.shortJR > shortJ) p.shortJR=0;
            p.shortNR++; if(p.shortNR > shortN) p.shortNR=0;

            earlyIL += p.eIL[p.shortIL - ((p.shortIL > shortI) ? shortI + 1 : 0)];
            earlyJL += p.eJL[p.shortJL - ((p.shortJL > shortJ) ? shortJ + 1 : 0)];
            earlyKL += p.eKL[p.shortKL - ((p.shortKL > shortK) ? shortK + 1 : 0)];
            earlyLL += p.eLL[p.shortLL - ((p.shortLL > shortL) ? shortL + 1 : 0)];
            earlyBR += p.eBR[p.shortBR - ((p.shortBR > shortB) ? shortB + 1 : 0)];
            earlyFR += p.eFR[p.shortFR - ((p.shortFR > shortF) ? shortF + 1 : 0)];
            earlyJR += p.eJR[p.shortJR - ((p.shortJR > shortJ) ? shortJ + 1 : 0)];
            earlyNR += p.eNR[p.shortNR - ((p.shortNR > shortN) ? shortN + 1 : 0)];

            // ER stage 5 (final)
            double earlyML = (earlyIL - (earlyJL + earlyKL + earlyLL)) - (p.eML[(p.shortML + 1) - ((p.shortML + 1 > shortM) ? shortM + 1 : 0)] * goldenRatio);
            double earlyNL = (earlyJL - (earlyIL + earlyKL + earlyLL)) - (p.eNL[(p.shortNL + 1) - ((p.shortNL + 1 > shortN) ? shortN + 1 : 0)] * goldenRatio);
            double earlyOL = (earlyKL - (earlyIL + earlyJL + earlyLL)) - (p.eOL[(p.shortOL + 1) - ((p.shortOL + 1 > shortO) ? shortO + 1 : 0)] * goldenRatio);
            double earlyPL = (earlyLL - (earlyIL + earlyJL + earlyKL)) - (p.ePL[(p.shortPL + 1) - ((p.shortPL + 1 > shortP) ? shortP + 1 : 0)] * goldenRatio);
            
            double earlyAR = (earlyBR - (earlyFR + earlyJR + earlyNR)) - (p.eAR[(p.shortAR + 1) - ((p.shortAR + 1 > shortA) ? shortA + 1 : 0)] * goldenRatio);
            double earlyER = (earlyFR - (earlyBR + earlyJR + earlyNR)) - (p.eER[(p.shortER + 1) - ((p.shortER + 1 > shortE) ? shortE + 1 : 0)] * goldenRatio);
            double earlyIR = (earlyJR - (earlyBR + earlyFR + earlyNR)) - (p.eIR[(p.shortIR + 1) - ((p.shortIR + 1 > shortI) ? shortI + 1 : 0)] * goldenRatio);
            double earlyMR = (earlyNR - (earlyBR + earlyFR + earlyJR)) - (p.eMR[(p.shortMR + 1) - ((p.shortMR + 1 > shortM) ? shortM + 1 : 0)] * goldenRatio);

            p.eML[p.shortML] = earlyML; earlyML *= goldenRatio;
            p.eNL[p.shortNL] = earlyNL; earlyNL *= goldenRatio;
            p.eOL[p.shortOL] = earlyOL; earlyOL *= goldenRatio;
            p.ePL[p.shortPL] = earlyPL; earlyPL *= goldenRatio;
            p.eAR[p.shortAR] = earlyAR; earlyAR *= goldenRatio;
            p.eER[p.shortER] = earlyER; earlyER *= goldenRatio;
            p.eIR[p.shortIR] = earlyIR; earlyIR *= goldenRatio;
            p.eMR[p.shortMR] = earlyMR; earlyMR *= goldenRatio;

            p.shortML++; if(p.shortML > shortM) p.shortML=0;
            p.shortNL++; if(p.shortNL > shortN) p.shortNL=0;
            p.shortOL++; if(p.shortOL > shortO) p.shortOL=0;
            p.shortPL++; if(p.shortPL > shortP) p.shortPL=0;
            p.shortAR++; if(p.shortAR > shortA) p.shortAR=0;
            p.shortER++; if(p.shortER > shortE) p.shortER=0;
            p.shortIR++; if(p.shortIR > shortI) p.shortIR=0;
            p.shortMR++; if(p.shortMR > shortM) p.shortMR=0;

            earlyML += p.eML[p.shortML - ((p.shortML > shortM) ? shortM + 1 : 0)];
            earlyNL += p.eNL[p.shortNL - ((p.shortNL > shortN) ? shortN + 1 : 0)];
            earlyOL += p.eOL[p.shortOL - ((p.shortOL > shortO) ? shortO + 1 : 0)];
            earlyPL += p.ePL[p.shortPL - ((p.shortPL > shortP) ? shortP + 1 : 0)];
            earlyAR += p.eAR[p.shortAR - ((p.shortAR > shortA) ? shortA + 1 : 0)];
            earlyER += p.eER[p.shortER - ((p.shortER > shortE) ? shortE + 1 : 0)];
            earlyIR += p.eIR[p.shortIR - ((p.shortIR > shortI) ? shortI + 1 : 0)];
            earlyMR += p.eMR[p.shortMR - ((p.shortMR > shortM) ? shortM + 1 : 0)];

            double earlyReflectionsL = -(earlyML + earlyNL + earlyOL + earlyPL) / 8.0;
            double earlyReflectionsR = -(earlyAR + earlyER + earlyIR + earlyMR) / 8.0;

            //VLF predelay 
            p.aVLFL[p.countVLF] = outSampleL;
            p.aVLFR[p.countVLF] = outSampleR;
            p.countVLF++; if (p.countVLF > vlfpredelay) p.countVLF = 0;
            
            outSampleL = p.aVLFL[p.countVLF - ((p.countVLF > vlfpredelay) ? vlfpredelay + 1 : 0)] * 2.0;
            outSampleR = p.aVLFR[p.countVLF - ((p.countVLF > vlfpredelay) ? vlfpredelay + 1 : 0)] * 2.0;

            inputL += outSampleL;
            inputR += outSampleR;

            //this is the main tank (5x5 Householder)
            //stage 1
            p.aAL[p.countAL] = inputL + (p.feedbackAL * 0.000285);
            p.aBL[p.countBL] = inputL + (p.feedbackBL * 0.000285);
            p.aCL[p.countCL] = inputL + (p.feedbackCL * 0.000285);
            p.aDL[p.countDL] = inputL + (p.feedbackDL * 0.000285);
            p.aEL[p.countEL] = inputL + (p.feedbackEL * 0.000285);

            p.aER[p.countER] = inputR + (p.feedbackER * 0.000285);
            p.aJR[p.countJR] = inputR + (p.feedbackJR * 0.000285);
            p.aOR[p.countOR] = inputR + (p.feedbackOR * 0.000285);
            p.aTR[p.countTR] = inputR + (p.feedbackTR * 0.000285);
            p.aYR[p.countYR] = inputR + (p.feedbackYR * 0.000285);

            p.countAL++; if(p.countAL > delayA) p.countAL=0;
            p.countBL++; if(p.countBL > delayB) p.countBL=0;
            p.countCL++; if(p.countCL > delayC) p.countCL=0;
            p.countDL++; if(p.countDL > delayD) p.countDL=0;
            p.countEL++; if(p.countEL > delayE) p.countEL=0;

            p.countER++; if(p.countER > delayE) p.countER=0;
            p.countJR++; if(p.countJR > delayJ) p.countJR=0;
            p.countOR++; if(p.countOR > delayO) p.countOR=0;
            p.countTR++; if(p.countTR > delayT) p.countTR=0;
            p.countYR++; if(p.countYR > delayY) p.countYR=0;

            double outAL = p.aAL[p.countAL - ((p.countAL > delayA)?delayA+1:0)];
            double outBL = p.aBL[p.countBL - ((p.countBL > delayB)?delayB+1:0)];
            double outCL = p.aCL[p.countCL - ((p.countCL > delayC)?delayC+1:0)];
            double outDL = p.aDL[p.countDL - ((p.countDL > delayD)?delayD+1:0)];
            double outEL = p.aEL[p.countEL - ((p.countEL > delayE)?delayE+1:0)];

            double outER = p.aER[p.countER - ((p.countER > delayE)?delayE+1:0)];
            double outJR = p.aJR[p.countJR - ((p.countJR > delayJ)?delayJ+1:0)];
            double outOR = p.aOR[p.countOR - ((p.countOR > delayO)?delayO+1:0)];
            double outTR = p.aTR[p.countTR - ((p.countTR > delayT)?delayT+1:0)];
            double outYR = p.aYR[p.countYR - ((p.countYR > delayY)?delayY+1:0)];

            //stage 2 (Householder mixing)
            p.aFL[p.countFL] = ((outAL*3.0) - ((outBL+outCL+outDL+outEL)*2.0));
            p.aGL[p.countGL] = ((outBL*3.0) - ((outAL+outCL+outDL+outEL)*2.0));
            p.aHL[p.countHL] = ((outCL*3.0) - ((outAL+outBL+outDL+outEL)*2.0));
            p.aIL[p.countIL] = ((outDL*3.0) - ((outAL+outBL+outCL+outEL)*2.0));
            p.aJL[p.countJL] = ((outEL*3.0) - ((outAL+outBL+outCL+outDL)*2.0));

            p.aDR[p.countDR] = ((outER*3.0) - ((outJR+outOR+outTR+outYR)*2.0));
            p.aIR[p.countIR] = ((outJR*3.0) - ((outER+outOR+outTR+outYR)*2.0));
            p.aNR[p.countNR] = ((outOR*3.0) - ((outER+outJR+outTR+outYR)*2.0));
            p.aSR[p.countSR] = ((outTR*3.0) - ((outER+outJR+outOR+outYR)*2.0));
            p.aXR[p.countXR] = ((outYR*3.0) - ((outER+outJR+outOR+outTR)*2.0));

            p.countFL++; if(p.countFL > delayF) p.countFL=0;
            p.countGL++; if(p.countGL > delayG) p.countGL=0;
            p.countHL++; if(p.countHL > delayH) p.countHL=0;
            p.countIL++; if(p.countIL > delayI) p.countIL=0;
            p.countJL++; if(p.countJL > delayJ) p.countJL=0;

            p.countDR++; if(p.countDR > delayD) p.countDR=0;
            p.countIR++; if(p.countIR > delayI) p.countIR=0;
            p.countNR++; if(p.countNR > delayN) p.countNR=0;
            p.countSR++; if(p.countSR > delayS) p.countSR=0;
            p.countXR++; if(p.countXR > delayX) p.countXR=0;

            double outFL = p.aFL[p.countFL - ((p.countFL > delayF)?delayF+1:0)];
            double outGL = p.aGL[p.countGL - ((p.countGL > delayG)?delayG+1:0)];
            double outHL = p.aHL[p.countHL - ((p.countHL > delayH)?delayH+1:0)];
            double outIL = p.aIL[p.countIL - ((p.countIL > delayI)?delayI+1:0)];
            double outJL = p.aJL[p.countJL - ((p.countJL > delayJ)?delayJ+1:0)];

            double outDR = p.aDR[p.countDR - ((p.countDR > delayD)?delayD+1:0)];
            double outIR = p.aIR[p.countIR - ((p.countIR > delayI)?delayI+1:0)];
            double outNR = p.aNR[p.countNR - ((p.countNR > delayN)?delayN+1:0)];
            double outSR = p.aSR[p.countSR - ((p.countSR > delayS)?delayS+1:0)];
            double outXR = p.aXR[p.countXR - ((p.countXR > delayX)?delayX+1:0)];

            //stage 3
            p.aKL[p.countKL] = ((outFL*3.0) - ((outGL+outHL+outIL+outJL)*2.0));
            p.aLL[p.countLL] = ((outGL*3.0) - ((outFL+outHL+outIL+outJL)*2.0));
            p.aML[p.countML] = ((outHL*3.0) - ((outFL+outGL+outIL+outJL)*2.0));
            p.aNL[p.countNL] = ((outIL*3.0) - ((outFL+outGL+outHL+outJL)*2.0));
            p.aOL[p.countOL] = ((outJL*3.0) - ((outFL+outGL+outHL+outIL)*2.0));

            p.aCR[p.countCR] = ((outDR*3.0) - ((outIR+outNR+outSR+outXR)*2.0));
            p.aHR[p.countHR] = ((outIR*3.0) - ((outDR+outNR+outSR+outXR)*2.0));
            p.aMR[p.countMR] = ((outNR*3.0) - ((outDR+outIR+outSR+outXR)*2.0));
            p.aRR[p.countRR] = ((outSR*3.0) - ((outDR+outIR+outNR+outXR)*2.0));
            p.aWR[p.countWR] = ((outXR*3.0) - ((outDR+outIR+outNR+outSR)*2.0));

            p.countKL++; if(p.countKL > delayK) p.countKL=0;
            p.countLL++; if(p.countLL > delayL) p.countLL=0;
            p.countML++; if(p.countML > delayM) p.countML=0;
            p.countNL++; if(p.countNL > delayN) p.countNL=0;
            p.countOL++; if(p.countOL > delayO) p.countOL=0;

            p.countCR++; if(p.countCR > delayC) p.countCR=0;
            p.countHR++; if(p.countHR > delayH) p.countHR=0;
            p.countMR++; if(p.countMR > delayM) p.countMR=0;
            p.countRR++; if(p.countRR > delayR) p.countRR=0;
            p.countWR++; if(p.countWR > delayW) p.countWR=0;

            double outKL = p.aKL[p.countKL - ((p.countKL > delayK)?delayK+1:0)];
            double outLL = p.aLL[p.countLL - ((p.countLL > delayL)?delayL+1:0)];
            double outML = p.aML[p.countML - ((p.countML > delayM)?delayM+1:0)];
            double outNL = p.aNL[p.countNL - ((p.countNL > delayN)?delayN+1:0)];
            double outOL = p.aOL[p.countOL - ((p.countOL > delayO)?delayO+1:0)];

            double outCR = p.aCR[p.countCR - ((p.countCR > delayC)?delayC+1:0)];
            double outHR = p.aHR[p.countHR - ((p.countHR > delayH)?delayH+1:0)];
            double outMR = p.aMR[p.countMR - ((p.countMR > delayM)?delayM+1:0)];
            double outRR = p.aRR[p.countRR - ((p.countRR > delayR)?delayR+1:0)];
            double outWR = p.aWR[p.countWR - ((p.countWR > delayW)?delayW+1:0)];

            // Stage 4
            p.aPL[p.countPL] = ((outKL*3.0) - ((outLL+outML+outNL+outOL)*2.0));
            p.aQL[p.countQL] = ((outLL*3.0) - ((outKL+outML+outNL+outOL)*2.0));
            p.aRL[p.countRL] = ((outML*3.0) - ((outKL+outLL+outNL+outOL)*2.0));
            p.aSL[p.countSL] = ((outNL*3.0) - ((outKL+outLL+outML+outOL)*2.0));
            p.aTL[p.countTL] = ((outOL*3.0) - ((outKL+outLL+outML+outNL)*2.0));

            p.aBR[p.countBR] = ((outCR*3.0) - ((outHR+outMR+outRR+outWR)*2.0));
            p.aGR[p.countGR] = ((outHR*3.0) - ((outCR+outMR+outRR+outWR)*2.0));
            p.aLR[p.countLR] = ((outMR*3.0) - ((outCR+outHR+outRR+outWR)*2.0));
            p.aQR[p.countQR] = ((outRR*3.0) - ((outCR+outHR+outMR+outWR)*2.0));
            p.aVR[p.countVR] = ((outWR*3.0) - ((outCR+outHR+outMR+outRR)*2.0));

            p.countPL++; if(p.countPL > delayP) p.countPL=0;
            p.countQL++; if(p.countQL > delayQ) p.countQL=0;
            p.countRL++; if(p.countRL > delayR) p.countRL=0;
            p.countSL++; if(p.countSL > delayS) p.countSL=0;
            p.countTL++; if(p.countTL > delayT) p.countTL=0;

            p.countBR++; if(p.countBR > delayB) p.countBR=0;
            p.countGR++; if(p.countGR > delayG) p.countGR=0;
            p.countLR++; if(p.countLR > delayL) p.countLR=0;
            p.countQR++; if(p.countQR > delayQ) p.countQR=0;
            p.countVR++; if(p.countVR > delayV) p.countVR=0;

            double outPL = p.aPL[p.countPL - ((p.countPL > delayP)?delayP+1:0)];
            double outQL = p.aQL[p.countQL - ((p.countQL > delayQ)?delayQ+1:0)];
            double outRL = p.aRL[p.countRL - ((p.countRL > delayR)?delayR+1:0)];
            double outSL = p.aSL[p.countSL - ((p.countSL > delayS)?delayS+1:0)];
            double outTL = p.aTL[p.countTL - ((p.countTL > delayT)?delayT+1:0)];

            double outBR = p.aBR[p.countBR - ((p.countBR > delayB)?delayB+1:0)];
            double outGR = p.aGR[p.countGR - ((p.countGR > delayG)?delayG+1:0)];
            double outLR = p.aLR[p.countLR - ((p.countLR > delayL)?delayL+1:0)];
            double outQR = p.aQR[p.countQR - ((p.countQR > delayQ)?delayQ+1:0)];
            double outVR = p.aVR[p.countVR - ((p.countVR > delayV)?delayV+1:0)];

            //stage 5 (modulated)
            p.aVL[p.countVL] = ((outQL*3.0) - ((outPL+outRL+outSL+outTL)*2.0));
            p.aWL[p.countWL] = ((outRL*3.0) - ((outPL+outQL+outSL+outTL)*2.0));
            p.aXL[p.countXL] = ((outSL*3.0) - ((outPL+outQL+outRL+outTL)*2.0));
            p.aYL[p.countYL] = ((outTL*3.0) - ((outPL+outQL+outRL+outSL)*2.0));

            p.aAR[p.countAR] = ((outBR*3.0) - ((outGR+outLR+outQR+outVR)*2.0));
            p.aFR[p.countFR] = ((outGR*3.0) - ((outBR+outLR+outQR+outVR)*2.0));
            p.aKR[p.countKR] = ((outLR*3.0) - ((outBR+outGR+outQR+outVR)*2.0));
            p.aPR[p.countPR] = ((outQR*3.0) - ((outBR+outGR+outLR+outVR)*2.0));

            //modulated Delay U
            double outUL = ((outPL*3.0) - ((outQL+outRL+outSL+outTL)*2.0)) - (p.aUL[(p.countUL + 1) - ((p.countUL + 1 > delayU) ? delayU + 1 : 0)] * goldenRatio);
            double outUR = ((outVR*3.0) - ((outBR+outGR+outLR+outQR)*2.0)) - (p.aUR[(p.countUR + 1) - ((p.countUR + 1 > delayU) ? delayU + 1 : 0)] * goldenRatio);
            
            p.aUL[p.countUL] = outUL; outUL *= goldenRatio;
            p.aUR[p.countUR] = outUR; outUR *= goldenRatio;
            p.countUL++; if(p.countUL > delayU) p.countUL=0;
            p.countUR++; if(p.countUR > delayU) p.countUR=0;
            outUL += p.aUL[p.countUL - ((p.countUL > delayU)?delayU+1:0)];
            outUR += p.aUR[p.countUR - ((p.countUR > delayU)?delayU+1:0)];

            //vibrato Logic
            p.vibB_L = p.vibA_L; p.vibA_L = outUL; // simplified history update
            p.vibB_R = p.vibA_R; p.vibA_R = outUR; 

            p.vibratoL += p.fpdL * 0.5e-13; if (p.vibratoL > M_PI * 2.0) p.vibratoL -= M_PI * 2.0;
            p.vibratoR += p.fpdR * 0.5e-13; if (p.vibratoR > M_PI * 2.0) p.vibratoR -= M_PI * 2.0;
            double quadL = sin(p.vibratoL) + 1.0;
            double quadR = sin(p.vibratoR) + 1.0;

            if (quadL < 1.0) outUL = (outUL * (1.0 - quadL)) + (p.vibA_L * quadL);
            else outUL = (p.vibA_L * (1.0 - (quadL - 1.0))) + (p.vibB_L * (quadL - 1.0)); //fixed variable name to vibB_L

            if (quadR < 1.0) outUR = (outUR * (1.0 - quadR)) + (p.vibA_R * quadR);
            else outUR = (p.vibA_R * (1.0 - (quadR - 1.0))) + (p.vibB_R * (quadR - 1.0));

            p.countVL++; if(p.countVL > delayV) p.countVL=0;
            p.countWL++; if(p.countWL > delayW) p.countWL=0;
            p.countXL++; if(p.countXL > delayX) p.countXL=0;
            p.countYL++; if(p.countYL > delayY) p.countYL=0;

            p.countAR++; if(p.countAR > delayA) p.countAR=0;
            p.countFR++; if(p.countFR > delayF) p.countFR=0;
            p.countKR++; if(p.countKR > delayK) p.countKR=0;
            p.countPR++; if(p.countPR > delayP) p.countPR=0;

            double outVL = p.aVL[p.countVL - ((p.countVL > delayV)?delayV+1:0)];
            double outWL = p.aWL[p.countWL - ((p.countWL > delayW)?delayW+1:0)];
            double outXL = p.aXL[p.countXL - ((p.countXL > delayX)?delayX+1:0)];
            double outYL = p.aYL[p.countYL - ((p.countYL > delayY)?delayY+1:0)];

            double outAR = p.aAR[p.countAR - ((p.countAR > delayA)?delayA+1:0)];
            double outFR = p.aFR[p.countFR - ((p.countFR > delayF)?delayF+1:0)];
            double outKR = p.aKR[p.countKR - ((p.countKR > delayK)?delayK+1:0)];
            double outPR = p.aPR[p.countPR - ((p.countPR > delayP)?delayP+1:0)];

            //feedback with Pearson filtering
            p.feedbackER = ((outUL*3.0) - ((outVL+outWL+outXL+outYL)*2.0));
            p.feedbackAL = ((outAR*3.0) - ((outFR+outKR+outPR+outUR)*2.0));

            //pearson stage 1
            for (int x = 0; x < 1; x += 4) { // loop runs once, processes 4 values
                double slew = ((p.feedbackAL - p.pearsonFilters[0][x]) + p.pearsonFilters[0][x+1]) * pear * 0.5;
                p.pearsonFilters[0][x] = p.feedbackAL = (pear * p.feedbackAL) + ((1.0 - pear) * (p.pearsonFilters[0][x] + p.pearsonFilters[0][x+1]));
                p.pearsonFilters[0][x+1] = slew;
                
                slew = ((p.feedbackER - p.pearsonFilters[0][x+2]) + p.pearsonFilters[0][x+3]) * pear * 0.5;
                p.pearsonFilters[0][x+2] = p.feedbackER = (pear * p.feedbackER) + ((1.0 - pear) * (p.pearsonFilters[0][x+2] + p.pearsonFilters[0][x+3]));
                p.pearsonFilters[0][x+3] = slew;
            }

            p.feedbackBL = ((outVL*3.0) - ((outUL+outWL+outXL+outYL)*2.0));
            p.feedbackJR = ((outFR*3.0) - ((outAR+outKR+outPR+outUR)*2.0));
            // pearson stage 2
            for (int x = 0; x < 1; x += 4) {
                 double slew = ((p.feedbackBL - p.pearsonFilters[1][x]) + p.pearsonFilters[1][x+1]) * pear * 0.5;
                 p.pearsonFilters[1][x] = p.feedbackBL = (pear * p.feedbackBL) + ((1.0 - pear) * (p.pearsonFilters[1][x] + p.pearsonFilters[1][x+1]));
                 p.pearsonFilters[1][x+1] = slew;
                 slew = ((p.feedbackJR - p.pearsonFilters[1][x+2]) + p.pearsonFilters[1][x+3]) * pear * 0.5;
                 p.pearsonFilters[1][x+2] = p.feedbackJR = (pear * p.feedbackJR) + ((1.0 - pear) * (p.pearsonFilters[1][x+2] + p.pearsonFilters[1][x+3]));
                 p.pearsonFilters[1][x+3] = slew;
            }

            p.feedbackCL = ((outWL*3.0) - ((outUL+outVL+outXL+outYL)*2.0));
            p.feedbackOR = ((outKR*3.0) - ((outAR+outFR+outPR+outUR)*2.0));
            // pearson stage 3
            for (int x = 0; x < 1; x += 4) {
                double slew = ((p.feedbackCL - p.pearsonFilters[2][x]) + p.pearsonFilters[2][x+1]) * pear * 0.5;
                p.pearsonFilters[2][x] = p.feedbackCL = (pear * p.feedbackCL) + ((1.0 - pear) * (p.pearsonFilters[2][x] + p.pearsonFilters[2][x+1]));
                p.pearsonFilters[2][x+1] = slew;
                slew = ((p.feedbackOR - p.pearsonFilters[2][x+2]) + p.pearsonFilters[2][x+3]) * pear * 0.5;
                p.pearsonFilters[2][x+2] = p.feedbackOR = (pear * p.feedbackOR) + ((1.0 - pear) * (p.pearsonFilters[2][x+2] + p.pearsonFilters[2][x+3]));
                p.pearsonFilters[2][x+3] = slew;
            }

            p.feedbackDL = ((outXL*3.0) - ((outUL+outVL+outWL+outYL)*2.0));
            p.feedbackTR = ((outPR*3.0) - ((outAR+outFR+outKR+outUR)*2.0));
            //pearson stage 4
            for (int x = 0; x < 1; x += 4) {
                double slew = ((p.feedbackDL - p.pearsonFilters[3][x]) + p.pearsonFilters[3][x+1]) * pear * 0.5;
                p.pearsonFilters[3][x] = p.feedbackDL = (pear * p.feedbackDL) + ((1.0 - pear) * (p.pearsonFilters[3][x] + p.pearsonFilters[3][x+1]));
                p.pearsonFilters[3][x+1] = slew;
                slew = ((p.feedbackTR - p.pearsonFilters[3][x+2]) + p.pearsonFilters[3][x+3]) * pear * 0.5;
                p.pearsonFilters[3][x+2] = p.feedbackTR = (pear * p.feedbackTR) + ((1.0 - pear) * (p.pearsonFilters[3][x+2] + p.pearsonFilters[3][x+3]));
                p.pearsonFilters[3][x+3] = slew;
            }

            p.feedbackEL = ((outYL*3.0) - ((outUL+outVL+outWL+outXL)*2.0));
            p.feedbackYR = ((outUR*3.0) - ((outAR+outFR+outKR+outPR)*2.0));
            //pearson stage 5
            for (int x = 0; x < 1; x += 4) {
                double slew = ((p.feedbackEL - p.pearsonFilters[4][x]) + p.pearsonFilters[4][x+1]) * pear * 0.5;
                p.pearsonFilters[4][x] = p.feedbackEL = (pear * p.feedbackEL) + ((1.0 - pear) * (p.pearsonFilters[4][x] + p.pearsonFilters[4][x+1]));
                p.pearsonFilters[4][x+1] = slew;
                slew = ((p.feedbackYR - p.pearsonFilters[4][x+2]) + p.pearsonFilters[4][x+3]) * pear * 0.5;
                p.pearsonFilters[4][x+2] = p.feedbackYR = (pear * p.feedbackYR) + ((1.0 - pear) * (p.pearsonFilters[4][x+2] + p.pearsonFilters[4][x+3]));
                p.pearsonFilters[4][x+3] = slew;
            }

            inputL = (outUL + outVL + outWL + outXL + outYL) * 0.0004;
            inputR = (outAR + outFR + outKR + outPR + outUR) * 0.0004;

            //subboost (inverse of subtight)
            outSampleL = inputL * 0.00186;
            outSampleR = inputR * 0.00186;
            
            scale = 0.5 + std::abs(outSampleL * 0.5);
            outSampleL = (p.subBufferL_A + (sin(p.subBufferL_A - outSampleL) * scale));
            p.subBufferL_A = outSampleL * scale;
            scale = 0.5 + std::abs(outSampleR * 0.5);
            outSampleR = (p.subBufferR_A + (sin(p.subBufferR_A - outSampleR) * scale));
            p.subBufferR_A = outSampleR * scale;
            
            scale = 0.5 + std::abs(outSampleL * 0.5);
            outSampleL = (p.subBufferL_B + (sin(p.subBufferL_B - outSampleL) * scale));
            p.subBufferL_B = outSampleL * scale;
            scale = 0.5 + std::abs(outSampleR * 0.5);
            outSampleR = (p.subBufferR_B + (sin(p.subBufferR_B - outSampleR) * scale));
            p.subBufferR_B = outSampleR * scale;
            
            scale = 0.5 + std::abs(outSampleL * 0.5);
            outSampleL = (p.subBufferL_C + (sin(p.subBufferL_C - outSampleL) * scale));
            p.subBufferL_C = outSampleL * scale;
            scale = 0.5 + std::abs(outSampleR * 0.5);
            outSampleR = (p.subBufferR_C + (sin(p.subBufferR_C - outSampleR) * scale));
            p.subBufferR_C = outSampleR * scale;
            
            outSampleL = -outSampleL; outSampleR = -outSampleR;
            if (outSampleL > 0.25) outSampleL = 0.25; if (outSampleL < -0.25) outSampleL = -0.25;
            if (outSampleR > 0.25) outSampleR = 0.25; if (outSampleR < -0.25) outSampleR = -0.25;
            inputL += (outSampleL * 32.0);
            inputR += (outSampleR * 32.0);

            //add early reflections
            inputL += (earlyReflectionsL * 0.25);
            inputR += (earlyReflectionsR * 0.25);

            //pversampling averaging logic
            if (cycleEnd == 4) {
                p.lastRefL[0] = p.lastRefL[4]; p.lastRefL[2] = (p.lastRefL[0] + inputL)/2; p.lastRefL[1] = (p.lastRefL[0] + p.lastRefL[2])/2; p.lastRefL[3] = (p.lastRefL[2] + inputL)/2; p.lastRefL[4] = inputL;
                p.lastRefR[0] = p.lastRefR[4]; p.lastRefR[2] = (p.lastRefR[0] + inputR)/2; p.lastRefR[1] = (p.lastRefR[0] + p.lastRefR[2])/2; p.lastRefR[3] = (p.lastRefR[2] + inputR)/2; p.lastRefR[4] = inputR;
            } else if (cycleEnd == 3) {
                p.lastRefL[0] = p.lastRefL[3]; p.lastRefL[2] = (p.lastRefL[0]+p.lastRefL[0]+inputL)/3; p.lastRefL[1] = (p.lastRefL[0]+inputL+inputL)/3; p.lastRefL[3] = inputL;
                p.lastRefR[0] = p.lastRefR[3]; p.lastRefR[2] = (p.lastRefR[0]+p.lastRefR[0]+inputR)/3; p.lastRefR[1] = (p.lastRefR[0]+inputR+inputR)/3; p.lastRefR[3] = inputR;
            } else if (cycleEnd == 2) {
                p.lastRefL[0] = p.lastRefL[2]; p.lastRefL[1] = (p.lastRefL[0] + inputL)/2; p.lastRefL[2] = inputL;
                p.lastRefR[0] = p.lastRefR[2]; p.lastRefR[1] = (p.lastRefR[0] + inputR)/2; p.lastRefR[2] = inputR;
            } else {
                p.lastRefL[0] = inputL; p.lastRefR[0] = inputR;
            }
            p.cycle = 0;
            inputL = p.lastRefL[p.cycle];
            inputR = p.lastRefR[p.cycle];

        } else {
            inputL = p.lastRefL[p.cycle];
            inputR = p.lastRefR[p.cycle];
        }

        //final Pearson smoothing
        for (int x = 0; x < 1; x += 4) {
            double slew = ((inputL - p.pearsonFilter[x]) + p.pearsonFilter[x+1]) * pearScaled * 0.5;
            p.pearsonFilter[x] = inputL = (pearScaled * inputL) + ((1.0 - pearScaled) * (p.pearsonFilter[x] + p.pearsonFilter[x+1]));
            p.pearsonFilter[x+1] = slew;
            slew = ((inputR - p.pearsonFilter[x+2]) + p.pearsonFilter[x+3]) * pearScaled * 0.5;
            p.pearsonFilter[x+2] = inputR = (pearScaled * inputR) + ((1.0 - pearScaled) * (p.pearsonFilter[x+2] + p.pearsonFilter[x+3]));
            p.pearsonFilter[x+3] = slew;
        }

        //outp mix
        if (wet < 1.0) { inputL *= wet; inputR *= wet; }
        if (dry < 1.0) { drySampleL *= dry; drySampleR *= dry; }
        inputL += drySampleL;
        inputR += drySampleR;

        //stereo dither
        int expon; frexpf((float)inputL, &expon);
        p.fpdL ^= p.fpdL << 13; p.fpdL ^= p.fpdL >> 17; p.fpdL ^= p.fpdL << 5;
        inputL += ((double(p.fpdL) - (uint32_t)0x7fffffff) * 5.5e-36l * pow(2, expon + 62));
        frexpf((float)inputR, &expon);
        p.fpdR ^= p.fpdR << 13; p.fpdR ^= p.fpdR >> 17; p.fpdR ^= p.fpdR << 5;
        inputR += ((double(p.fpdR) - (uint32_t)0x7fffffff) * 5.5e-36l * pow(2, expon + 62));

        outL[i] = (float)inputL;
        outR[i] = (float)inputR;
    }
}