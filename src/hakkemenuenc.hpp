#ifndef MENUENC_HAKKE
#define MENUENC_HAKKE

#pragma once

#include "hakkebutton.hpp"
#include "Encoder.h"
#include "hakkeoled.hpp"
#include "hakkesettings.hpp"

namespace hakke {

enum class MenuAction { NextLayer, PrevLayer, NextPage, PrevPage };

/**
 * @brief A menu encoder for hakkebræt
 * @author Mads Kjeldgaard
 * @date 23-04-2021
 */
template <int numLayers, int numPages, int numValuesPerPage>
class HakkeMenuEncoder {
private:
  int pinA, pinB;
  MenuAction leftAction, rightAction;
  Encoder encoder;
  long encoderState;
  HakkeState<numLayers, numPages, numValuesPerPage> &settings;

  int buttonPin;
  HakkeButton<numLayers, numPages, numValuesPerPage> button;

  int grain;

public:
  bool hasNewValue{false};
  // @TODO: Button!
  HakkeMenuEncoder(
      int setupPinA, int setupPinB, int butPin, int buttonNumber,
      MenuAction left, MenuAction right, ButtonAction buttonOnAction,
      ButtonAction buttonOffAction, ButtonAction buttonLongPressAction,
      HakkeState<numLayers, numPages, numValuesPerPage> &globalSettings)
      : pinA(setupPinA), pinB(setupPinB), leftAction(left), rightAction(right), encoder(setupPinA, setupPinB),
        settings(globalSettings), buttonPin(butPin),
        button(butPin, 10, buttonNumber, buttonOnAction, buttonOffAction, buttonLongPressAction,
               globalSettings){};

  void setup() {
    setGranularity(2);

    // Setup button
    button.setup();

    encoderState = encoder.read();
  }

  // How many clicks does it take to change a value?
  void setGranularity(int granularity) { grain = granularity; }

  void read() {

    // Read button
    button.read();

    constexpr auto grain = 4;
    auto reading = encoder.read();

    // Did something new happen?
    if (reading > (encoderState + grain) || reading < (encoderState - grain)) {
      hasNewValue = true;

      // Is encoder being turned left or right?
      if (reading > encoderState) {
        // RIGHT
        performAction(leftAction);
      } else {
        // LEFT
        performAction(rightAction);
      }

      // Update state
      encoderState = reading;
	  
	  // Wake up screen if sleeping
    } else {
      hasNewValue = false;
    }

  };

  void setLeftEncoderAction(MenuAction action) { leftAction = action; };
  void setRightEncoderAction(MenuAction action) { rightAction = action; };

private:
  void performAction(MenuAction action) {

#if DEBUG
    Serial.print("Menu encoder on pins ");
	Serial.print("A: ");
	Serial.print(pinA);
	Serial.print(", B: ");
	Serial.print(pinB);
#endif

    switch (action) {
    case MenuAction::PrevLayer:
      prevLayer();
      break;
    case MenuAction::NextLayer:
      nextLayer();
      break;
    case MenuAction::PrevPage:
      prevPage();
      break;
    case MenuAction::NextPage:
      nextPage();
      break;
    }
  };

  void nextLayer() {
    settings.currentLayer = (settings.currentLayer + 1) % settings.numLayers;

    // Always start at first page when entering new layer
    // @TODO Could be a setting?
    settings.currentPage = 0;
  };

  void prevLayer() {
    settings.currentLayer = ((settings.currentLayer - 1) < 0)
                                ? settings.numLayers - 1
                                : settings.currentLayer - 1;
    // Always start at first page when entering new layer
    settings.currentPage = 0;
  };

  void nextPage() {
    settings.currentPage = (settings.currentPage + 1) % settings.numPages;
  };

  void prevPage() {
    settings.currentPage = ((settings.currentPage - 1) < 0)
                               ? settings.numPages - 1
                               : settings.currentPage - 1;
  };
};
} // namespace hakke

#endif
