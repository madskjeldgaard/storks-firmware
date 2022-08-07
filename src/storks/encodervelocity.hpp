#pragma once
#include "hardwareencoder.hpp"
#ifndef STORKSENCVEL
#define STORKSENCVEL
#include "Encoder.h"
#include "storksmath.hpp"

/**
 * @brief An object for calculating encoder velocity stuff
 * @author Mads Kjeldgaard
 * @date 2022-10-02
 * @details
 */
namespace storkspace {
/*
 * The controller uses PEC11R for value encoders
 * Datasheet: https://no.mouser.com/datasheet/2/54/PEC11R-777457.pdf
 *
 * Resolution: 24 PPR
 *
 * Nice info on calculating CPR etc:
 * https://www.cuidevices.com/blog/what-is-encoder-ppr-cpr-and-lpr#cpr
 *
 */

// Pulses per revolution
constexpr auto ppr = 24;
// Cycles per revolution
constexpr auto cpr = ppr * 4; // Quadrature, see above
constexpr auto degreesPerPosition = 360.0 / cpr;
constexpr auto radiansPerPosition = degreesPerPosition * (M_PI / 180.0);

class EncoderVelocity {
public:
  EncoderVelocity(){

  };

  // Setup the encoder object
  void setup() { oldReadTime = millis();  };

  // Update the encoder object
  void loop(int newValue) {

    // Velocity
    // Inspiration:
    // https://forum.arduino.cc/t/rotary-encoder-angular-velocity/703260

	// Elapsed is from a Teensy lib. See here: https://www.pjrc.com/teensy/td_timing_elaspedMillis.html
    // const auto newReadTime = micros();
	const elapsedMillis newReadTime;
	// Sometimes this overfloews the integer, hence the use of max
	const int deltaTime = max(1, abs(static_cast<int>(newReadTime - oldReadTime)));
    // FIXME: This will always be 1, right?
    const auto deltaPosition = abs(newValue - oldValue);

    const float deltaAngle = deltaPosition * radiansPerPosition;

    // Radians per time unit
	const auto scaleAngularSpeed = 1;
    angularSpeed = (deltaAngle / static_cast<float>(deltaTime)) * scaleAngularSpeed;

    oldReadTime = newReadTime;
	oldValue = newValue;

     // Print info
    Serial.println("-------");
    Serial.print("Delta time: ");
    Serial.println(deltaTime);
    Serial.print("Delta pos: ");
    Serial.println(deltaPosition);
    Serial.print("New angular speed: ");
    Serial.println(angularSpeed, 20);
  };

  // Get angular speed (radians per second)
  float getValue(){
	return angularSpeed;
  };

  // ~EncoderVelocity();

private:
  // Hardware Encoder stuff
  int oldValue{0};
  elapsedMillis oldReadTime{0};
  float angularSpeed{0};
};

}; // namespace storkspace
#endif
