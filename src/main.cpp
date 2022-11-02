#include "Arduino.h"
#include "storks/storks.hpp"

// #define DEBUG

storkspace::Storks storks;

void setup() {
  delay(1000); // Seems this is necessary to ensure the hardware is ready
  storks.setup();
}

void loop() {
  storks.loop();
}
