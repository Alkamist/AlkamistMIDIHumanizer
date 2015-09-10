#ifndef MIDIHUMANIZER_H_INCLUDED
#define MIDIHUMANIZER_H_INCLUDED

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

private:

    static const int MAX_NUMBER_OF_KEYS = 128;
    double mSampleOffsetBuffer[MAX_NUMBER_OF_KEYS];

    int mBlockSize;

    UnboundMIDIBuffer mUnboundMIDIBuffer;
    MidiBuffer mHumanizedMIDIBuffer;
    MidiBuffer mOtherMIDIEvents;

    boost::mt19937 mMersenneTwisterRNG;
    double generateNormalRandomNumber();

    void pushMessageFromBuffer (TaggedMIDIMessage& inputMessage);

    double mTimingStandardDeviationInSamples;
    double mMaximumDelayTimeInSamples;

};

#endif // MIDIHUMANIZER_H_INCLUDED
