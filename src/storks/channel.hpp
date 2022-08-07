#pragma once
#ifndef MIDICHANSTORK
#define MIDICHANSTORK
#include "storks/mux.hpp"
#include "storks/storksdisplay.hpp"
#include "common.hpp"
#include "layer.hpp"

/**
 * @brief Channel
 * @author Mads Kjeldgaard
 * @date 08-10-2022
 * @details An object representing a midi channel, containing layers of virtual
 * encoders
 */

namespace storkspace {
class Channel {
public:
  void setup(
      const std::array<HardwareEncoder, numHardwareEncoders> &hardwareEncoders,
      const Mux *buttonMux, const int midichan, OSCHost *oschost, StorksDisplay *storksdisplay);

  void loop();

  void enableLayer(const std::size_t newLayer) {

    if (newLayer < layers.size()) {

      // Disable previous layer
      layers[currentLayer].enable(false);

      // Enable new layer
      layers[newLayer].enable(true);

      currentLayer = newLayer;

    } else {
      Serial.println(
          "ERROR: Can't set layer higher than maximum number of layers");
    }
  }

  void toggle(const bool onOrOff) {
    enabled = onOrOff;
    for (std::size_t numLayer = 0; numLayer < layers.size(); numLayer++) {
      layers[numLayer].enable(enabled);
    }

  };

private:
  bool enabled;
  std::size_t currentLayer;
  std::array<Layer, numLayers> layers;
  OSCHost *osc;
};

} // namespace storkspace
#endif
