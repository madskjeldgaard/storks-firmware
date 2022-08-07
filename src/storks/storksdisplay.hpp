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
  void setup() {

    Wire.setSCL(19);
    Wire.setSDA(18);

    // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
    if (!ssd1306display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {

#ifdef DEBUG
      Serial.println(F("SSD1306 allocation failed"));
      for (;;)
        ; // Don't proceed, loop forever
#endif
    }

    ssd1306display.clearDisplay();
    greet();
  };

  void loop() {

    if (timer > updateInterval) {
      sleep(false);
      ssd1306display.clearDisplay();
      encoderInfo();
      timer = elapsedMillis();
    }

    // greet();

    //   if (settings.screenNeedsUpdate) {
    //     // Update view
    //     switch (settings.screenview) {
    //     case ScreenView::Intro:
    //       sleep(false);
    //       greet();
    //       break;
    //     case ScreenView::EncoderValue:
    //       sleep(false);
    // encoderInfoLastTouched();
    //       break;
    //
    // // @TODO
    //     case ScreenView::ButtonValue:
    //       sleep(false);
    //       encoderInfoLastTouched();
    //       break;
    //
    // // @TODO:
    //     case ScreenView::PageOverview:
    //       sleep(false);
    //       pageOverview();
    //       break;
    //  case ScreenView::MidiModeDisplay:
    // displayMidiMode();
    // break;
    //     case ScreenView::Sleep:
    //       sleep(true);
    //       break;
    //     }
    //   };
  }

  void sleep(bool dosleep) { ssd1306display.dim(dosleep); }

  // @TODO
  void greet() {

    for (int i = 0; i < 3; i++) {
      auto scale = i + 1;
      ssd1306display.setTextSize(max(2, 4 - scale));

      if (i % 2 == 0) {
        ssd1306display.setTextColor(SSD1306_WHITE); // Draw white text
      } else {
        ssd1306display.setTextColor(SSD1306_BLACK,
                                    SSD1306_WHITE); // Draw 'inverse' text
      }
      ssd1306display.setCursor(0, 0); // Start at top-left corner
      ssd1306display.cp437(true);     // Use full 256 char 'Code Page 437' font

      ssd1306display.clearDisplay();
      ssd1306display.println(F("<,^,>"));
      ssd1306display.display();

      delay(33 * scale);
      ssd1306display.setCursor(ssd1306display.width() / scale,
                               ssd1306display.height() / scale);
      ssd1306display.clearDisplay();
      ssd1306display.println(F("<,^,>"));
      ssd1306display.display();

      delay(24 * scale * 2);

      ssd1306display.clearDisplay();
      ssd1306display.setCursor(0, 0);
      ssd1306display.println(F("<,^,>"));
      ssd1306display.display();

      delay(27 / scale);
    }

    ssd1306display.clearDisplay();

    /* display.setTextSize(3); */
    /* display.setCursor(0, 0); */
    /* display.println(F("-------")); */
    ssd1306display.setTextSize(2);
    ssd1306display.setCursor(0, ssd1306display.height() - 40);
    ssd1306display.println(F("Storks"));
    /* display.setTextSize(3); */
    /* display.setCursor(0, display.height() - 20); */
    /* display.println(F("________")); */
    ssd1306display.display();
  };

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
  void encoderInfo() {

    ssd1306display.clearDisplay();
    ssd1306display.setTextColor(SSD1306_WHITE); // Draw white text
    ssd1306display.cp437(true); // Use full 256 char 'Code Page 437' font
    ssd1306display.setCursor(0, 0);

    // Display midi mode

    ssd1306display.setTextSize(2);

    // Midi info
    ssd1306display.printf("CC%i CH:%i\nLY:%i/4", lastTouchedEncoder.encoderNum,
                          lastTouchedEncoder.midiChannel,
                          lastTouchedEncoder.layerNum + 1);

    /* @TODO: */
    /* if(settings.buttonSetup == ButtonSetup::Midi){ */
    /* 	ssd1306display.println(" note"); */
    /* } else if(settings.buttonSetup == ButtonSetup::KeyBoard){ */
    /* 	ssd1306display.println(" keys"); */
    /* }; */

    // Now print the value
    auto valueOffsetHeight = SCREEN_HEIGHT / 2;

    switch (valstyle) {
    case ValueStyle::RawMidiValue:
      // Print encoder value
      ssd1306display.setCursor(0, valueOffsetHeight);
      ssd1306display.setTextSize(4);
      ssd1306display.print(lastTouchedEncoder.midi14);
      break;
    case ValueStyle::FloatingPoint:
      // Print encoder value
      ssd1306display.setCursor(0, valueOffsetHeight + 10);
      ssd1306display.setTextSize(3);
      ssd1306display.print(
          lastTouchedEncoder.value,
          // static_cast<float>(val) / static_cast<float>(settings.maxValue),
          4);
      break;
    case ValueStyle::Bar:
      const auto maxLength = ssd1306display.width() / 10 * 8;
      const auto xOffset = 0;
      const auto barLength =
          lastTouchedEncoder.value * static_cast<float>(maxLength);
      const auto barheight = valueOffsetHeight - 20;

      // Boundaries
      ssd1306display.drawRect(xOffset, valueOffsetHeight + 10, maxLength,
                              barheight, SSD1306_WHITE);

      // Progress
      ssd1306display.fillRect(xOffset, valueOffsetHeight + 10, barLength,
                              barheight, SSD1306_WHITE);
      ssd1306display.display();

      break;
    }

    // Update ssd1306display
    ssd1306display.display();
  }

  // Update the screen's data for last touched encoder
  void setLastTouchedEncoder(int hardwareEncoderNum, int layerNum,
                             int midiChannel, float value, int midi14Val) {
    lastTouchedEncoder.value = value;
    lastTouchedEncoder.encoderNum = hardwareEncoderNum;
    lastTouchedEncoder.layerNum = layerNum;
    lastTouchedEncoder.midi14 = midi14Val;
    lastTouchedEncoder.midiChannel = midiChannel;
  };

  EncoderTouch lastTouchedEncoder;
  ValueStyle valstyle{ValueStyle::FloatingPoint};
  unsigned int updateInterval;

  elapsedMillis timer;
};

} // namespace storkspace
#endif
