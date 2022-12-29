#include "virtualbutton.hpp"

namespace storkspace {

// NOTE: This is a super stupid hack. The hardware buttons are hooked up to a
// mux, and not connected in the correct sequence. This allows the virtual
// button to change it's index in the mux
static std::array<int, numHardwareButtons> hardwareButtonIndices = {
    {0, 7, 6, 5, 4, 3, 2, 1}};

void VirtualButton::loop() {
  if (enabled) {
    const auto hardwareIndex = hardwareButtonIndices[index];
    const auto newValue = buttonMux->muxvals[hardwareIndex];

    if (newValue != value) {
      send();

      Serial.printf("Reading button at index %i val %i\n", index, newValue);
      value = newValue;
    }
  }
}

void VirtualButton::send() const {
  if (sendMidiEnabled) {
    sendMidiNote();
  };

  if (sendOSCEnabled) {
    sendOSC();
  }
}

void VirtualButton::sendMidiNote() const {
  constexpr auto velocity = 127;
  constexpr auto noteOnToggle = 1;
  const auto sendFunc = [this](const int &midiNote) {
    if (value == noteOnToggle) {
      usbMIDI.sendNoteOn(midiNote, velocity, midiChannel + 1);

    } else {
      usbMIDI.sendNoteOff(midiNote, velocity, midiChannel + 1);
    }
  };

  std::for_each(cbegin(midiNotes), cend(midiNotes), sendFunc);
}

void VirtualButton::sendOSC() const {
  osc->sendButton(value, midiChannel, layernumber, midiNotes[0]);
}
}  // namespace storkspace
