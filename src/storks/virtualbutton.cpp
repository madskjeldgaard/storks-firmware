#include "virtualbutton.hpp"

namespace storkspace {

void VirtualButton::loop() {

  if (enabled) {
    const auto newValue = buttonMux->muxvals[index];

    if (newValue != value) {
      send();

	  Serial.printf("Reading button at index %i val %i\n", index, newValue);
      value = newValue;
    }
  }
}

void VirtualButton::send() const {
  if (sendMidiEnabled) {
    sendMidi();
  };

  if (sendOSCEnabled) {
    sendOSC();
  }
}

void VirtualButton::sendMidi() const {

  constexpr auto velocity = 127;
  if (value == 1) {
    usbMIDI.sendNoteOn(midiNoteNum, velocity, midiChannel+1);
  } else {
    usbMIDI.sendNoteOff(midiNoteNum, velocity, midiChannel+1);
  }
}

void VirtualButton::sendOSC() const {
  osc->sendButton(value, midiChannel, layernumber, midiNoteNum);
}
} // namespace storkspace
