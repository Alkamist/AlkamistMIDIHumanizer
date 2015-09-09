#ifndef COMPLETEMIDINOTE_H_INCLUDED
#define COMPLETEMIDINOTE_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

struct TaggedMIDIMessage
{
    TaggedMIDIMessage();
    TaggedMIDIMessage (bool isItAPlaceholder);
    TaggedMIDIMessage (MidiMessage inputMessage,
                       int inputSamplePosition,
                       bool isItAPlaceholder);
    TaggedMIDIMessage (const TaggedMIDIMessage& inputMessage);

    void clear();

    MidiMessage message;
    int samplePosition;
    bool isPlaceholder;
};

struct CompleteMIDINote
{
    CompleteMIDINote (TaggedMIDIMessage inputNoteOn,
                      TaggedMIDIMessage inputNoteOff);

    TaggedMIDIMessage noteOn;
    TaggedMIDIMessage noteOff;
};

#endif  // COMPLETEMIDINOTE_H_INCLUDED
