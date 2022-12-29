#include "layer.hpp"

namespace storkspace {

const static ScaleIntervals intervals;

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
    constexpr auto middleCMidiNote = 60;
    constexpr auto baseMidiNote = middleCMidiNote;

    // The midi note associated with this button
    const auto noteNum = intervals.midinoteWithScaleInterval(
        baseMidiNote, "Major", hardwareButtonIndex);

    virtualButtons[virtbuttonNum].setup(buttonMux, hardwareButtonIndex, noteNum,
                                        midiChan, layerNumber, osc);
  };
}

void Layer::loop() {
  // Loop through encoders
  std::for_each(
      begin(virtualEncoders), end(virtualEncoders),
      [this](VirtualEncoder &virtualEncoder) { virtualEncoder.loop(); });

  // Loop through buttons
  std::for_each(begin(virtualButtons), end(virtualButtons),
                [this](VirtualButton &virtualButton) { virtualButton.loop(); });
}

}  // namespace storkspace
