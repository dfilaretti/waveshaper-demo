/*
  ==============================================================================

    PluginEditor.cpp
    Created: 20 Sep 2018 4:27:44pm
    Author:  dfila

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

WaveshaperAudioProcessorEditor::WaveshaperAudioProcessorEditor(WaveshaperDemoAudioProcessor& p, AudioProcessorValueTreeState& vts)
	: AudioProcessorEditor(&p), processor(p), valueTreeState(vts)
{
	lookAndFeel1.setColour(Slider::trackColourId, Colours::lightgreen);
	lookAndFeel1.setColour(Slider::thumbColourId, Colours::green);
	lookAndFeel2.setColour(Slider::trackColourId, Colours::lightpink);
	lookAndFeel2.setColour(Slider::thumbColourId, Colours::salmon);
	lookAndFeel3.setColour(Slider::trackColourId, Colours::lightblue);
	lookAndFeel3.setColour(Slider::thumbColourId, Colours::blue );

	driveLabel.setText("Drive", dontSendNotification);
	mixLabel.setText("Mix", dontSendNotification);
	tanhAmpLabel.setText("Tanh amp", dontSendNotification);
	tanhSlopeLabel.setText("Tanh slope", dontSendNotification);
	sineAmpLabel.setText("Sin amp", dontSendNotification);
	sineFreqLabel.setText("Sin freq", dontSendNotification);

	addAndMakeVisible(transferFunctionGraph);

	addAndMakeVisible(driveLabel);
	addAndMakeVisible(mixLabel);
	addAndMakeVisible(tanhAmpLabel);
	addAndMakeVisible(tanhSlopeLabel);
	addAndMakeVisible(sineAmpLabel);
	addAndMakeVisible(sineFreqLabel);

	driveSlider.setLookAndFeel(&lookAndFeel1);
	addAndMakeVisible(driveSlider);
	driveSliderAttachment.reset(new SliderAttachment(valueTreeState, "DRIVE", driveSlider));

	mixSlider.setLookAndFeel(&lookAndFeel1);
	addAndMakeVisible(mixSlider);
	mixSliderAttachment.reset(new SliderAttachment(valueTreeState, "MIX", mixSlider));

	tanhAmpSlider.setLookAndFeel(&lookAndFeel2);
	addAndMakeVisible(tanhAmpSlider);
	tanhAmpSliderAttachment.reset(new SliderAttachment(valueTreeState, "TANHAMP", tanhAmpSlider));
	tanhAmpSlider.onValueChange = [this] { OnWaveshapeParamsChanged(); };

	tanhSlopeSlider.setLookAndFeel(&lookAndFeel2);
	addAndMakeVisible(tanhSlopeSlider);
	tanhSlopeSliderAttachment.reset(new SliderAttachment(valueTreeState, "TANHSLOPE", tanhSlopeSlider));
	tanhSlopeSlider.onValueChange = [this] { OnWaveshapeParamsChanged(); };

	sineAmpSlider.setLookAndFeel(&lookAndFeel3);
	addAndMakeVisible(sineAmpSlider);
	sineAmpSliderAttachment.reset(new SliderAttachment(valueTreeState, "SINEAMP", sineAmpSlider));
	sineAmpSlider.onValueChange = [this] { OnWaveshapeParamsChanged(); };

	sineFreqSlider.setLookAndFeel(&lookAndFeel3);
	addAndMakeVisible(sineFreqSlider);
	sineFreqSliderAttachment.reset(new SliderAttachment(valueTreeState, "SINEFREQ", sineFreqSlider));
	sineFreqSlider.onValueChange = [this] { OnWaveshapeParamsChanged(); };

	setSize(400, 420);
}

void WaveshaperAudioProcessorEditor::OnWaveshapeParamsChanged()
{
	float newTanhAmp   = tanhAmpSlider.getValue();
	float newTanhSlope = tanhSlopeSlider.getValue();
	float newSinAmp    = sineAmpSlider.getValue();
	float newSinFreq   = sineFreqSlider.getValue();

	transferFunctionGraph.setTanhAmp(newTanhAmp);
	transferFunctionGraph.setTanhSlope(newTanhSlope);
	transferFunctionGraph.setSineAmp(newSinAmp);
	transferFunctionGraph.setSineFreq(newSinFreq);

	transferFunctionGraph.repaint();
}

WaveshaperAudioProcessorEditor::~WaveshaperAudioProcessorEditor()
{
}

void WaveshaperAudioProcessorEditor::paint(Graphics& g)
{
	g.fillAll(Colours::darkslategrey.withMultipliedBrightness(.4));
}

void WaveshaperAudioProcessorEditor::resized()
{
	auto r = getLocalBounds();

	auto waveformRect = r.removeFromTop(200);
	transferFunctionGraph.setBounds(waveformRect);

	// ------------------
	// Mix / Drive
	// ------------------

	auto driveRect = r.removeFromTop(paramControlHeight);
	driveLabel.setBounds(driveRect.removeFromLeft(paramLabelWidth));
	driveSlider.setBounds(driveRect);

	auto mixRect = r.removeFromTop(paramControlHeight);
	mixLabel.setBounds(mixRect.removeFromLeft(paramLabelWidth));
	mixSlider.setBounds(mixRect);

	// ------------------
	// Waveshape controls
	// ------------------

	auto tanhAmpRect = r.removeFromTop(paramControlHeight);
	tanhAmpLabel.setBounds(tanhAmpRect.removeFromLeft(paramLabelWidth));
	tanhAmpSlider.setBounds(tanhAmpRect);

	auto tanhSlopeRect = r.removeFromTop(paramControlHeight);
	tanhSlopeLabel.setBounds(tanhSlopeRect.removeFromLeft(paramLabelWidth));
	tanhSlopeSlider.setBounds(tanhSlopeRect);

	auto sineAmpRect = r.removeFromTop(paramControlHeight);
	sineAmpLabel.setBounds(sineAmpRect.removeFromLeft(paramLabelWidth));
	sineAmpSlider.setBounds(sineAmpRect);

	auto sineFreqRect = r.removeFromTop(paramControlHeight);
	sineFreqLabel.setBounds(sineFreqRect.removeFromLeft(paramLabelWidth));
	sineFreqSlider.setBounds(sineFreqRect);

	OnWaveshapeParamsChanged();
}