#include "TaggedMIDIMessage.h"

TaggedMIDIMessage::TaggedMIDIMessage (MidiMessage inputMIDIMessage, int inputSamplePosition)
    : message (inputMIDIMessage),
      samplePosition (inputSamplePosition)
{}