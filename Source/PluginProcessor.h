/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/**
*/
class WaveshaperDemoAudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    WaveshaperDemoAudioProcessor();
    ~WaveshaperDemoAudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void reset() override;
    void releaseResources() override;

    void processBlock (AudioSampleBuffer&, MidiBuffer&) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override { return JucePlugin_Name; }
    
    bool acceptsMidi() const override
    {
        #if JucePlugin_WantsMidiInput
            return true;
        #else
            return false;
        #endif
    }
    
    bool producesMidi() const override
    {
        #if JucePlugin_ProducesMidiOutput
            return true;
        #else
            return false;
        #endif
    }
    
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override { return 1; }
    int getCurrentProgram() override { return 0; }
    void setCurrentProgram (int /*index*/) override {}
    const String getProgramName (int /*index*/) override { return String(); }
    void changeProgramName (int /*index*/, const String& /*newName*/) override {}

    //==============================================================================
	void getStateInformation(MemoryBlock& destData) override;
	void setStateInformation(const void* data, int sizeInBytes) override;

private:
	AudioProcessorValueTreeState parameters;

    //==============================================================================
    void updateProcessing();

    //==============================================================================
    bool isActive = false;

	// basic controls
	float* prmDrive = nullptr;
	float* prmMix = nullptr;
	
	// waveshape params
	float* prmTanhAmp = nullptr;
	float* prmTanhSlope = nullptr;
	float* prmSineAmp = nullptr;
	float* prmSineFreq = nullptr;

    LinearSmoothedValue<float> driveVolume, dryVolume, wetVolume;

	LinearSmoothedValue<float> tanhAmplitude, tanhSlope, sineAmplitude, sineFrequency;

    AudioBuffer<float> mixBuffer;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WaveshaperDemoAudioProcessor)
};

