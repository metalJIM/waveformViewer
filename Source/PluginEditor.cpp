/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
WaveformViewerAudioProcessorEditor::WaveformViewerAudioProcessorEditor (WaveformViewerAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    addAndMakeVisible(audioProcessor.waveViewer);
    audioProcessor.waveViewer.setOpaque(false);
    audioProcessor.waveViewer.setColours(juce::Colours::black, juce::Colours::orange.withAlpha(0.5f));
    setSize (400, 400);

    addAndMakeVisible(waveZoom);
    waveZoom.setRange(32, 1024, 1.0);
    waveZoom.setTextValueSuffix(" S");
    waveZoom.setTextBoxStyle(juce::Slider::TextBoxLeft, false, waveZoom.getTextBoxWidth(), waveZoom.getTextBoxHeight());

    addAndMakeVisible(zoomLabel);
    zoomLabel.setText("Zoom", juce::dontSendNotification);
    zoomLabel.attachToComponent(&waveZoom, true);

    waveZoom.onValueChange = [this]()  //Lambda will run whenever there is a valueChange
    {
        audioProcessor.waveViewer.setBufferSize(waveZoom.getValue());
    };

    addAndMakeVisible(channelToggle);
    channelToggle.setButtonText("Mono/Stereo");
    channelToggle.changeWidthToFitText();
    channelToggle.onClick = [this]()
    {
        //Check to see if we have a single channel input or 2 channel input. 
        if(audioProcessor.getCurrentNumChannels() > 1)
        { 
            channelToggle.getToggleState() ? audioProcessor.waveViewer.setNumChannels(2) : audioProcessor.waveViewer.setNumChannels(1);
        }
    };
}

WaveformViewerAudioProcessorEditor::~WaveformViewerAudioProcessorEditor()
{
}

//==============================================================================
void WaveformViewerAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll(juce::Colours::black.brighter(0.1));

    g.setColour (juce::Colours::white);
    g.setFont (36.0f);
    g.drawFittedText ("Waveform Viewer", getLocalBounds(), juce::Justification::centredTop, 1);
}

void WaveformViewerAudioProcessorEditor::resized()
{
    audioProcessor.waveViewer.setBounds(getLocalBounds().withSizeKeepingCentre(getWidth(), getHeight() * 0.5));
    auto sliderLeft = 80;
    auto buttonCenter = getWidth();
    waveZoom.setBounds(sliderLeft, 350, getWidth() - sliderLeft - 10, 20);
    channelToggle.setBounds(160, 310, getWidth() - sliderLeft - 10, 20);
}
