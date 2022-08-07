#pragma once
#ifndef STORKSLAYER
#define STORKSLAYER
#include "common.hpp"
#include "mux.hpp"
#include "storks/storksdisplay.hpp"
#include "virtualbutton.hpp"
#include "virtualencoder.hpp"
#include <array>

/**
 * @brief Layer
 * @author Mads Kjeldgaard
 * @date 04-10-2022
 * @details A layer containing 16 virtual encoders and 8 virtual
 */

namespace storkspace {

class Layer {
private:
  std::array<VirtualEncoder, numHardwareEncoders> virtualEncoders;
  std::array<VirtualButton, numHardwareButtons> virtualButtons;

  bool enabled;

  int layerNumber, midiChannel;

public:
  OSCHost *osc;
  void setup(
      const std::array<HardwareEncoder, numHardwareEncoders> &hardwareEncoders,
      const Mux *buttonMux, const int midiChan, const int layerNum,
      OSCHost *oschost, StorksDisplay *storksdisplay);

  void loop();

  void enable(const bool onOrOff) {
    enabled = onOrOff;
    for (std::size_t virtencNum = 0; virtencNum < virtualEncoders.size();
         virtencNum++) {
      virtualEncoders[virtencNum].enable(enabled);
    }

    for (std::size_t virtbuttonNum = 0; virtbuttonNum < virtualButtons.size();
         virtbuttonNum++) {
      virtualButtons[virtbuttonNum].enable(enabled);
    }
  };
};

}; // namespace storkspace
#endif
