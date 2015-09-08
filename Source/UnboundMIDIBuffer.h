#ifndef UNBOUNDMIDIBUFFER_H_INCLUDED
#define UNBOUNDMIDIBUFFER_H_INCLUDED

#include <vector>

#include "../JuceLibraryCode/JuceHeader.h"
#include "TaggedMIDIMessage.h"

class UnboundMIDIBuffer
{
public:

    UnboundMIDIBuffer();
    ~UnboundMIDIBuffer();

    void addMessage (MidiMessage inputMIDIMessage, int inputSamplePosition);
    void eraseMessage (int messageNumber);
    void clear();
    
    inline bool isEmpty() { return mInternalMIDIBuffer.empty(); };
    inline int getSize() { return (unsigned int) mInternalMIDIBuffer.size(); };

    std::vector<TaggedMIDIMessage> mInternalMIDIBuffer;

    TaggedMIDIMessage& operator[] (int index);

};

#endif  // UNBOUNDMIDIBUFFER_H_INCLUDED
