# Rain Gauge Keeper

Status: reference design; firmware not target-build-verified, schematic not captured in KiCad, and hardware not fabricated or tested. Author: jayis1.

## Overview
A low-cost, locally logging tipping-bucket rain gauge node for community gardens and small farms. It timestamps each bucket tip, detects implausibly rapid chatter, stores daily totals on microSD, and exposes records over USB serial; it does not require a network or cloud service.

Target user: a gardener who wants a durable rainfall history without a phone app or mains-powered station. Today the workaround is reading a manual gauge and writing totals in a notebook. Success metric: retain timestamped tips across power cycles and export a parseable CSV in under one minute. Differentiator: an offline-first, serviceable logger designed around the pulse output of a conventional reed-switch tipping bucket, not an environmental sensor suite.

## Block diagram

```text
Tipping-bucket reed switch ──> RC + Schmitt input ──> RP2040 ──SPI── microSD
USB-C 5 V ──> 3.3 V regulator ──> RP2040 + microSD
                                  └── USB CDC CSV export
```

## SoC choice
Raspberry Pi RP2040 on a Raspberry Pi Pico module (40-pin header module). Dual-core Cortex-M0+ and native USB make reliable pulse timestamping and direct CDC export straightforward. Use the Pico module rather than a bare RP2040 to avoid an undocumented high-speed PCB layout. The Pico's 3.3 V GPIO is not 5 V tolerant.

## Schematic / pin assignments
Schematic directory contains the connection-level netlist; native KiCad capture and PCB routing remain future work. All signals are 3.3 V logic.

| Pico signal | GPIO | Connection |
|---|---:|---|
| GP2 | 2 | Reed input, external 10 kΩ pull-up to 3V3; switch to GND |
| GP18 | 18 | microSD SPI SCK |
| GP19 | 19 | microSD SPI TX/MOSI |
| GP16 | 16 | microSD SPI RX/MISO |
| GP17 | 17 | microSD CS, 10 kΩ pull-up to 3V3 |
| GND | — | Gauge return, card ground, USB ground |
| VSYS | — | USB module input (Pico USB connector) |

GP2 interrupt uses falling-edge detection; firmware debounces for 40 ms. No GPIO is shared. Keep Pico SWD pads and BOOTSEL accessible. Never connect an external 5 V signal to GPIO.

## Power architecture
USB 5 V powers the Pico through its onboard regulator; the Pico 3V3 output supplies the microSD module. Use a microSD breakout explicitly rated for 3.3 V logic and power. Typical current estimate: Pico ~20–30 mA active plus card idle ~1–5 mA; card write bursts may add 50–100 mA (module-dependent). Design for at least 150 mA on 3V3. No battery is included. Add 100 nF close to the card module and 10 µF local bulk capacitance. USB-C receptacle breakout must include 5.1 kΩ CC1/CC2 pull-downs if not already present.

## BOM
See `hardware/BOM.csv`. Confirm module and breakout revisions before ordering; distributor availability and prices are not asserted.

## Firmware
Portable event-debounce logic is in `firmware/rain_core.c`; Pico SDK adapter is `firmware/main.c`. Build requires Raspberry Pi Pico SDK 2.x and CMake. The firmware stores one CSV row per accepted tip on microSD; card filesystem integration is intentionally not included in this reference implementation, which emits tips over USB CDC and leaves persistence as a board integration task. Therefore this version is a firmware prototype, not a complete logger.

Build: `cmake -S firmware -B firmware/build -DPICO_SDK_PATH=/path/to/pico-sdk && cmake --build firmware/build`. This run did not have Pico SDK installed, so target compilation was not verified. Host core check: `gcc -std=c11 -Wall -Wextra -Werror -fsyntax-only firmware/rain_core.c`.

## Assembly and use
See `docs/assembly.md`. Connect only a passive reed-switch gauge. USB CDC output format is documented in `docs/api.md`. Validate the bucket's volume-per-tip calibration against a measured water volume before interpreting totals.

## Limitations and safety
This is an unvalidated reference design, not a weather-service instrument. Outdoor enclosure, surge/lightning protection, moisture sealing, and calibration are not designed here. Install sensor wiring away from lightning exposure and disconnect USB during storms. The reed input has no galvanic isolation; use a short, protected cable. No claims of accuracy or environmental rating.
