//
// Created by Facundo Franchino on 24/03/2025.
//
#pragma once

// forward declare the processor class
class ushuaiaVerbAudioProcessor;

//correct signature accepting the class reference
void processUshuaiaVerb(ushuaiaVerbAudioProcessor& p, float* inL, float* inR, float* outL, float* outR,
                        int numSamples, double sampleRate);