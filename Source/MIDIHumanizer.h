#ifndef MIDIHUMANIZER_H_INCLUDED
#define MIDIHUMANIZER_H_INCLUDED

#include <vector>

#include "../JuceLibraryCode/JuceHeader.h"
#include "CompleteMIDINote.h"
#include "UnboundMIDIBuffer.h"

class MIDIHumanizer
{
public:

    MIDIHumanizer();
    ~MIDIHumanizer();

    void processMIDIBuffer (MidiBuffer& inputMIDIBuffer);
    void reset (double inputSampleRate, int inputBlockSize);

    // Input is ms.
    void setTimingRange (std::vector<float> inputVector);

    // Input is 4 bit Velocity (0-127).
    inline void setVelocityRange (std::vector<float> inputVector) { mVelocityRange = inputVector; };

    // Input is samples.
    inline void setMaximumDelayTime (double input)                { mMaximumDelayTimeInSamples = input; };

private:

    static const int MAX_NUMBER_OF_KEYS = 128;
    double mSampleOffsetBuffer[MAX_NUMBER_OF_KEYS];

    double mSampleRate;
    int mBlockSize;

    UnboundMIDIBuffer mUnboundMIDIBuffer;
    MidiBuffer mHumanizedMIDIBuffer;
    MidiBuffer mOtherMIDIEvents;
    std::vector<TaggedMIDIMessage> mOtherMIDIExtendedBuffer;

    Random mRandomTiming;
    Random mRandomVelocity;

    void pushMessageFromBuffer (TaggedMIDIMessage& inputMessage);

    std::vector<float> mTimingRangeInSamples;
    std::vector<float> mVelocityRange;

    double mMaximumDelayTimeInSamples;

};

#endif // MIDIHUMANIZER_H_INCLUDED
