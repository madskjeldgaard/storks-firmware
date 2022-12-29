#pragma once
#ifndef STORKSLAYER
#define STORKSLAYER
#include "common.hpp"
#include "mux.hpp"
#include "storks/scaleintervals.hpp"
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

    auto doOnEachEncoder = [this](VirtualEncoder &enc) {
      enc.enable(enabled);
	};

    std::for_each(begin(virtualEncoders), end(virtualEncoders), doOnEachEncoder);

	auto doOnEachButton = [this](VirtualButton &but) {
	  but.enable(enabled);
	};

    std::for_each(begin(virtualButtons), end(virtualButtons), doOnEachButton);

  };
};

}; // namespace storkspace
#endif
