#ifndef PLUGINPROCESSOR_H_INCLUDED
#define PLUGINPROCESSOR_H_INCLUDED

#include <boost/random.hpp>
#include <boost/random/normal_distribution.hpp>

#include "../JuceLibraryCode/JuceHeader.h"

class FloatParameter;

//==============================================================================
/**
*/
class AlkamistMIDIHumanizerAudioProcessor  : public AudioProcessor
{
public:

    //==============================================================================
    AlkamistMIDIHumanizerAudioProcessor();
    ~AlkamistMIDIHumanizerAudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    void processBlock (AudioSampleBuffer&, MidiBuffer&) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override                                                 { return true; };

    //==============================================================================
    const String getName() const override                                           { return JucePlugin_Name; }

    const String getInputChannelName (int channelIndex) const override;
    const String getOutputChannelName (int channelIndex) const override;
    bool isInputChannelStereoPair (int index) const override;
    bool isOutputChannelStereoPair (int index) const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool silenceInProducesSilenceOut() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override                                                   { return 1; }
    int getCurrentProgram() override                                                { return 0; }
    void setCurrentProgram (int /*index*/) override                                 {}
    const String getProgramName (int /*index*/) override                            { return "Default"; }
    void changeProgramName (int /*index*/, const String& /*newName*/) override      {}

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    //==============================================================================

    void parameterChange (FloatParameter* parameterThatWasChanged);

    //==============================================================================

    void reset();
    inline void signalForParameterChange() { mParameterChangeFlag = true; };

    FloatParameter* timingStandardDeviation;

private:

    double generateNormalRandomNumber();

    struct TaggedMIDIMessage
    {
        MidiMessage MIDIMessage;
        double samplePosition;
        bool slotIsUsed;
    };

    void handleParameterChanges();
    void clearParameterChanges();
    bool mParameterChangeFlag;

    double mTimingStandardDeviationInSamples;
    double mMaximumDelayTimeInSamples;

    double mMIDISampleOffsetBuffer[128];

    const static int taggedMIDIBufferSize = 1024;

    TaggedMIDIMessage taggedMIDIBuffer[taggedMIDIBufferSize];
    MidiBuffer humanizedMIDIBuffer;

    boost::mt19937 mMersenneTwisterGenerator;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AlkamistMIDIHumanizerAudioProcessor)
};

#endif  // PLUGINPROCESSOR_H_INCLUDED
