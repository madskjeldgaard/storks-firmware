#pragma once
#include "encodervelocity.hpp"
#ifndef STORKSENC
#define STORKSENC
#include "Encoder.h"
#include "storksmath.hpp"
#include "onepole.hpp"
#include <cmath>

namespace storkspace {

// A simple way to pass encoder values around
struct EncoderValues {
  int velocity7;
  int velocity14;
  float velocityFloat;
  int sign;
  float angularSpeed;

  friend bool operator==(const EncoderValues &vals1,
                         const EncoderValues &vals2) {
    return vals1.velocity7 == vals2.velocity7 &&
           vals1.velocity14 == vals2.velocity14 &&
           vals1.velocityFloat == vals2.velocityFloat &&
           vals1.sign == vals2.sign && vals1.angularSpeed == vals2.angularSpeed;
  };

  friend bool operator!=(const EncoderValues &vals1,
                         const EncoderValues &vals2) {
    return vals1.velocity7 != vals2.velocity7 &&
           vals1.velocity14 != vals2.velocity14 &&
           vals1.velocityFloat != vals2.velocityFloat &&
           vals1.sign != vals2.sign && vals1.angularSpeed != vals2.angularSpeed;
  };
};

/**
 * @brief An object that wraps around a hardware encoder
 * @author Mads Kjeldgaard
 * @date 2022-10-02
 * @details This object serves as an interface for hardware encoders and handle
 * updating, reading and calculating values
 */
class HardwareEncoder {
public:
  HardwareEncoder(const int encoderPinA, const int encoderPinB)
      : hardwareEncoder(encoderPinA, encoderPinB){

        };

  const EncoderValues &getValues() const;

  // Setup the encoder object
  void setup();

  // Update the encoder object
  void loop();

  // ~EncoderController();

  const int getRaw() const { return rawValue; };

private:

  OnePoleSmoother smoother;
  EncoderValues values;
  Encoder hardwareEncoder;
  EncoderVelocity encVelocity;
  int rawValue{0}, newValue{0};
};

}; // namespace storkspace
#endif
