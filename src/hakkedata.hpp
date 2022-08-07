#ifndef HAKKEDATA
#define HAKKEDATA
#pragma once

#include <array>
#include <Arduino.h>

namespace hakke {

/**
 * @brief The data structure for Hakkebræt
 * @author Mads Kjeldgaard
 * @date 22-04-2021
 * @decription This is the data structure for the Hakkebræt midi controller. It 
 * organizes the data in layers. Each layer has a number of pages, each page has
 *  number of encoders.
 */
template <int numLayers, int numPages, int numValuesPerPage> class HakkeData {

private:
  /*
   * Each layer contains a number of pages. Each page has a number of encoders
   * corresponding to the number of hardware encoders.
   */
  std::array<std::array<std::array<int, numValuesPerPage>, numPages>, numLayers>
      midiLayers;

public:
  int lastTouchedEncoderNum{0};

  HakkeData() {
    reset();
  }

  void reset(int value = 0) {
    for (auto layer : midiLayers) {
      for (auto page : layer) {
        page.fill(value);
      }
    }
  }


  // Sets the value for encoder num in the layer and page put in. 
  void setValue(int layer, int page, int encoderNum, int value) {
    // Set value in layer
    midiLayers[layer][page][encoderNum] = value;
  };

  std::array<int, numValuesPerPage>& getPage(int layer, int page){
	  return midiLayers[layer][page];
  }

  int getValue(int layer, int page, int encoderNum) {
    return midiLayers[layer][page][encoderNum];
  }

  void printValues() {
    for (int layNum = 0; layNum < midiLayers.size(); layNum++) {

      Serial.print("Layer:");
      Serial.println(layNum);

      for (int pageNum = 0; pageNum < midiLayers[layNum].size(); pageNum++) {
        Serial.print("\tPage: ");
        Serial.println(pageNum);

        for (int encNum = 0; encNum < midiLayers[layNum][pageNum].size();
             encNum++) {
          const auto val = getValue(layNum, pageNum, encNum);
          Serial.print("\t\tEnc: ");
          Serial.print(encNum);
          Serial.print(", ");
          Serial.println(val);
        }
      }
    }
  }
};

} // namespace hakke

#endif
