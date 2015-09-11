#ifndef MIDIHUMANIZER_H_INCLUDED
#define MIDIHUMANIZER_H_INCLUDED

#include <vector>
#include <boost/random.hpp>
#include <boost/random/normal_distribution.hpp>

//#include "GallantSignal.h"
#include "../JuceLibraryCode/JuceHeader.h"
#include "CompleteMIDINote.h"
#include "UnboundMIDIBuffer.h"

class MIDIHumanizer
{
public:

    MIDIHumanizer();
    ~MIDIHumanizer();

    void processMIDIBuffer (MidiBuffer& inputMIDIBuffer);
    inline void reset (int inputBlockSize) { mBlockSize = inputBlockSize; };

    //Gallant::Signal0<> parameterChangeSignal;

    inline void setTimingStandardDeviationInSamples (double input) { mTimingStandardDeviationInSamples = input; };
    inline void setMaximumDelayTimeInSamples (double input)        { mMaximumDelayTimeInSamples = input; };
    inline void setVelocityStandardDeviation (double input)        { mVelocityStandardDeviation = input; };

private:

    static const int MAX_NUMBER_OF_KEYS = 128;
    double mSampleOffsetBuffer[MAX_NUMBER_OF_KEYS];

    int mBlockSize;

    UnboundMIDIBuffer mUnboundMIDIBuffer;
    MidiBuffer mHumanizedMIDIBuffer;
    MidiBuffer mOtherMIDIEvents;
    std::vector<TaggedMIDIMessage> mOtherMIDIExtendedBuffer;

    boost::mt19937 mMersenneTwisterTiming;
    boost::mt19937 mMersenneTwisterVelocity;
    double generateNormalRandomNumber (boost::mt19937& inputRNG);

    void pushMessageFromBuffer (TaggedMIDIMessage& inputMessage);

    double mTimingStandardDeviationInSamples;
    double mMaximumDelayTimeInSamples;
    double mVelocityStandardDeviation;

};

#endif // MIDIHUMANIZER_H_INCLUDED
