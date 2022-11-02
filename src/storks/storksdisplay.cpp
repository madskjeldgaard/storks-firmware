#include "storksdisplay.hpp"

namespace storkspace {
void StorksDisplay::setup() {

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

void StorksDisplay::loop() {

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
};

void StorksDisplay::greet() {

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

void StorksDisplay::encoderInfo() {

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
};

void StorksDisplay::setLastTouchedEncoder(int hardwareEncoderNum, int layerNum,
                                          int midiChannel, float value,
                                          int midi14Val) {
  lastTouchedEncoder.value = value;
  lastTouchedEncoder.encoderNum = hardwareEncoderNum;
  lastTouchedEncoder.layerNum = layerNum;
  lastTouchedEncoder.midi14 = midi14Val;
  lastTouchedEncoder.midiChannel = midiChannel;
};

} // namespace storkspace
