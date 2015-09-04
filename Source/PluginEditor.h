#ifndef PLUGINEDITOR_H_INCLUDED
#define PLUGINEDITOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

class AlkamistMIDIHumanizerAudioProcessor;
class MainEditorWindow;

class AlkamistMIDIHumanizerAudioProcessorEditor  : public AudioProcessorEditor
{
public:

    AlkamistMIDIHumanizerAudioProcessorEditor (AlkamistMIDIHumanizerAudioProcessor& owner);
    ~AlkamistMIDIHumanizerAudioProcessorEditor();

    void paint (Graphics&) override;
    void resized() override;

private:

    AlkamistMIDIHumanizerAudioProcessor& mParentProcessor;

    ScopedPointer<MainEditorWindow> mMainEditorWindow;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AlkamistMIDIHumanizerAudioProcessorEditor)

};


#endif  // PLUGINEDITOR_H_INCLUDED