#include "storks.hpp"

namespace storkspace {

constexpr int menuEncPinA = 32;
constexpr int menuEncPinB = 31;
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
  auto doOnEachEncoder = [this](HardwareEncoder &enc) { enc.setup(); };
  std::for_each(begin(hardwareEncoders), end(hardwareEncoders),
                doOnEachEncoder);

  // Menu encoder
  menuEncoder.setup();

  // Setup channels
  for (std::size_t chanNum = 0; chanNum < channels.size(); chanNum++) {
    const auto midichan = chanNum;
    channels[chanNum].setup(hardwareEncoders, &buttonMux, midichan, &osc,
                            &storksdisplay);

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
  readMIDIInput();

  // Update hardware stuff
  auto doOnEachEncoder = [this](HardwareEncoder &enc) { enc.loop(); };
  std::for_each(begin(hardwareEncoders), end(hardwareEncoders),
                doOnEachEncoder);

  menuEncoder.loop();
  buttonMux.loop();

  //  for (std::size_t index = 0; index < numHardwareButtons; index ++) {
  // Serial.printf("Button num %i: %i\n", index, buttonMux.muxvals[index]);
  //  }

  // Update virtual stuff
  auto doOnEachChannel = [this](Channel &chan) { chan.loop(); };
  std::for_each(begin(channels), end(channels), doOnEachChannel);

  // Send osc bundles if any
  osc.loop();
}

void Storks::readMIDIInput() {
  while (usbMIDI.read() > 0) {
    // Read Sysex
    if (usbMIDI.getType() == usbMIDI.SystemExclusive) {
      auto *data = usbMIDI.getSysExArray();
      auto len = usbMIDI.getSysExArrayLength();

      // Print data
      Serial.print("SYSEX: ");
      for (int i = 0; i < len; ++i) Serial.printf("%i ", data[i]);
      Serial.println();
    }
  }
}

}  // namespace storkspace
