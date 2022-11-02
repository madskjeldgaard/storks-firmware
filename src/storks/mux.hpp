#pragma once
#include "common.hpp"
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

  void setup();

  void loop();

  int readMuxDigital(int channel) const;

private:
};

} // namespace storkspace
#endif
