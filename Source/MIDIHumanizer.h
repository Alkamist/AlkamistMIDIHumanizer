#ifndef MIDIHUMANIZER_H_INCLUDED
#define MIDIHUMANIZER_H_INCLUDED

#include <boost/random.hpp>
#include <boost/random/normal_distribution.hpp>

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

};

#endif // MIDIHUMANIZER_H_INCLUDED
