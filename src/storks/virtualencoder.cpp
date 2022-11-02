#include "virtualencoder.hpp"
#include "usb_midi.h"

namespace storkspace {

// extern usb_midi_class usbMIDI;

void VirtualEncoder::print() const {
  // Serial.print("Value 7: ");
  // Serial.println(value7, 10);

  Serial.print("Value 14: ");
  Serial.println(value14, 10);

  Serial.print("Value Float: ");
  Serial.println(valueFloat, 10);
};

void VirtualEncoder::loop() {
  if (enabled) {
    const auto values = hardwareEnc->getValues();
    // Serial.println(values.velocity14);

    if ( // values.velocity7 != pastVelocity7 ||
        values.velocity14 != pastVelocity14 ||
        values.velocityFloat != pastVelocityFloat) {

      // Accumulate
      add(values.velocity7, values.velocity14, values.velocityFloat);
      send();

      // pastVelocity7 = values.velocity7;
      pastVelocity14 = values.velocity14;
      pastVelocityFloat = values.velocityFloat;

      display->setLastTouchedEncoder(ccNum, layernumber, midiChannel,
                                     valueFloat, value14);
    }
  }
}

void VirtualEncoder::add(int velocity7, int velocity14, float velocityFloat) {
  value14 = value14 + velocity14;
  // TODO: This makes velocty7 obsolete
  // value7 = value14 >> 7;
  // value7 = value7 + velocity7;
  valueFloat = valueFloat + velocityFloat;

  // constexpr int min7val = 0;
  // constexpr int max7Val = pow(2, 7) - 1;

  constexpr int min14val = 0;
  constexpr int max14Val = pow(2, 14) - 1;

  constexpr float minFloatVal = 0.0;
  constexpr float maxFloatVal = 1.0;

  switch (wrapmode) {
  case WrapMode::Clip:
    // value7 = clipValue(value7, min7val, max7Val);
    value14 = clipValue(value14, min14val, max14Val);
    valueFloat = clipValue(valueFloat, minFloatVal, maxFloatVal);
    break;

  case WrapMode::Wrap:
    // value7 = wrapValue(value7, min7val, max7Val);
    value14 = wrapValue(value14, min14val, max14Val);
    valueFloat = wrapMinMaxFloat(valueFloat, minFloatVal, maxFloatVal);
    break;
  case WrapMode::Raw:
    break;
  }

  print();
};

void VirtualEncoder::send() const {
  if (sendMidiEnabled) {
    sendMidi();
  };

  if (sendOSCEnabled) {
    sendOSC();
  }
}

void VirtualEncoder::sendMidi() const {

  // Lower 7 bits of signal
  const auto lowBitVal = value14 & 0x7F;

  // Upper 7 bits of signal
  const auto highBitVal = (value14 >> 7) & 0x7F;

  usbMIDI.sendControlChange(ccNum + 32, lowBitVal, midiChannel+1);
  usbMIDI.sendControlChange(ccNum, highBitVal, midiChannel+1);
}

void VirtualEncoder::sendOSC() const {
  osc->sendEncoder(valueFloat, midiChannel, layernumber, ccNum);
}

} // namespace storkspace
