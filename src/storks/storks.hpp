#pragma once
#ifndef STORKS
#define STORKS

// #include "menuencoder.hpp"
#include "channel.hpp"
#include "common.hpp"
#include "hardwareencoder.hpp"
#include "mux.hpp"
#include "oschost.hpp"
#include "storksdisplay.hpp"
#include <array>

#include "Encoder.h"
namespace storkspace {

class Storks {

  /**
   * @brief MenuEncoder
   * @author Mads Kjeldgaard
   * @date 08-10-2022
   * @details An object that is an interface for the menu encoder
   */
  class MenuEncoder {
    // TODO: Button
  public:
    MenuEncoder(Storks *storks, const int encoderPinA, const int encoderPinB,
                const int buttonPin)
        : storkSystem(storks), hardwareEncoder(encoderPinA, encoderPinB),
          buttonpin(buttonPin){};
    // ~MenuEncoder();

    // Setup the encoder object
    void setup() { rawValue = hardwareEncoder.read(); };

    // Update the encoder object
    void loop() {
      const auto newValue =
          hardwareEncoder.read() /
          4; // Divide by 4 because each encoder tick causes 4 readings

      if (rawValue != newValue) {
        const auto sign = get_sign(newValue - rawValue);
        incrementOrDecrementChannel(sign);
      };

      rawValue = newValue;
    };

    // Increase or decrease channel num / layer num
    void incrementOrDecrementChannel(const int inc) {
      const auto currentChanNum = storkSystem->getCurrentChannel();
      const auto currentLayerNum = storkSystem->getCurrentLayer();

      // If the potential new layer number wraps, it's time to change channel
      const bool channelNeedsToBeChanged =
          ((currentLayerNum + inc) >= numLayers) ||
          ((currentLayerNum + inc) < 0);

      if (channelNeedsToBeChanged) {
        const bool channelNumHasBeenDecremented =
            (currentChanNum + inc) < currentChanNum;

		// Change the channel
        storkSystem->changeChannel(
            wrapValue(currentChanNum + inc, 0, numChannels - 1), numLayers - 1);

        // Set new layer to miniumum or maximum layer depending on direction of
        // incrementation
        if (channelNumHasBeenDecremented) {
          storkSystem->changeLayer(numLayers - 1);
        } else {
          storkSystem->changeLayer(0);
        }

      } else {
        storkSystem->changeLayer(currentLayerNum + inc);
      };
    }

  private:
    Storks *storkSystem;
    Encoder hardwareEncoder;
    int buttonpin;
    int rawValue;
  };
  // MenuEncoder END

public:
  OSCHost osc;
  StorksDisplay storksdisplay;

private:
  // Hardware encoders
  std::array<HardwareEncoder, numHardwareEncoders> hardwareEncoders = {{
      HardwareEncoder(0, 1),
      HardwareEncoder(2, 3),
      HardwareEncoder(4, 5),
      HardwareEncoder(6, 7),
      HardwareEncoder(8, 9),
      HardwareEncoder(10, 11),
      HardwareEncoder(12, 24),
      HardwareEncoder(25, 26),
      HardwareEncoder(23, 22),
      HardwareEncoder(21, 20),
      HardwareEncoder(17, 16),
      HardwareEncoder(15, 14),
      HardwareEncoder(41, 40),
      HardwareEncoder(39, 38),
      HardwareEncoder(37, 36),
      HardwareEncoder(35, 34),
  }};

  // std::array<HardwareButton, numHardwareButtons> hardwareButtons = {{
  // 	HardwareButton(1),
  // 	HardwareButton(2),
  // 	HardwareButton(3),
  // 	HardwareButton(4),
  // 	HardwareButton(5),
  // 	HardwareButton(6),
  // 	HardwareButton(7),
  // 	HardwareButton(8),
  //   }};

  // Channel objects
  std::array<Channel, numChannels> channels;

public:
  Storks();
  void setup();
  void loop();
  Storks(Storks &&) = default;
  Storks(const Storks &) = default;
  Storks &operator=(Storks &&) = default;
  Storks &operator=(const Storks &) = default;
  ~Storks();

  const int getCurrentChannel() const { return currentChannel; }
  const int getCurrentLayer() const { return currentLayer; }

  void changeLayer(std::size_t newLayer) {
    if (newLayer < numLayers) {
      // Enable first layer in new channel
      channels[currentChannel].enableLayer(newLayer);
      currentLayer = newLayer;
    } else {
      Serial.println(
          "ERROR: Can't set layer higher than maximum number of layers");
    }
  }

  void changeChannel(std::size_t newChannel) {
    if (newChannel < channels.size()) {
      // Disable previous channel
      channels[currentChannel].toggle(false);

      // Enable first layer in new channel
      channels[newChannel].enableLayer(0);
      currentChannel = newChannel;
      currentLayer = 0;
    } else {
      Serial.println(
          "ERROR: Can't set channel higher than maximum number of channels");
    }
  }

private:
  std::size_t currentChannel, currentLayer;
  MenuEncoder menuEncoder;

  Mux buttonMux;

  // Layer testlayer;
};
} // namespace storkspace

#endif
