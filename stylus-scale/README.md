# Stylus Scale

A small USB-powered force gauge for setting turntable stylus tracking force without relying on an uncalibrated counterweight dial.

## Problem and user
Vinyl listeners and repairers need to set tracking force within a cartridge maker's recommended range. Counterweight scales are convenient but their markings can be hard to read and do not measure force at record height. Stylus Scale places a low-capacity load cell at a repeatable record-height platform and displays the measured gram-force. It is a setup aid, not a record-protection guarantee; a known calibration mass remains necessary.

## Features
- 5 g strain-gauge load cell and HX711 24-bit bridge ADC
- RP2040 Pico USB-powered controller; local SSD1306 display and USB CDC text output
- Tare and calibration commands over serial; no wireless or mains circuitry

## Block diagram
```text
5 g load cell -> HX711 (RATE low, 10 SPS) -> Pico RP2040 -> USB CDC
                                                   +-> SSD1306 OLED (I2C)
USB 5 V -> Pico module regulator -> 3V3 logic and display
```

## SoC choice
The Raspberry Pi Pico module provides RP2040, external flash, regulator, USB, and a supported pico-sdk path. It is inexpensive and has adequate GPIO; the design uses USB rather than adding a battery or radio. This differs from the repo's spool-motion monitor: the measured quantity is static gram-force at a turntable's record plane, using a bridge ADC rather than optical/magnetic motion sensing.

## Pin assignments
| Pico pin | Function | Connection |
|---|---|---|
| GP0 | I2C0 SDA | SSD1306 SDA, 4.7 kΩ pull-up to 3V3 |
| GP1 | I2C0 SCL | SSD1306 SCL, 4.7 kΩ pull-up to 3V3 |
| GP2 | HX711 DOUT | ADC data output |
| GP3 | HX711 SCK | Clock, held low except read cycle |
| GP4 | Tare button | Button to ground, internal pull-up |
| USB | power + CDC | Pico USB connector |

RP2040 GPIO assignments are within GP0–GP4; ADC, boot, and flash pins are unused. OLED I2C address is 0x3C; HX711 is a dedicated two-wire interface, not I2C. Confirm the chosen display breakout is 3.3 V compatible.

## Power architecture and budget
USB 5 V powers the Pico; its onboard regulator supplies 3V3 logic. No battery, charger, or external voltage input is included. Design estimates (not measured): Pico 25–40 mA at 5 V, OLED 8–20 mA depending on lit pixels, load cell/HX711 under 5 mA; budget 80 mA typical, 120 mA maximum allowance. Do not draw from the Pico 3V3 output beyond its board rating.

## BOM
See [hardware/BOM.csv](hardware/BOM.csv). Prices and live stock are not asserted. The load cell's capacity and bridge wiring vary by supplier; use a 5 g four-wire strain-gauge cell and verify its lead colors with its datasheet.

## Firmware
The firmware design uses HX711 10-SPS conversions, a bounded median-of-five filter, tare offset, and a calibrated counts-per-gram coefficient. The included C core is host-compilable and implements the conversion/filtering; Pico SDK GPIO/USB integration is a documented integration point, not represented as a completed target application.

## Build and flash
Portable algorithm check:
```sh
gcc -std=c11 -Wall -Wextra -Werror -Ifirmware/include -fsyntax-only firmware/src/stylus_scale.c
```
For target integration, install pico-sdk (1.5 or later), initialize its environment, and build the CMake project from `firmware/`; this initial reference does not yet include the SDK import file or USB CDC driver, so it is not a flashable image. Never treat the syntax check as a target build.

## Assembly and calibration
See [docs/ASSEMBLY.md](docs/ASSEMBLY.md). Calibrate at the same platform height used for measurement with a traceable 1 g or 2 g mass; never rest a cartridge stylus on the bare load-cell element. Use a broad, flat, nonmagnetic platform and a mechanical overload stop. USB output format and calibration procedure are in [docs/API.md](docs/API.md).

## Safety and limitations
Low-voltage USB only. This reference has no native KiCad capture, PCB layout, or validated mechanical fixture. Load-cell creep, platform friction, off-center loading, vibration, and the display's electromagnetic/mechanical coupling can affect readings. Firmware, target build, accuracy, and hardware behavior have not been verified. This is not a substitute for cartridge instructions or careful handling of a delicate stylus.

## License
MIT, consistent with the repository license. Invented by jayis1.
