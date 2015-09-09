#ifndef UNBOUNDMIDIBUFFER_H_INCLUDED
#define UNBOUNDMIDIBUFFER_H_INCLUDED

#include <vector>

#include "../JuceLibraryCode/JuceHeader.h"
#include "CompleteMIDINote.h"

class UnboundMIDIBuffer
{
public:

    UnboundMIDIBuffer();
    ~UnboundMIDIBuffer();

    void addNoteOn (const TaggedMIDIMessage& inputMessage);
    void addNoteOff (const TaggedMIDIMessage& inputMessage);
    void addCompleteMessage (const CompleteMIDINote& inputMessage);
    void removeCompleteMessage (int messageNumber);
    void clear();
    //void fixNoteOverlaps();
    
    inline bool isEmpty() { return mInternalNoteBuffer.empty(); };
    inline int getSize() { return (unsigned int) mInternalNoteBuffer.size(); };

    std::vector<CompleteMIDINote> mInternalNoteBuffer;

    CompleteMIDINote& operator[] (int index);

private:

};

#endif  // UNBOUNDMIDIBUFFER_H_INCLUDED
