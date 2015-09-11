#include "PluginProcessor.h"
#include "MainEditorWindow.h"

#include "PluginEditor.h"

//==============================================================================
AlkamistMIDIHumanizerAudioProcessorEditor::AlkamistMIDIHumanizerAudioProcessorEditor (AlkamistMIDIHumanizerAudioProcessor& owner)
    : AudioProcessorEditor (owner),
      mParentProcessor (owner)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (240, 128);

    addAndMakeVisible(mMainEditorWindow = new MainEditorWindow(mParentProcessor));
}

AlkamistMIDIHumanizerAudioProcessorEditor::~AlkamistMIDIHumanizerAudioProcessorEditor()
{
    mMainEditorWindow = nullptr;
}

//==============================================================================
void AlkamistMIDIHumanizerAudioProcessorEditor::paint (Graphics& g)
{
    g.fillAll (Colours::black);
}

void AlkamistMIDIHumanizerAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}
