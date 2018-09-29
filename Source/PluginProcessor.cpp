/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
WaveshaperDemoAudioProcessor::WaveshaperDemoAudioProcessor()
	: parameters(*this, nullptr)
{
	parameters.createAndAddParameter("DRIVE",     "Drive",          String(), NormalisableRange<float>(0.f, 40.f, 0.01),     20.f,    nullptr, nullptr);
	parameters.createAndAddParameter("MIX",       "Mix",            String(), NormalisableRange<float>(0.f, 100.f, 0.01),    100.f,   nullptr, nullptr);

	parameters.createAndAddParameter("TANHAMP",   "Tanh amp",       String(), NormalisableRange<float>(0.f, 100.f, 0.01),    100.f,   nullptr, nullptr);
	parameters.createAndAddParameter("TANHSLOPE", "Tanh slope",     String(), NormalisableRange<float>(1.f, 15.f, 0.01),     1.f,     nullptr, nullptr);
	parameters.createAndAddParameter("SINEAMP",   "Sine amp",       String(), NormalisableRange<float>(0.f, 100.f, 0.01),    0.f,     nullptr, nullptr);
	parameters.createAndAddParameter("SINEFREQ",  "Sine freq",      String(), NormalisableRange<float>(0.5f, 100.f, 0.01),   1.f,     nullptr, nullptr);

	parameters.state = ValueTree(Identifier("Waveshaper"));

	prmDrive = parameters.getRawParameterValue("DRIVE");
	prmMix = parameters.getRawParameterValue("MIX");
	prmTanhAmp = parameters.getRawParameterValue("TANHAMP");
	prmTanhSlope = parameters.getRawParameterValue("TANHSLOPE");
	prmSineAmp = parameters.getRawParameterValue("SINEAMP");
	prmSineFreq = parameters.getRawParameterValue("SINEFREQ");
}

WaveshaperDemoAudioProcessor::~WaveshaperDemoAudioProcessor()
{
}

void WaveshaperDemoAudioProcessor::getStateInformation(MemoryBlock& destData)
{
	auto state = parameters.copyState();
	std::unique_ptr<XmlElement> xml(state.createXml());
	copyXmlToBinary(*xml, destData);
}

void WaveshaperDemoAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
	std::unique_ptr<XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));

	if (xmlState.get() != nullptr)
		if (xmlState->hasTagName(parameters.state.getType()))
			parameters.replaceState(ValueTree::fromXml(*xmlState));
}

//==============================================================================
double WaveshaperDemoAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

//==============================================================================
void WaveshaperDemoAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    auto channels = static_cast<uint32> (jmin (getMainBusNumInputChannels(), getMainBusNumOutputChannels()));
    dsp::ProcessSpec spec { sampleRate, static_cast<uint32> (samplesPerBlock), channels };
    
    mixBuffer.setSize (channels, samplesPerBlock);
    
    updateProcessing();
    reset();

    isActive = true;
}

void WaveshaperDemoAudioProcessor::reset()
{
    mixBuffer.applyGain (0.f);
    
    driveVolume.reset (getSampleRate(), 0.05);
    dryVolume.reset (getSampleRate(), 0.05);
    wetVolume.reset (getSampleRate(), 0.05);

	tanhAmplitude.reset(getSampleRate(), 0.05);
	tanhSlope.reset(getSampleRate(), 0.05);
	sineAmplitude.reset(getSampleRate(), 0.05);
	sineFrequency.reset(getSampleRate(), 0.05);
    
}

void WaveshaperDemoAudioProcessor::releaseResources()
{

}

void WaveshaperDemoAudioProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& /*midiMessages*/)
{
    if (isActive == false)
        return;

    updateProcessing();
    
    // ------------------------------------------------------------------------------------------
    const auto totalNumInputChannels  = getTotalNumInputChannels();
    const auto totalNumOutputChannels = getTotalNumOutputChannels();
    
    const auto numChannels = jmin (totalNumInputChannels, totalNumOutputChannels);
    const auto numSamples = buffer.getNumSamples();
        
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // Mix Buffer feeding
    // ------------------------------------------------------------------------------------------
    for (auto channel = 0; channel < numChannels; channel++)
        mixBuffer.copyFrom(channel, 0, buffer, channel, 0, numSamples);

    // Drive Volume
    // ------------------------------------------------------------------------------------------
    
	driveVolume.applyGain (buffer, numSamples);
    
    // Waveshaper
    // ------------------------------------------------------------------------------------------

	for (auto channel = 0; channel < numChannels; channel++)
	{
		auto* channelData = buffer.getWritePointer(channel);

		for (auto i = 0; i < numSamples; i++)
			channelData[i] = tanhAmplitude.getNextValue() * std::tanh(channelData[i] * tanhSlope.getNextValue()) + sineAmplitude.getNextValue() * std::sin(channelData[i] * sineFrequency.getNextValue());
	}

    // Mix processing
    // ------------------------------------------------------------------------------------------
    dryVolume.applyGain (mixBuffer, numSamples);
    wetVolume.applyGain (buffer, numSamples);
    
    for (auto channel = 0; channel < numChannels; channel++)
        buffer.addFrom(channel, 0, mixBuffer, channel, 0, numSamples);
    

    // Hard clipper limiter
    // ------------------------------------------------------------------------------------------
    
	for (auto channel = 0; channel < numChannels; channel++)
    {
        auto* channelData = buffer.getWritePointer (channel);
        
        for (auto i = 0; i < numSamples; i++)
            channelData[i] = jlimit (-1.f, 1.f, channelData[i]);
    }
}

void WaveshaperDemoAudioProcessor::updateProcessing()
{
    driveVolume.setValue (Decibels::decibelsToGain (*prmDrive));
    
	tanhAmplitude.setValue(*prmTanhAmp * 0.01f);
	tanhSlope.setValue(*prmTanhSlope);
	sineAmplitude.setValue(*prmSineAmp * 0.01f);
	sineFrequency.setValue(*prmSineFreq);

    auto mix = *prmMix * 0.01f;
    dryVolume.setValue (1.f - mix);
    wetVolume.setValue (mix);
}

//==============================================================================
bool WaveshaperDemoAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* WaveshaperDemoAudioProcessor::createEditor()
{
    return new WaveshaperAudioProcessorEditor (*this, parameters);
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new WaveshaperDemoAudioProcessor();
}
