#pragma once
#include "storks/common.hpp"
#ifndef MUXBUTTONS
#define MUXBUTTONS

namespace storkspace {

/**
 * @brief Mux
 * @author Mads Kjeldgaard
 * @date 08-10-2022
 * @details Multiplexer interface (for reading button values)
 */

class Mux {
private:
  int add_0 = 30;
  int add_1 = 29;
  int add_2 = 28;
  int Z_pin = 27;

public:
  // Mux();
  // ~Mux();

  int muxvals[8]{0, 0, 0, 0, 0, 0, 0, 0};

  int controlPin[3]{add_0, add_1, add_2};

  void setup() {

    Serial.println("Setting up multiplexer");

    pinMode(add_0, INPUT);
    pinMode(add_1, INPUT);
    pinMode(add_2, INPUT);

    pinMode(Z_pin, INPUT);

    digitalWrite(add_0, LOW);
    digitalWrite(add_1, LOW);
    digitalWrite(add_2, LOW);
  };

  // const std::array<int, numHardwareButtons> *getVals() const {
  //   return &muxvals;
  // };

  void loop() {
    for (int muxChan = 0; muxChan < 8; muxChan++) {
      const auto reading = readMuxDigital(muxChan);

      if (reading != muxvals[muxChan]) {
        muxvals[muxChan] = reading;
      }

      delay(1);
    }
  };

  int readMuxDigital(int channel) const {

    constexpr int muxChannel[8][3] = {
        {0, 0, 0}, // channel 0
        {1, 0, 0}, // channel 1
        {0, 1, 0}, // channel 2
        {1, 1, 0}, // channel 3
        {0, 0, 1}, // channel 4
        {1, 0, 1}, // channel 5
        {0, 1, 1}, // channel 6
        {1, 1, 1}, // channel 7
    };

    // loop through the 3 sig
    for (int controlPinNum = 0; controlPinNum < 3; controlPinNum++) {
      digitalWrite(controlPin[controlPinNum],
                   muxChannel[channel][controlPinNum]);
    }

    // read the value at the Z pin
    return digitalRead(Z_pin);
  };

private:
};

} // namespace storkspace
#endif
