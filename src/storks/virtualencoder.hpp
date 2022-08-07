#pragma once
#include "storks/storksdisplay.hpp"
#ifndef STORKSENCVIRT
#define STORKSENCVIRT
#include "hardwareencoder.hpp"
#include "oschost.hpp"
#include "storksmath.hpp"
#include <cmath>

namespace storkspace {
enum class WrapMode { Clip, Wrap, Raw };

/**
 * @brief A virtual encoder
 * @author Mads Kjeldgaard
 * @date 2022-10-02
 * @details Contains and keeps track of data set by the hardware encoders, wraps
 * it and sends it.
 */
class VirtualEncoder {
public:
  // Setup the encoder object
  void setup(const HardwareEncoder *hardwareEncoder, const int ccNumber,
             const int chan, const int layerNum, WrapMode mode,
             OSCHost *oschost, StorksDisplay *storksdisplay) {
    display = storksdisplay;

    osc = oschost;
    wrapmode = mode;
    hardwareEnc = hardwareEncoder;
    midiChannel = chan;
    ccNum = ccNumber;
    layernumber = layerNum;
  };

  // Update the encoder object
  void loop();

  // Send MIDI and OSC values
  void send() const;

  // Turn this virtual encoder on/off
  // This will make it poll values from the hardware encoder it is connected to
  void enable(bool onOrOff) { enabled = onOrOff; };

private:
  void sendMidi() const;
  void sendOSC() const;
  void print() const;

  // Accumulate from new encoder values
  void add(int velocity7, int velocity14, float velocityFloat);
  int value14, pastVelocity14;
  // int value7, pastVelocity7;
  float valueFloat, pastVelocityFloat;
  const HardwareEncoder *hardwareEnc;
  OSCHost *osc;

  // Settings
  WrapMode wrapmode;
  int midiChannel, ccNum, layernumber;
  bool enabled{false}, sendOSCEnabled{true}, sendMidiEnabled{true};

  // OLED
  StorksDisplay *display;
};

}; // namespace storkspace
#endif
