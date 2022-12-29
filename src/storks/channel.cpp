#include "channel.hpp"

namespace storkspace {

void Channel::setup(
    const std::array<HardwareEncoder, numHardwareEncoders> &hardwareEncoders,
    const Mux *buttonMux, const int midichan, OSCHost *oschost,
    StorksDisplay *storksdisplay) {
  osc = oschost;

  for (std::size_t layerNum = 0; layerNum < layers.size(); layerNum++) {
    layers[layerNum].setup(hardwareEncoders, buttonMux, midichan, layerNum, osc,
                           storksdisplay);
  }

  // Enable first layer
  currentLayer = 0;
  enableLayer(currentLayer);
};

void Channel::loop() {
  auto doOnEachLayer = [this](Layer &layer) { layer.loop(); };
  std::for_each(begin(layers), end(layers), doOnEachLayer);
};

}  // namespace storkspace
