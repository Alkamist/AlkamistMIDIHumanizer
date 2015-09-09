#include "CompleteMIDINote.h"

TaggedMIDIMessage::TaggedMIDIMessage()
{
    clear();
}

TaggedMIDIMessage::TaggedMIDIMessage (bool isItAPlaceholder)
{
    clear();
    isPlaceholder = isItAPlaceholder;
}

TaggedMIDIMessage::TaggedMIDIMessage (MidiMessage inputMessage,
                                      int inputSamplePosition,
                                      bool isItAPlaceholder)
    : message (inputMessage),
      samplePosition (inputSamplePosition),
      isPlaceholder (isItAPlaceholder)
{}

TaggedMIDIMessage::TaggedMIDIMessage (const TaggedMIDIMessage& inputMessage)
    : message (inputMessage.message),
      samplePosition (inputMessage.samplePosition),
      isPlaceholder (inputMessage.isPlaceholder)
{}

void TaggedMIDIMessage::clear()
{
    MidiMessage defaultMIDIMessage;

    message = defaultMIDIMessage;
    samplePosition = 0;
    isPlaceholder = false;
}

//===================================================================

CompleteMIDINote::CompleteMIDINote (TaggedMIDIMessage inputNoteOn,
                                    TaggedMIDIMessage inputNoteOff)
    : noteOn (inputNoteOn),
      noteOff (inputNoteOff)
{}