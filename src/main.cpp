#include "hakkebraet.hpp"

#define DEBUG false

using namespace hakke;

/* settings.numLayers = 4; */
constexpr int kNumLayers = 16;
constexpr int kNumPages = 8;
constexpr int kNumEncoders = 16;
constexpr int kNumButtons = 0;
constexpr int kNumMenuEncoders = 1;
Hakkebraet<kNumLayers, kNumPages, kNumEncoders, kNumMenuEncoders, kNumButtons>
    hakkebraet;

void setup() {
  hakkebraet.setup();

#if DEBUG
  while (!Serial)
    ;
#endif
}

void loop() {
  hakkebraet.loop();

#if DEBUG
  delay(25);
#endif
}
