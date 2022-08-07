#include "hardwareencoder.hpp"

namespace storkspace {

void HardwareEncoder::loop() {
  // Read encoder
  newValue = hardwareEncoder.read();

  if (rawValue != newValue) {
    encVelocity.loop(newValue);

    // Going backwards or forwards?
    values.sign = get_sign(static_cast<float>(newValue - rawValue));

    values.angularSpeed = encVelocity.getValue();

    // Calculate values scaled by velocity
    constexpr auto sevenBitMaxValue = pow(2, 7);
    constexpr auto fourteenBitMaxValue = pow(2, 14);
    values.velocity7 =
        values.sign * max(1, values.angularSpeed * sevenBitMaxValue);
    values.velocity14 =
        values.sign * max(1, values.angularSpeed * fourteenBitMaxValue);
    values.velocityFloat = values.sign * values.angularSpeed;

    // Accumulate values
    // NOTE: Probably unneccessary since this will be done in VirtualEncoder
    // value7 = value7 + values.velocity7 ;
    // value14 = value14 + values.velocity14 ;
    // valueFloat = valueFloat + values.velocityFloat;
#ifdef DEBUG

    Serial.print("rawValue: ");
    Serial.println(rawValue);

    Serial.print("sign: ");
    Serial.println(values.sign);

#endif

    // Update state
    rawValue = static_cast<int>(smoother.loop(static_cast<float>(newValue)));
  }
}

void HardwareEncoder::setup() {
  smoother.setup();
  encVelocity.setup();
  rawValue = hardwareEncoder.read();
}

const EncoderValues &HardwareEncoder::getValues() const { return values; }

} // namespace storkspace
