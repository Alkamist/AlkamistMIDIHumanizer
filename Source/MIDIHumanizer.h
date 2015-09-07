#ifndef MIDIHUMANIZER_H_INCLUDED
#define MIDIHUMANIZER_H_INCLUDED

#include <boost/random.hpp>
#include <boost/random/normal_distribution.hpp>

#include "../JuceLibraryCode/JuceHeader.h"
#include "UnboundMIDIBuffer.h"

class MIDIHumanizer
{
public:

    MIDIHumanizer();
    ~MIDIHumanizer();

    void processMIDIBuffer (MidiBuffer &inputMIDIBuffer, int blockSize);

private:

    UnboundMIDIBuffer mUnboundMIDIBuffer;
    MidiBuffer mHumanizedMIDIBuffer;

    boost::mt19937 mMersenneTwisterRNG;
    double generateNormalRandomNumber();

};

#endif // MIDIHUMANIZER_H_INCLUDED
