#ifndef UNBOUNDMIDIBUFFER_H_INCLUDED
#define UNBOUNDMIDIBUFFER_H_INCLUDED

#include <vector>

#include "../JuceLibraryCode/JuceHeader.h"

class UnboundMIDIBuffer
{
public:

    UnboundMIDIBuffer();
    ~UnboundMIDIBuffer();

    void addMessage (MidiMessage inputMIDIMessage, int inputSamplePosition);
    void clear();
    
    inline bool isEmpty() { return internalMIDIBuffer.empty(); };
    inline int getSize() { return internalMIDIBuffer.size(); };

private:

    struct TaggedMIDIMessage
    {
        TaggedMIDIMessage (MidiMessage inputMIDIMessage, int inputSamplePosition)
            : message (inputMIDIMessage),
              samplePosition (inputSamplePosition)
        {}
              
        MidiMessage message;
        int samplePosition;
    };

    std::vector<TaggedMIDIMessage> mInternalMIDIBuffer;

};

#endif  // UNBOUNDMIDIBUFFER_H_INCLUDED
