#pragma once
#ifndef VIRTBUTTON
#define VIRTBUTTON

#include "storks/mux.hpp"
#include "oschost.hpp"

namespace storkspace {

/**
 * @brief VirtualButton
 * @author Mads Kjeldgaard
 * @date 08-10-2022
 * @details A virtual representation of a hardware button. This is responsible
 * of polling values and sending them.
 */

class VirtualButton {
public:
  // VirtualButton();
  // ~VirtualButton();

  void setup(const Mux *buttonMultiplex, int hardwareButtonIndex, int noteNum,
             int chan, int layerNum, OSCHost *oschost) {
	buttonMux = buttonMultiplex;
    osc = oschost;
    index = hardwareButtonIndex;
    midiChannel = chan;
    midiNoteNum = noteNum;
    layernumber = layerNum;

	Serial.printf("Setting up button %i in layer %i HEHE \n", hardwareButtonIndex, layerNum);
  };

  // Update the object
  void loop();

  // Send MIDI and OSC values
  void send() const;
  void sendOSC() const;
  void sendMidi() const;

  void enable(bool onOrOff) { enabled = onOrOff; };

private:
  unsigned int midiNoteNum;
  unsigned int layernumber;
  unsigned int midiChannel;
  unsigned int index;

  int value{0};

  const Mux *buttonMux;

  bool enabled{false}, sendOSCEnabled{true}, sendMidiEnabled{true};
  OSCHost *osc;
};

} // namespace storkspace
#endif
