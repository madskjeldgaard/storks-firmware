#ifndef HAKKEENC
#define HAKKEENC
#include "hakkeosc.hpp"
#pragma once
#include "Encoder.h"
#include "hakkedata.hpp"
#include "hakkeglobals.hpp"
#include "hakkeoled.hpp"
#include "hakkesettings.hpp"
#include "midisender.hpp"
#define ENCODER_OPTIMIZE_INTERRUPTS

namespace hakke {

/**
 * @brief Encoder for hakkebræt
 * @author Mads Kjeldgaard
 * @date 22-04-2021
 */
template <int numLayers, int numPages, int numEncs> class HakkeEncoder {
public:
  int pinA, pinB;

  int value;
  long encoderState;

  bool hasNewValue{false};

  // Midi layer / page logic
  int encoderNum;

  HakkeState<numLayers, numPages, numEncs> &settings;
  Encoder encoder;

  // Midi stuffs
  HakkeMidiSender<numLayers, numPages, numEncs> sender;
  HakkeOSC<numLayers, numPages, numEncs> &oscSender;

  HakkeEncoder(int setupPinB, int setupPinA, int initEncNum,
               HakkeState<numLayers, numPages, numEncs> &globalSettings,
               HakkeOSC<numLayers, numPages, numEncs> &osc)
      : settings(globalSettings), encoder(setupPinA, setupPinB),
        sender(globalSettings), oscSender(osc) {
    init(setupPinA, setupPinB, initEncNum);
  }

  void init(int setupPinA, int setupPinB, int initEncNum) {
    pinA = setupPinA;
    pinB = setupPinB;

    value = 0;
    encoderState = encoder.read();

    encoderNum = initEncNum;
  };

  // Get data from data structure and put in "value"
  int getVal() {
    return settings.data.getValue(settings.currentLayer, settings.currentPage,
                                  encoderNum);
  }

  void fetchValueInData() { value = getVal(); }

  void updateData(int value) {
    settings.data.setValue(settings.currentLayer, settings.currentPage,
                           encoderNum, value);
  }

  // Read encoder value, send midi cc and update oled
  void readAndSend() {
    auto reading = encoder.read();
    fetchValueInData();

    // Is value new?
    if (reading != encoderState) {
      hasNewValue = true;

      // Check direction
      if (reading > encoderState) {
        value += settings.incrementSize;
      } else {
        value -= settings.incrementSize;
      }

      // @TODO: Wrap or clip value
      const auto maxmidival = settings.maxValue;
      if (settings.wrapMode) {
        // Wrap data
        value = wrapValue(value, 0, maxmidival);
      } else {
        // Clip data
        value = clipValue(value, 0, maxmidival);
      }

      updateData(value);
      fetchValueInData();
      settings.data.lastTouchedEncoderNum = encoderNum;

      // @FIXME This allows  to send negative values, clipping needs to be done
      // correctly before sending
      // Send off midi cc and update data structure
      sender.sendCC(value, settings.currentLayer, settings.currentPage,
                    encoderNum, settings.numEncoders);

      // Send OSC
      // constexpr auto maxAddrSize = 1024;
      // char address[maxAddrSize];
      //    sprintf(address, "/layer/%i/page/%i/encoder/%i",
      //    settings.currentLayer, settings.currentPage, encoderNum);
      // oscSender.sendFloatMsg(address, static_cast<float>(value /
      // maxmidival));

      // Update encoder state
      encoderState = reading;

    } else {
      hasNewValue = false;
    };
  }

  void print() {
    Serial.print("Encoder on pins A:");
    Serial.print(pinA);
    Serial.print(", B: ");
    Serial.println(pinB);
    Serial.println(value);
  }

  /* int smallIncrement = 1; */
  /* int largeIncrement = 32; */
};
} // namespace hakke

#endif
