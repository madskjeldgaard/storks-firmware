#include "layer.hpp"

namespace storkspace {

void Layer::setup(
    const std::array<HardwareEncoder, numHardwareEncoders> &hardwareEncoders,
    const Mux *buttonMux, const int midiChan, const int layerNum,
    OSCHost *oschost, StorksDisplay *storksdisplay)

{
  osc = oschost;
  layerNumber = layerNum;
  midiChannel = midiChan;

  // Setup virtual encoders
  for (std::size_t virtencNum = 0; virtencNum < virtualEncoders.size();
       virtencNum++) {
    const auto hardwareEncIndex = virtencNum % numHardwareEncoders;
    const auto ccnum = hardwareEncIndex + (layerNum * numHardwareEncoders);
    const auto wrapmode = WrapMode::Clip;

    virtualEncoders[virtencNum].setup(&hardwareEncoders[hardwareEncIndex],
                                      ccnum, midiChan, layerNumber, wrapmode,
                                      osc, storksdisplay);
  };

  // Setup virtual buttons
  for (std::size_t virtbuttonNum = 0; virtbuttonNum < virtualButtons.size();
       virtbuttonNum++) {
    const auto hardwareButtonIndex = virtbuttonNum % numHardwareButtons;
    const auto noteNum = 44 + hardwareButtonIndex;
    // Serial.printf("Setting up button with hardware index %i layer %i \n",
    // hardwareButtonIndex, layerNumber);

    virtualButtons[virtbuttonNum].setup(buttonMux, hardwareButtonIndex, noteNum,
                                        midiChan, layerNumber, osc);
  };
}

void Layer::loop() {
  for (std::size_t virtencNum = 0; virtencNum < virtualEncoders.size();
       virtencNum++) {
    virtualEncoders[virtencNum].loop();
  };

  for (std::size_t virtbuttonNum = 0; virtbuttonNum < virtualButtons.size();
       virtbuttonNum++) {
    virtualButtons[virtbuttonNum].loop();
  };
}

}  // namespace storkspace
