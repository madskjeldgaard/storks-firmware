#ifndef HAKKEOLED
#define HAKKEOLED
#pragma once

#include "Arduino.h"
#include "hakkedata.hpp"
#include "hakkeglobals.hpp"
#include "hakkesettings.hpp"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <SPI.h>
#include <Wire.h>

namespace hakke {

constexpr auto SCREEN_WIDTH = 128; // OLED display width, in pixels
constexpr auto SCREEN_HEIGHT = 64; // OLED display height, in pixels

constexpr auto OLED_RESET =
    -1; // Reset pin # (or -1 if sharing Arduino reset pin)
constexpr auto SCREEN_ADDRESS =
    0x3C; ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32

extern Adafruit_SSD1306 display;

enum class ValueStyle { RawMidiValue, FloatingPoint, Bar };
constexpr int numValueStyles = 3;

enum class ScreenView { EncoderValue, PageOverview, MidiModeDisplay, Intro, Sleep, ButtonValue };

// @FIXME: Is this proper?
// Forward declare HakkeState
template <int numLayers, int numPages, int numEncs> class HakkeState;

/**
 * @brief Screen controller for Hakkebræt
 * @author Mads Kjeldgaard
 * @date 22-04-2021
 */
template <int numLayers, int numPages, int numEncs> class HakkeOled {
public:
  HakkeState<numLayers, numPages, numEncs> &settings;

  HakkeOled(HakkeState<numLayers, numPages, numEncs> &globalSettings)
      : settings(globalSettings){};

  void setup() {

    Wire.setSCL(19);
    Wire.setSDA(18);

    // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
    if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {

#ifdef DEBUG
      Serial.println(F("SSD1306 allocation failed"));
      for (;;)
        ; // Don't proceed, loop forever
#endif
    }
  };

  void update() {

    if (settings.screenNeedsUpdate) {
      // Update view
      switch (settings.screenview) {
      case ScreenView::Intro:
        sleep(false);
        greet();
        break;
      case ScreenView::EncoderValue:
        sleep(false);
        encoderInfoLastTouched();
        break;

		// @TODO
      case ScreenView::ButtonValue:
        sleep(false);
        encoderInfoLastTouched();
        break;

		// @TODO:
      case ScreenView::PageOverview:
        sleep(false);
        pageOverview();
        break;
	  case ScreenView::MidiModeDisplay:
		displayMidiMode();
		break;
      case ScreenView::Sleep:
        sleep(true);
        break;
      }
    };
  }

  void sleep(bool dosleep) { display.dim(dosleep); }

  // @TODO
  void greet() {

    for (int i = 0; i < 3; i++) {
      auto scale = i + 1;
      display.setTextSize(std::max(2, 4 - scale));

      if (i % 2 == 0) {
        display.setTextColor(SSD1306_WHITE); // Draw white text
      } else {
        display.setTextColor(SSD1306_BLACK,
                             SSD1306_WHITE); // Draw 'inverse' text
      }
      display.setCursor(0, 0); // Start at top-left corner
      display.cp437(true);     // Use full 256 char 'Code Page 437' font

      display.clearDisplay();
      display.println(F("ha"));
      display.display();

      delay(33 * scale);
      display.setCursor(display.width() / scale, display.height() / scale);
      display.clearDisplay();
      display.println(F(" hak"));
      display.display();

      delay(24 * scale * 2);

      display.clearDisplay();
      display.setCursor(0, 0);
      display.println(F("HAK"));
      display.display();

      delay(27 / scale);
    }

    display.clearDisplay();

    /* display.setTextSize(3); */
    /* display.setCursor(0, 0); */
    /* display.println(F("-------")); */
    display.setTextSize(2);
    display.setCursor(0, display.height() - 40);
    display.println(F("HAKKEBRAET"));
    /* display.setTextSize(3); */
    /* display.setCursor(0, display.height() - 20); */
    /* display.println(F("________")); */
    display.display();
  };

  void displayMidiMode(){
	  auto mode = (settings.mode == MIDIMODE::MODE_14BIT)? "14 bit" : "7 bit";
	  display.clearDisplay();
	  display.setTextSize(4);
	  display.setTextColor(SSD1306_WHITE); // Draw white text
	  display.cp437(true); // Use full 256 char 'Code Page 437' font
	  display.println(mode);
	  display.display();
  }

  // @TODO
  void pageOverview() {
    auto layer = settings.currentLayer;
    auto page = settings.currentPage;

    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE); // Draw white text
    display.cp437(true); // Use full 256 char 'Code Page 437' font

    for (size_t i = 0; i < settings.data.getPage(layer, page).size(); i++) {
      auto val = settings.data.getPage(layer, page)[i];
      auto offset = i % (settings.numEncoders / 2);
      auto digits = 2;
      auto heightOffset = (i % (settings.numEncoders / 2) == 0) ? 1 : 0;
      display.setCursor(offset * (SCREEN_WIDTH / 4),
                        heightOffset * (SCREEN_HEIGHT / 2));

      auto floatval =
          static_cast<float>(val) / static_cast<float>(settings.maxValue);

      if (i % (settings.numEncoders / 2) == 0) {
        display.println(floatval, digits);
      } else {
        display.print(floatval, digits);
      }
    }

    display.display();
  }

  void pageAndLayer() {
    auto layer = settings.currentLayer;
    auto page = settings.currentPage;

    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(SSD1306_WHITE); // Draw white text
    display.cp437(true); // Use full 256 char 'Code Page 437' font

    // Print layer number
    display.setCursor(0, 0);
    display.print("Layer: ");
    display.println(layer);

    // Print page
    display.setCursor(0, SCREEN_HEIGHT / 2);
    display.print("Page:  ");
    display.println(page);

    display.display();
  }

  void encoderInfoLastTouched() {
    encoderInfoCurrentLayer(settings.data.lastTouchedEncoderNum);
  }

  void encoderInfoCurrentLayer(int encoderNum) {
    auto midichan = settings.currentLayer;
    auto midicc = pageToMidiNum(encoderNum, settings.currentPage, numEncs);

    encoderInfo(settings.currentLayer, settings.currentPage, encoderNum,
                midichan, midicc);
  }

  void encoderInfo(int layer, int page, int encoderNum, int midichan,
                   int midicc) {
    auto val = settings.data.getValue(layer, page, encoderNum);

    display.clearDisplay();
    display.setTextColor(SSD1306_WHITE); // Draw white text
    display.cp437(true); // Use full 256 char 'Code Page 437' font
    display.setCursor(0, 0);

	// Display midi mode

	display.setTextSize(2);

    // Midi info
    display.print("CH");
    display.print(midichan);

    display.print(" PG");
    display.println(page + 1); // Offset with 1 even though it's cheating!!!

    display.print("CC");
    display.print(midicc);

	auto mode = (settings.mode == MIDIMODE::MODE_14BIT)? " 14BIT" : " 7BIT";
	display.println(mode);

	/* @TODO: */ 
	/* if(settings.buttonSetup == ButtonSetup::Midi){ */
	/* 	display.println(" note"); */
	/* } else if(settings.buttonSetup == ButtonSetup::KeyBoard){ */
	/* 	display.println(" keys"); */
	/* }; */


	// Now print the value
    auto valueOffsetHeight = SCREEN_HEIGHT / 2;

    switch (settings.valstyle) {
    case ValueStyle::RawMidiValue:
      // Print encoder value
      display.setCursor(0, valueOffsetHeight);
      display.setTextSize(4);
      display.print(val);
      break;
    case ValueStyle::FloatingPoint:
      // Print encoder value
      display.setCursor(0, valueOffsetHeight + 10);
      display.setTextSize(3);
      display.print(
          static_cast<float>(val) / static_cast<float>(settings.maxValue), 4);
      break;
    case ValueStyle::Bar:
      const auto maxLength = display.width() / 10 * 8;
      const auto xOffset = 0;
      const auto barLength =
          (static_cast<float>(val) / static_cast<float>(settings.maxValue)) *
          static_cast<float>(maxLength);
      const auto barheight = valueOffsetHeight - 20;

      // Boundaries
      display.drawRect(xOffset, valueOffsetHeight + 10, maxLength, barheight,
                       SSD1306_WHITE);

      // Progress
      display.fillRect(xOffset, valueOffsetHeight + 10, barLength, barheight,
                       SSD1306_WHITE);
      display.display();

      break;
    }

    // Update display
    display.display();
  }
};

} // namespace hakke
#endif
