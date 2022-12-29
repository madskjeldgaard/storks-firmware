#pragma once
#include "storks/storksmath.hpp"
#ifndef VIRTBUTTON
#define VIRTBUTTON

#include "oschost.hpp"
#include "storks/mux.hpp"
#include "storks/scaleintervals.hpp"
#include <memory>
#include <algorithm>

namespace storkspace {

const static ScaleIntervals scaleintervals;

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
    // setMidiNoteNumber(noteNum);

	// std::unique_ptr<std::vector<int>> newChord = std::make_unique<std::vector<int>>() ;
	auto newChord = makeChordVector();
	newChord->push_back(noteNum);
	newChord->push_back(noteNum + 4);
	newChord->push_back(noteNum + 8);
	setMidiNoteChord(newChord);

    layernumber = layerNum;

    Serial.printf("Setting up button %i in layer %i HEHE \n",
                  hardwareButtonIndex, layerNum);
  };

  // Update the object
  void loop();

  // Send MIDI and OSC values
  void send() const;
  void sendOSC() const;
  void sendMidiNote() const;

  /**
   * @brief Set individual midi note to be sent off when button is pressed
   *
   * @param newMidiNote 7 bit midi note value
   */

  void setMidiNoteNumber(const int newMidiNote) {
    const auto maxMidiNum = 127;
    const auto minMidiNum = 0;

    // Remove old chord if any
    midiNotes.clear();

    midiNotes.push_back(clipValue(newMidiNote, minMidiNum, maxMidiNum));
  }

  /**
   * @brief Set a midi chord to be sent off when button is pressed
   *
   * @param chord A list of 7 bit midi note values
   */
  void setMidiNoteChord(ChordVector &chord) {
    const auto maxMidiNum = 127;
    const auto minMidiNum = 0;

    // Remove old chord if any
    midiNotes.clear();

    const auto setFunc = [this](const int &chordMidiNote) {
      const auto clippedChordMidiNote = clipValue(chordMidiNote, minMidiNum, maxMidiNum);
      midiNotes.push_back(clippedChordMidiNote);
	  Serial.println(clippedChordMidiNote);
    };

	// FIXME: transform instead
    std::for_each(cbegin(*chord), cend(*chord), setFunc);

  }

  void enable(bool onOrOff) { enabled = onOrOff; };

private:
  std::vector<int> midiNotes{60};
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
