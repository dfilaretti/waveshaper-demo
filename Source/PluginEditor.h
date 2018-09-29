/*
  ==============================================================================

    PluginEditor.h
    Created: 20 Sep 2018 4:27:44pm
    Author:  dfila

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

class TransferFunctionGraphWidget : public Component
{
public:
	//==============================================================================
	TransferFunctionGraphWidget() {}

	~TransferFunctionGraphWidget() {}

	void paint(Graphics& g) override
	{
		int width  = getWidth();
		int height = getHeight();

		g.fillAll(Colours::darkslategrey.withMultipliedBrightness(.4));

		// X
		Array<float> x;
		int resolution = 800;

		float min = -2;
		float max = +2;
		float range = max - min;
		float delta = range / resolution;

		float v = min;
		for (int i = 0; i < resolution; i++)
		{
			x.add(v);
			v = v + delta;
		}

		// Y
		Array<float> y;
		
		for (int i = 0; i < resolution; i++)
		{
			float vNorm;
			float v = tanhAmplitude * std::tanh(x[i] * tanhSlope) + sineAmplitude * std::sin(x[i] * sineFrequency);

			// clip to -1...1
			if (v <= -1)
				vNorm = -1;
			else if ((v >= 1))
				vNorm = +1;
			else
				vNorm = v;
			y.add(vNorm);
		}

		// PATH
		Path t;
		
		t.startNewSubPath(x[0], y[0]);
		for (int i = 1; i < resolution; i++)
			t.lineTo(x[i], y[i]);

		t.applyTransform(AffineTransform::scale(80.0f, 80.0f));

		t.applyTransform(AffineTransform::translation(width / 2, height / 2));

		t.applyTransform(AffineTransform::verticalFlip(height));

		g.setColour(Colours::white);

		g.strokePath(t, PathStrokeType(1.0f));
	}

	void resized() override
	{
		// This is called when the MainContentComponent is resized.
		// If you add any child components, this is where you should
		// update their positions.
	}

	void setTanhAmp(float v)
	{
		tanhAmplitude = v / 100.f;
	}

	void setTanhSlope(float v)
	{
		tanhSlope = v;
	}

	void setSineAmp(float v)
	{
		sineAmplitude = v / 100.f;
	}

	void setSineFreq(float v)
	{
		sineFrequency = v;
	}

private:
	// params
	float tanhAmplitude, tanhSlope, sineAmplitude, sineFrequency;

	//==============================================================================
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TransferFunctionGraphWidget)
};


class WaveshaperAudioProcessorEditor : public AudioProcessorEditor
{
public:
	typedef AudioProcessorValueTreeState::SliderAttachment SliderAttachment;

	enum
	{
		paramControlHeight = 35,
		paramLabelWidth = 80,
		paramSliderWidth = 200
	};

	// Constructor and destructor
	WaveshaperAudioProcessorEditor(WaveshaperDemoAudioProcessor&, AudioProcessorValueTreeState&);
	~WaveshaperAudioProcessorEditor();

	// Overrider
	void paint(Graphics&) override;
	void resized() override;
private:
	
	// remove?
	AudioProcessorValueTreeState& valueTreeState;


	TransferFunctionGraphWidget transferFunctionGraph;

	void OnWaveshapeParamsChanged();

	LookAndFeel_V4 lookAndFeel1, lookAndFeel2, lookAndFeel3;
	
	// labels
	Label driveLabel,
		mixLabel,
		tanhAmpLabel,
		tanhSlopeLabel,
		sineAmpLabel,
		sineFreqLabel;

	// sliders
	Slider driveSlider,
		mixSlider,
		tanhAmpSlider,
		tanhSlopeSlider,
		sineAmpSlider,
		sineFreqSlider;

	// attachments
	std::unique_ptr<SliderAttachment> driveSliderAttachment,
		mixSliderAttachment,
		tanhAmpSliderAttachment,
		tanhSlopeSliderAttachment,
		sineAmpSliderAttachment,
		sineFreqSliderAttachment;

	// This reference is provided as a quick way for your editor to
	// access the processor object that created it.
	WaveshaperDemoAudioProcessor& processor;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(WaveshaperAudioProcessorEditor)
};