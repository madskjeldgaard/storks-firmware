#include "mux.hpp"

namespace storkspace {
void Mux::setup() {
  Serial.println("Setting up multiplexer");

  pinMode(add_0, INPUT);
  pinMode(add_1, INPUT);
  pinMode(add_2, INPUT);

  pinMode(Z_pin, INPUT);

  digitalWrite(add_0, LOW);
  digitalWrite(add_1, LOW);
  digitalWrite(add_2, LOW);
}

void Mux::loop() {
  for (int muxChan = 0; muxChan < 8; muxChan++) {
    const auto reading = readMuxDigital(muxChan);

    if (reading != muxvals[muxChan]) {
      muxvals[muxChan] = reading;
    }

    delay(1);
  }
};

int Mux::readMuxDigital(int channel) const {
  constexpr int muxChannel[8][3] = {
      {0, 0, 0},  // channel 0
      {1, 0, 0},  // channel 1
      {0, 1, 0},  // channel 2
      {1, 1, 0},  // channel 3
      {0, 0, 1},  // channel 4
      {1, 0, 1},  // channel 5
      {0, 1, 1},  // channel 6
      {1, 1, 1},  // channel 7
  };

  // loop through the 3 sig
  for (int controlPinNum = 0; controlPinNum < 3; controlPinNum++) {
    digitalWrite(controlPin[controlPinNum], muxChannel[channel][controlPinNum]);
  }

  // read the value at the Z pin
  return digitalRead(Z_pin);
}

}  // namespace storkspace
