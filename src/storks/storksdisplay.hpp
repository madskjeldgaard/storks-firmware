#pragma once
#ifndef STORKSDISPLAY
#define STORKSDISPLAY
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
/*
 *
 * TODO:
 * - Move to .cpp file
 *
 */

namespace storkspace {

// A simple type to pass around encoder values inside the display class
struct EncoderTouch {

  float value;

  int encoderNum, layerNum, midiChannel, midi14;
};

enum class ValueStyle { RawMidiValue, FloatingPoint, Bar };
constexpr int numValueStyles = 3;

enum class ScreenView {
  EncoderValue,
  PageOverview,
  MidiModeDisplay,
  Intro,
  Sleep,
  ButtonValue
};

/**
 * @brief StorksDisplay
 * @author Mads Kjeldgaard
 * @date 10-10-2022
 * @details Interface for the OLED screen of Storks
 */
class StorksDisplay {
public:
  // ~StorksDisplay();

  static constexpr auto SCREEN_WIDTH = 128; // OLED display width, in pixels
  static constexpr auto SCREEN_HEIGHT = 64; // OLED display height, in pixels

  static constexpr auto OLED_RESET =
      -1; // Reset pin # (or -1 if sharing Arduino reset pin)
  static constexpr auto SCREEN_ADDRESS =
      0x3C; ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32

  Adafruit_SSD1306 ssd1306display;

  // screenUpdateInterval is milliseconds between screen updates
  StorksDisplay(unsigned int screenUpdateInterval = 10)
      : ssd1306display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET),
        updateInterval(screenUpdateInterval){};
  void setup();

  void loop();

  void sleep(bool dosleep) { ssd1306display.dim(dosleep); }

  // @TODO
  void greet();

  // void displayMidiMode(){
  //  auto mode = (settings.mode == MIDIMODE::MODE_14BIT)? "14 bit" : "7 bit";
  //  display.clearDisplay();
  //  display.setTextSize(4);
  //  display.setTextColor(SSD1306_WHITE); // Draw white text
  //  display.cp437(true); // Use full 256 char 'Code Page 437' font
  //  display.println(mode);
  //  display.display();
  // }

  // @TODO
  // void pageOverview() {
  //   auto layer = settings.currentLayer;
  //   auto page = settings.currentPage;
  //
  //   display.clearDisplay();
  //   display.setTextSize(1);
  //   display.setTextColor(SSD1306_WHITE); // Draw white text
  //   display.cp437(true); // Use full 256 char 'Code Page 437' font
  //
  //   for (size_t i = 0; i < settings.data.getPage(layer, page).size(); i++) {
  //     auto val = settings.data.getPage(layer, page)[i];
  //     auto offset = i % (settings.numEncoders / 2);
  //     auto digits = 2;
  //     auto heightOffset = (i % (settings.numEncoders / 2) == 0) ? 1 : 0;
  //     display.setCursor(offset * (SCREEN_WIDTH / 4),
  //                       heightOffset * (SCREEN_HEIGHT / 2));
  //
  //     auto floatval =
  //         static_cast<float>(val) / static_cast<float>(settings.maxValue);
  //
  //     if (i % (settings.numEncoders / 2) == 0) {
  //       display.println(floatval, digits);
  //     } else {
  //       display.print(floatval, digits);
  //     }
  //   }
  //
  //   display.display();
  // }

  // void pageAndLayer() {
  //   auto layer = settings.currentLayer;
  //   auto page = settings.currentPage;
  //
  //   display.clearDisplay();
  //   display.setTextSize(2);
  //   display.setTextColor(SSD1306_WHITE); // Draw white text
  //   display.cp437(true); // Use full 256 char 'Code Page 437' font
  //
  //   // Print layer number
  //   display.setCursor(0, 0);
  //   display.print("Layer: ");
  //   display.println(layer);
  //
  //   // Print page
  //   display.setCursor(0, SCREEN_HEIGHT / 2);
  //   display.print("Page:  ");
  //   display.println(page);
  //
  //   display.display();
  // }
  //
  void encoderInfo();

  // Update the screen's data for last touched encoder
  void setLastTouchedEncoder(int hardwareEncoderNum, int layerNum,
                             int midiChannel, float value, int midi14Val);

  EncoderTouch lastTouchedEncoder;
  ValueStyle valstyle{ValueStyle::FloatingPoint};
  unsigned int updateInterval;

  elapsedMillis timer;
};

} // namespace storkspace
#endif
