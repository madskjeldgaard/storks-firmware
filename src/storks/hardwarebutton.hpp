#pragma once
#include "core_pins.h"
#ifndef HARDBUTTONSTORKSPACE
#define HARDBUTTONSTORKSPACE
#include <Bounce.h>

namespace storkspace {

/**
 * @brief HardwareButton
 * @author Mads Kjeldgaard
 * @date 08-10-2022
 * @details Controller for a hardware button
 */

class HardwareButton {
public:
  HardwareButton(const unsigned int pin, int debounceTime = 100)
      : buttonPin(pin), button(pin, debounceTime){

                        };

  void setup() { pinMode(buttonPin, INPUT_PULLUP); };

  void loop() {
    button.update();
    const auto reading = button.read();

    if (reading != value) {

      value = reading;
    }
  }

  const int getValue() const { return value; }

  // ~HardwareButton();

private:
  const unsigned int buttonPin;
  Bounce button;

  int value;
};

} // namespace storkspace
#endif
