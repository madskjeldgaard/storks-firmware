#include "storks.hpp"

namespace storkspace {

constexpr int menuEncPinA = 31;
constexpr int menuEncPinB = 32;
constexpr int menuEncButtonPin = 33;

Storks::Storks()
    : menuEncoder(
          MenuEncoder(this, menuEncPinA, menuEncPinB, menuEncButtonPin)){};
Storks::~Storks() {}

void Storks::setup() {

  // #ifdef DEBUG
  //   while (!Serial)
  //     ;
  // #endif

  storksdisplay.setup();

  // Setup hardware
  buttonMux.setup();

  // Setup OSC
  const auto targetPort = 1234;
  const auto my_ip = IPAddress(192, 168, 0, 1);
  const auto target_ip = IPAddress(169, 254, 85, 44);
  osc.setup(targetPort, my_ip, target_ip);
  osc.sendEncoder(666);

  // Encoder setup
  for (uint16_t numEncoder = 0; numEncoder < hardwareEncoders.size();
       numEncoder++) {
    Serial.print("Setting up hardware encoder num ");
    Serial.println(numEncoder);
    hardwareEncoders[numEncoder].setup();
  }

  // Menu encoder
  menuEncoder.setup();

  // Setup channels
  for (std::size_t chanNum = 0; chanNum < channels.size(); chanNum++) {
    const auto midichan = chanNum;
    channels[chanNum].setup(hardwareEncoders, &buttonMux, midichan, &osc, &storksdisplay);

    // Make sure all channels are off by default
    channels[chanNum].toggle(false);
  }

  // Start at channel 0 by default
  currentChannel = 0;

  // Set initial channel and enable it
  changeChannel(currentChannel);
}

void Storks::loop() {

  storksdisplay.loop();

  // Update hardware stuff
  for (std::size_t encoderNum = 0; encoderNum < hardwareEncoders.size();
       encoderNum++) {
    hardwareEncoders[encoderNum].loop();
  }

  menuEncoder.loop();
  buttonMux.loop();

  //  for (std::size_t index = 0; index < numHardwareButtons; index ++) {
  // Serial.printf("Button num %i: %i\n", index, buttonMux.muxvals[index]);
  //  }

  // Update virtual stuff
  for (std::size_t chanNum = 0; chanNum < channels.size(); chanNum++) {
    channels[chanNum].loop();
  }

  // Send osc bundles if any
  osc.loop();
}
} // namespace storkspace
