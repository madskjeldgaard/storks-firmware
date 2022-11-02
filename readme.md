# Storks firmware

This repository contains the firmware for the Storks controller (a MIDI and OSC controller for music applications).

## Acknowledgements

This project uses libraries from other people and would like to thank the developers of those for their work!

- The Arduino Framework
- QNEthernet by Shawn Silverman (MIT License)
- Adafruit GFX (BSD License)
- Adafruit SSD1306 (BSD License)
- Adafruit BusIO (MIT License)
- Teensy hardware and associated usb libraries
- Thom Johansen at Notam for inspiration

## Installation 

This project uses `platformio`. Make sure you have that installed, then connect the Teensy to your computer and run the following command:

```bash
git clone https://github.com/madskjeldgaard/storks-firmware
cd storks-firmware
pio run -t upload
```
