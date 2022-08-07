#ifndef MIDIHAKKE
#define MIDIHAKKE
#include "hakkesettings.hpp"
#pragma once
#include "hakkeglobals.hpp"
#include "usb_midi.h"

namespace hakke {

extern usb_midi_class usbMIDI;

/**
 * @brief Midi sender class for Hakkebræt
 * @author Mads Kjeldgaard
 * @date 22-04-2021
 */
template <int numLayers, int numPages, int numEncs> class HakkeMidiSender {

public:
  HakkeState<numLayers, numPages, numEncs> &settings;

  // Midi channel
  int channel;

  // This will be interpreted as the "lower" bit if in 14 bit mode
  int ccNum;

  HakkeMidiSender(HakkeState<numLayers, numPages, numEncs> &globalSettings)
      : settings(globalSettings) {
  };

  void sendCC(int value, int layer, int page, int encNum, int numEncoders) {
    calculateCCNum(encNum, layer, page, numEncoders);

    switch (settings.mode) {
    case MIDIMODE::MODE_NORMAL:
      usbMIDI.sendControlChange(ccNum, value, channel + 1);
      break;
    case MIDIMODE::MODE_14BIT:
      send14BitMidi(ccNum, value, channel + 1);
      break;
    }
  }

private:
  // This will calculate the correct midi cc number based on the current page,
  // layer and encoder
  inline void calculateCCNum(int encNum, int layer, int page, int numEncoders) {
    channel = layer;
    ccNum = pageToMidiNum(encNum, page, numEncoders);
  }

  // Expects 14 bit values in range 0 - 16383
  void send14BitMidi(int lowerccnum, int value, int channel) {
    // Throw away everything above 14 bits;
    /* value = value << 14; */

    // Clip values
    if (value > settings.maxValue) {
      value = settings.maxValue;
    } else if (value < 0) {
      value = 0;
    }

    // Lower 7 bits of signal
    auto lowBitVal = value & 0x7F;

    // Upper 7 bits of signal
    auto highBitVal = (value >> 7) & 0x7F;

    usbMIDI.sendControlChange(ccNum + 32, lowBitVal, channel);
    usbMIDI.sendControlChange(ccNum, highBitVal, channel);
  }
};
} // namespace hakke

#endif
