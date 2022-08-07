#include "Arduino.h"
#include "storks/storks.hpp"

// #define DEBUG

storkspace::Storks storks;

void setup() {
  storks.setup();
}

void loop() {
  storks.loop();
}
