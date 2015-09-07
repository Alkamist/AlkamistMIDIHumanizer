#ifndef MIDIPROCESSOR_H_INCLUDED
#define MIDIPROCESSOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

class MIDIProcessor
{
public:

    MIDIProcessor();
    ~MIDIProcessor();

    void processPerBlock();
    void processPerSample();

    inline void setMIDIBuffer (MidiBuffer& inputMIDIBuffer) { mInputMIDIBuffer = &inputMIDIBuffer; };

private:

    MidiBuffer* mInputMIDIBuffer;
    MidiBuffer mOutputMIDIBuffer;
    MidiBuffer::Iterator* mMIDIMessagesIterator;

    MidiBuffer mEmptyMIDIBuffer;

    MidiMessage mCurrentMIDIMessage;
    int mCurrentMIDIMessageSamplePosition;

    int mCurrentSampleIndex;
};

#endif  // MIDIPROCESSOR_H_INCLUDED
