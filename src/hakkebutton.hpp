#ifndef HAKKEBUT
#define HAKKEBUT
#pragma once

#include "core_pins.h"
#include "hakkeglobals.hpp"
#include "hakkesettings.hpp"
#include "usb_midi.h"
#include <Bounce.h>

namespace hakke {

enum class ButtonAction {
  Nothing,
  // Midi
  SendNoteOn,
  SendNoteOff,
  /* SendMidiCCHigh, */
  /* SendMidiCCLow, */

  CycleMidiMode,

  // Modifiers
  HugeIncrements,
  LargeIncrements,
  SmallIncrements,

  // Info
  PrintInfo,

  // Display
  CycleNextViewStyle,
  ShowPageOverview,

  CycleNextButtonSetup,

  // Keyboard
  KeyUp,
  KeyDown,
  KeyLeft,
  KeyRight,
  KeyF5,
  KeyF12,
  KeyCtrlE, // Evaluate supercollider in vim
  KeySpace,
  KeyPeriod,
  KeyEnter,
  KeyEsc,
  KeyH,
  KeyJ,
  KeyK,
  KeyL
};

/**
 * @brief Button for hakkebræt
 * @author Mads Kjeldgaard
 * @date 22-04-2021
 */
template <int numLayers, int numPages, int numValuesPerPage> class HakkeButton {
public:
  HakkeState<numLayers, numPages, numValuesPerPage> &settings;
  bool hasNewValue{false};

  int pin;
  Bounce button;
  int buttonNumber;

  unsigned long duration, longThreshold, timeElapsed;

  ButtonAction onAction, offAction, longAction;
  /* HakkeMidiSender midisender; */

  HakkeButton(int initPin, int debounceTime, int butNumber,
              ButtonAction buttonOnAction, ButtonAction buttonOffAction,
              ButtonAction buttonLongAction, /*MIDIMODE midiMode,*/
              HakkeState<numLayers, numPages, numValuesPerPage> &globalState)
      : settings(globalState), button(initPin, debounceTime),
        buttonNumber(butNumber), onAction(buttonOnAction),
        offAction(buttonOffAction), longAction(buttonLongAction)
  /*, midisender(midiMode)*/ {
    pin = initPin;
    state = LOW;
    duration = 0;

    longThreshold = 300;
    timeElapsed = 0;
  };

  void setup() { pinMode(pin, INPUT_PULLUP); };

  void read() {
    button.update();
    auto reading = button.read();

    // If value is new, perform actions
    if (reading != state) {

      // Timer for long press action
      timeElapsed = millis() - duration;

      if (reading == HIGH) {

        // @FIXME: This also triggers main button function (should not). Move
        // out of if ?
        if (timeElapsed > longThreshold) {
          performLongPressAction();
		  settings.screenNeedsUpdate = true;
        } else {
          performOffAction();
        }

        // Reset counter
        duration = 0;

      } else if (reading == LOW) {

        // Start counting how long the button has been pressed
        duration = millis();

        performOnAction();
      }

      // Update state
      hasNewValue = true;
      state = reading;
    } else {
      hasNewValue = false;
    };

#if DEBUG
    print();
#endif
  };

  void setButtonOnAction(ButtonAction buttonAction) { onAction = buttonAction; }
  void setButtonOffAction(ButtonAction buttonAction) {
    offAction = buttonAction;
  }

  void setButtongLongAction(ButtonAction buttonAction) {
    longAction = buttonAction;
  }

  void print() {
    Serial.print("Button num ");
    Serial.print(buttonNumber);
    Serial.print(" on pin ");
    Serial.print(pin);
    Serial.print(": ");
    Serial.println(state);
    Serial.println("Duration: ");
    Serial.println(timeElapsed);
  }

private:
  int state;

  void performLongPressAction() {
	  performAction(longAction);
  };

  void performAction(ButtonAction act) {
    switch (act) {
    case ButtonAction::Nothing:
      break;
    case ButtonAction::CycleMidiMode:
      settings.cycleMidiMode();
      break;
    case ButtonAction::SendNoteOn:
      usbMIDI.sendNoteOn(pageToMidiNum(buttonNumber, settings.currentPage,
                                       settings.numButtons),
                         127, settings.currentLayer);
      break;
    case ButtonAction::SendNoteOff:
      usbMIDI.sendNoteOff(pageToMidiNum(buttonNumber, settings.currentPage,
                                        settings.numButtons),
                          127, settings.currentLayer);
      break;
      // @todo
      /* case ButtonAction::SendMidiCCHigh: */
      /*   midisender.sendCC(settings.maxValue, settings.currentLayer, */
      /*                     settings.currentPage, buttonNumber, */
      /*                     settings.numActionButtons); */
      /*   usbMIDI.sendControlChange(pageToMidiNum(buttonNumber, */
      /*                                           settings.currentPage, */
      /*                                           settings.numButtons), */
      /*                             settings.maxValue, settings.currentLayer);
       */
      /* break; */

    case ButtonAction::SmallIncrements:
      settings.setIncrement(IncrementType::Small);
      break;
    case ButtonAction::LargeIncrements:
      settings.setIncrement(IncrementType::Large);
      break;
    case ButtonAction::HugeIncrements:
      settings.setIncrement(IncrementType::Huge);
      break;
    case ButtonAction::PrintInfo:
      print();
      break;

    case ButtonAction::CycleNextViewStyle:
      cycleNextView();
      break;

    case ButtonAction::CycleNextButtonSetup:
      cycleNextButtonSetup();
      break;

    case ButtonAction::ShowPageOverview:
      settings.screenview = ScreenView::PageOverview;
      settings.screenNeedsUpdate = true;
      break;

      // Keyboard actions
    case ButtonAction::KeyLeft:
      pressKeyboard(KEY_LEFT);
      break;
    case ButtonAction::KeyRight:
      pressKeyboard(KEY_RIGHT);
      break;
    case ButtonAction::KeyUp:
      pressKeyboard(KEY_UP);
      break;
    case ButtonAction::KeyDown:
      pressKeyboard(KEY_DOWN);
      break;
    case ButtonAction::KeyF5:
      pressKeyboard(KEY_F5);
      break;
    case ButtonAction::KeyF12:
      pressKeyboard(KEY_F12);
      break;
    case ButtonAction::KeyCtrlE:
      pressKeyboard(MODIFIERKEY_CTRL, KEY_E);
      break;
    case ButtonAction::KeySpace:
      pressKeyboard(KEY_SPACE);
      break;
    case ButtonAction::KeyPeriod:
      pressKeyboard(KEY_PERIOD);
      break;
    case ButtonAction::KeyEnter:
      pressKeyboard(KEY_ENTER);
      break;
    case ButtonAction::KeyEsc:
      pressKeyboard(KEY_ESC);
      break;
    case ButtonAction::KeyH:
      pressKeyboard(KEY_H);
      break;
    case ButtonAction::KeyJ:
      pressKeyboard(KEY_J);
      break;
    case ButtonAction::KeyK:
      pressKeyboard(KEY_K);
      break;
    case ButtonAction::KeyL:
      pressKeyboard(KEY_L);
      break;
    }
  }

  void performOnAction() { performAction(onAction); }

  void performOffAction() { performAction(offAction); }

private:
  void pressKeyboard(int key) {
    // Keyboard.press(key);
    // Keyboard.release(key);
  }

  void pressKeyboard(int key1, int key2) {
    // Keyboard.press(key1);
    // Keyboard.press(key2);
    // Keyboard.release(key1);
    // Keyboard.release(key2);
  }

  void cycleNextView() {
    auto currentStyleNum = static_cast<int>(settings.valstyle);
    auto newStyle = (currentStyleNum + 1) % numValueStyles;
    settings.screenNeedsUpdate = true;
    settings.valstyle = static_cast<ValueStyle>(newStyle);
  };

  void cycleNextButtonSetup() {
    auto numButtonStyles = 2;
    auto currentStyleNum = static_cast<int>(settings.buttonSetup);
    auto newStyle = (currentStyleNum + 1) % numButtonStyles;
    settings.screenNeedsUpdate = true;
    settings.buttonSetup = static_cast<ButtonSetup>(newStyle);
  }
};

} // namespace hakke

#endif
