#ifndef HAKKEBRAETMAIN
#define HAKKEBRAETMAIN
#pragma once

#include "Arduino.h"
#include "hakkeosc.hpp"
#include "hakkebutton.hpp"
#include "hakkedata.hpp"
#include "hakkeenc.hpp"
#include "hakkemenuenc.hpp"
#include "hakkeoled.hpp"
#include "hakkesettings.hpp"
#include <array>

namespace hakke {

/**
 * @brief Main hakkebraet interface
 * @author Mads Kjeldgaard
 * @date 04-05-2021
 */

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

constexpr int debounce = 10;               // ms
constexpr unsigned long sleeptime = 10000; // ms

template <int numLayers, int numPages, int numEncoders, int numMenuEncoders,
          int numButtons>
class Hakkebraet {
public:
  HakkeState<numLayers, numPages, numEncoders> settings;
  HakkeOled<numLayers, numPages, numEncoders> screen;

  HakkeOSC<numLayers, numPages, numEncoders> osc;

  typedef HakkeEncoder<numLayers, numPages, numEncoders> HEnc;
  typedef HakkeMenuEncoder<numLayers, numPages, numEncoders> HMenuEnc;
  std::array<HEnc, numEncoders> encoders;

  ButtonSetup buttonSetup;

  enum SwitchNames {
    SW1,
    SW2,
    SW3,
    SW4,
    SW5,
    SW6,
    SW7,
    SW8,
  };
  std::array<HakkeButton<numLayers, numPages, numEncoders>, numButtons> buttons;
  std::array<HMenuEnc, numMenuEncoders> menuEncoders;

  Hakkebraet()
      :
	screen(settings),
	osc(settings),
	encoders{
      HEnc(0, 1, 0, settings, osc),
      HEnc(2, 3, 1, settings, osc),
      HEnc(4, 5, 2, settings, osc),
      HEnc(6, 7, 3, settings, osc),
      HEnc(8, 9, 4, settings, osc),
      HEnc(10, 11, 5, settings, osc),
      HEnc(12, 24, 6, settings, osc),
      HEnc(25, 26, 7, settings, osc),

      HEnc(23, 22, 8, settings, osc),
      HEnc(21, 20, 9, settings, osc),
      HEnc(17, 16, 10, settings, osc),
      HEnc(15, 14, 11, settings, osc),
      HEnc(41, 40, 12, settings, osc),
      HEnc(39, 38, 13, settings, osc),
      HEnc(37, 36, 14, settings, osc),
      HEnc(35, 34, 15, settings, osc),
    },
	// FIXME: needs to be adjusted to new teensy 4.1 layoout
        // buttons{
            // HakkeButton<numLayers, numPages, numEncoders>(
            //     27, debounce, SW1, ButtonAction::SendNoteOn,
            //     ButtonAction::SendNoteOff, ButtonAction::Nothing, settings),
            // HakkeButton<numLayers, numPages, numEncoders>(
            //     24, debounce, SW2, ButtonAction::SendNoteOn,
            //     ButtonAction::SendNoteOff, ButtonAction::Nothing, settings),
            // HakkeButton<numLayers, numPages, numEncoders>(
            //     25, debounce, SW3, ButtonAction::SendNoteOn,
            //     ButtonAction::SendNoteOff, ButtonAction::Nothing, settings),
            // HakkeButton<numLayers, numPages, numEncoders>(
            //     26, debounce, SW4, ButtonAction::SendNoteOn,
            //     ButtonAction::SendNoteOff, ButtonAction::Nothing, settings),
            // HakkeButton<numLayers, numPages, numEncoders>(
            //     12, debounce, SW5, ButtonAction::SendNoteOn,
            //     ButtonAction::SendNoteOff, ButtonAction::Nothing, settings),
            // HakkeButton<numLayers, numPages, numEncoders>(
            //     30, debounce, SW6, ButtonAction::SendNoteOn,
            //     ButtonAction::SendNoteOff, ButtonAction::Nothing, settings),
            // HakkeButton<numLayers, numPages, numEncoders>(
            //     17, debounce, SW7, ButtonAction::SendNoteOn,
            //     ButtonAction::SendNoteOff, ButtonAction::Nothing, settings),
            // HakkeButton<numLayers, numPages, numEncoders>(
            //     20, debounce, SW8, ButtonAction::SendNoteOn,
            //     ButtonAction::SendNoteOff, ButtonAction::Nothing, settings),
        // },
        menuEncoders{
            HMenuEnc(31, 32, 33, 8, MenuAction::PrevLayer,
                     MenuAction::NextLayer, ButtonAction::Nothing,
                     ButtonAction::CycleNextViewStyle,
                     ButtonAction::CycleMidiMode, settings),
                    } {};

  void setup() {
	Serial.println("Starting hakkebræt");

    // Setup state object
    settings.setup(numButtons, IncrementType::Large, MIDIMODE::MODE_NORMAL);
	osc.setup();
    settings.buttonSetup = ButtonSetup::Midi;

	// osc();

    // Setup buttons
    // for (size_t i = 0; i < buttons.size(); i++) {
    //   buttons[i].setup();
    // }

    // if (settings.buttonSetup == ButtonSetup::Midi) {
    //   buttonSetupMidi();
    // } else if (settings.buttonSetup == ButtonSetup::KeyBoard) {
    //   buttonSetupKeyboard();
    // }

    buttonSetup = settings.buttonSetup;

    for (size_t i = 0; i < menuEncoders.size(); i++) {
      menuEncoders[i].setup();
    }

    // Setup screen
    screen.setup();
    screen.greet();
    screen.update();
  }

  void loop() {
	osc.loop();
    readEncoders();
    // readButtons();
    // readButtonSetup();
    pollForNewScreenState();
    screen.update();
    settings.screenNeedsUpdate = false;
  }

private:
  void readButtonSetup() {
    if (settings.buttonSetup != buttonSetup) {
      if (settings.buttonSetup == ButtonSetup::Midi) {
        buttonSetupMidi();
      } else if (settings.buttonSetup == ButtonSetup::KeyBoard) {
        buttonSetupKeyboard();
      }
      buttonSetup = settings.buttonSetup;
    }
  }

  void buttonSetupMidi() {
    buttons[SW1].setButtonOnAction(ButtonAction::SendNoteOn);
    buttons[SW1].setButtonOffAction(ButtonAction::SendNoteOff);

    buttons[SW2].setButtonOnAction(ButtonAction::SendNoteOn);
    buttons[SW2].setButtonOffAction(ButtonAction::SendNoteOff);

    buttons[SW3].setButtonOnAction(ButtonAction::SendNoteOn);
    buttons[SW3].setButtonOffAction(ButtonAction::SendNoteOff);

    buttons[SW4].setButtonOnAction(ButtonAction::SendNoteOn);
    buttons[SW4].setButtonOffAction(ButtonAction::SendNoteOff);

    buttons[SW5].setButtonOnAction(ButtonAction::SendNoteOn);
    buttons[SW5].setButtonOffAction(ButtonAction::SendNoteOff);

    buttons[SW6].setButtonOnAction(ButtonAction::SendNoteOn);
    buttons[SW6].setButtonOffAction(ButtonAction::SendNoteOff);

    buttons[SW7].setButtonOnAction(ButtonAction::SendNoteOn);
    buttons[SW7].setButtonOffAction(ButtonAction::SendNoteOff);

    buttons[SW8].setButtonOnAction(ButtonAction::SendNoteOn);
    buttons[SW8].setButtonOffAction(ButtonAction::SendNoteOff);
  }

  void buttonSetupKeyboard() {
    buttons[SW1].setButtonOnAction(ButtonAction::KeyH);
    buttons[SW1].setButtonOffAction(ButtonAction::Nothing);

    buttons[SW2].setButtonOnAction(ButtonAction::KeyJ);
    buttons[SW2].setButtonOffAction(ButtonAction::Nothing);

    buttons[SW3].setButtonOnAction(ButtonAction::KeyK);
    buttons[SW3].setButtonOffAction(ButtonAction::Nothing);

    buttons[SW4].setButtonOnAction(ButtonAction::KeyL);
    buttons[SW4].setButtonOffAction(ButtonAction::Nothing);

    buttons[SW5].setButtonOnAction(ButtonAction::KeyLeft);
    buttons[SW5].setButtonOffAction(ButtonAction::Nothing);

    buttons[SW6].setButtonOnAction(ButtonAction::KeyUp);
    buttons[SW6].setButtonOffAction(ButtonAction::Nothing);

    buttons[SW7].setButtonOnAction(ButtonAction::KeyDown);
    buttons[SW7].setButtonOffAction(ButtonAction::Nothing);

    buttons[SW8].setButtonOnAction(ButtonAction::KeyRight);
    buttons[SW8].setButtonOffAction(ButtonAction::Nothing);
  }

  void pollForNewScreenState() {

    // New value?
    if (settings.newButtonValue) {
      settings.screenview = ScreenView::ButtonValue;
      settings.lastUpdate = millis();
      settings.screenNeedsUpdate = true;
    } else if (settings.newEncoderValue) {
      settings.screenview = ScreenView::EncoderValue;
      settings.lastUpdate = millis();
      settings.screenNeedsUpdate = true;
    } else if (settings.newMenuEncoderVal) {
      settings.screenview = ScreenView::EncoderValue;
      settings.lastUpdate = millis();
      settings.screenNeedsUpdate = true;
    }

    /* } else if (settings.newMenuEncoderVal) { */
    /*   settings.lastUpdate = millis(); */
    /*   settings.screenview = ScreenView::PageOverview; */
    /*   settings.screenNeedsUpdate = true; */

    else if ((millis() - settings.lastUpdate) > settings.sleepTime) {
      settings.screenview = ScreenView::Sleep;
      settings.screenNeedsUpdate = true;
    }

    ;
  }

  void readButtons() {

    settings.newButtonValue = false;
    for (size_t i = 0; i < buttons.size(); i++) {
      buttons[i].read();
      if (buttons[i].hasNewValue) {
        buttons[i].print();
        settings.newButtonValue = true;
      }
    }
  };

  void readEncoders() {
    settings.newMenuEncoderVal = false;

    for (size_t encNum = 0; encNum < menuEncoders.size(); encNum++) {
      menuEncoders[encNum].read();
      if (menuEncoders[encNum].hasNewValue) {
        settings.newMenuEncoderVal = true;
      }
    }

    settings.newEncoderValue = false;
    for (size_t encNum = 0; encNum < encoders.size(); encNum++) {
      encoders[encNum].readAndSend();
      if (encoders[encNum].hasNewValue) {
        settings.newEncoderValue = true;
#if DEBUG
        encoders[encNum].print();
#endif
      }
    }
  };
};

} // namespace hakke

#endif
