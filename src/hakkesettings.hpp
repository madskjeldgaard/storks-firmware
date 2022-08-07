#ifndef HAKKESETTINGS
#define HAKKESETTINGS
#pragma once

#include <NativeEthernet.h>
#include <NativeEthernetUdp.h>
#include "hakkedata.hpp"
#include "hakkeglobals.hpp"
#include "hakkeoled.hpp"
#include "midisender.hpp"
#include <Arduino.h>

namespace hakke {

constexpr int maxMidiVal14Bit = 16383;
constexpr int maxMidiVal7Bit = 127;

constexpr int large14bitval = maxMidiVal14Bit / 128;
constexpr int huge14bitval = maxMidiVal14Bit / 20;

enum class IncrementType { Small, Large, Huge };
/**
 * @brief Settings object for Hakkebræt
 * @author Mads Kjeldgaard
 * @date 22-04-2021
 */
template <int knumLayers, int knumPages, int knumEncoders> class HakkeState {
public:
  HakkeState(){};
  HakkeData<knumLayers, knumPages, knumEncoders> data;

  IncrementType incrementType;

  ButtonSetup buttonSetup{ButtonSetup::Midi};
  // screen stuff
  unsigned long sleepTime;
  bool screenNeedsUpdate;
  ScreenView screenview;
  ValueStyle valstyle;

  int maxValue;

  int largeIncrement, smallIncrement, hugeIncrement;
  int incrementSize;

  int currentLayer = 0;
  int currentPage = 0;

  int numLayers = knumLayers;
  int numPages = knumPages;
  int numEncoders = knumEncoders;
  int numButtons;

  // OSC
  int devicePort, targetPort;
  IPAddress deviceIp, targetIp;

  // Midi processing mode
  MIDIMODE mode;

  bool wrapMode = false;

  bool newEncoderValue{false}, newButtonValue{false}, newMenuEncoderVal{false};
  unsigned long lastUpdate{0};

  /* HakkeState(){}; */

  void setup(int knumButtons, IncrementType incType, MIDIMODE midimode) {
    numButtons = knumButtons;
    incrementType = incType;
    setMidiMode(midimode);

    screenview = ScreenView::EncoderValue;
    valstyle = ValueStyle::FloatingPoint;
    screenNeedsUpdate = false;
    sleepTime = 10000;

	// Setup osc stuff
	devicePort = 8888;
	targetPort = 9999;
	deviceIp = IPAddress(192, 168, 0, 177);
	targetIp = IPAddress(192,168,0,149);

  };

  void setMidiMode(MIDIMODE midimode) {
    mode = midimode;

    if (midimode == MIDIMODE::MODE_NORMAL) {

      maxValue = maxMidiVal7Bit;

      smallIncrement = 1;
      largeIncrement = 2;
      hugeIncrement = maxMidiVal7Bit / 20;
    } else if (midimode == MIDIMODE::MODE_14BIT) {
      maxValue = maxMidiVal14Bit;
      smallIncrement = 1;
      largeIncrement = large14bitval;
      hugeIncrement = huge14bitval;
    }

    // Update current increment size
    setIncrement(incrementType);
  }

  void cycleMidiMode() {
    if (mode == MIDIMODE::MODE_14BIT) {
      setMidiMode(MIDIMODE::MODE_NORMAL);
    } else if (mode == MIDIMODE::MODE_NORMAL) {
      setMidiMode(MIDIMODE::MODE_14BIT);
    }
  }

  // Set current increment size
  void setIncrement(IncrementType incType) {
    incrementType = incType;

    switch (incType) {
    case IncrementType::Small:
      incrementSize = smallIncrement;
      break;
    case IncrementType::Large:
      incrementSize = largeIncrement;
      break;
    case IncrementType::Huge:
      incrementSize = hugeIncrement;
      break;
    }
  };
  void print() {
    Serial.print("largeIncrement: ");
    Serial.println(largeIncrement);

    Serial.print("smallIncrement: ");
    Serial.println(smallIncrement);

    Serial.print("currentLayer: ");
    Serial.println(currentLayer);

    Serial.print("currentPage: ");
    Serial.println(currentPage);

    Serial.print("numLayers: ");
    Serial.println(numLayers);

    Serial.print("numPages: ");
    Serial.println(numPages);

    Serial.print("numEncoders: ");
    Serial.println(numEncoders);

    Serial.print("Mode: ");
    if (mode == MIDIMODE::MODE_NORMAL) {
      Serial.println("7 bit midi");
    } else if (mode == MIDIMODE::MODE_14BIT) {
      Serial.println("14 bit midi");
    }
  };
  /// Read by the oled class to figure out what screen to display
  /* ScreenView currentScreen; */

  /* void progressTimer() { time = millis(); } */
  /* private: */
  /*   int time; */
};
} // namespace hakke

#endif
