# Shutter Tick

Status: reference design; firmware core host-compiled and unit-tested; schematic is a human-readable connection netlist, not a KiCad capture; no physical prototype or calibration has been performed.

Shutter Tick helps a camera repair hobbyist compare a mechanical leaf shutter's actual open time with its marked setting. A phototransistor behind a small opaque suction-cup light hood senses the shutter's transmitted light; the Raspberry Pi Pico timestamps the rising and falling edges and streams pulse duration over USB CDC. It is an inexpensive bench screening aid, not a calibration instrument. Unlike camera light meters, it measures the shutter's light gate directly.

Product brief: user is a film-camera repairer checking a camera after cleaning. Success is repeatable pulse-duration record over 0.1–1000 ms; timestamp granularity is 1 µs but measurement uncertainty is dominated by polling and optical threshold, which are not characterized; compare repeated shots and assess consistency. Non-goals: focal-plane curtain travel analysis, flash synchronization, exposure calibration, or acceptance certification. Target build cost is under $15 excluding Pico; soldering on perfboard is expected.

## Architecture

```text
Shutter light -> opaque hood -> phototransistor/resistor threshold -> Pico GP15
Pico timer capture -> duration calculation -> USB CDC JSONL -> host CSV script
USB 5 V -> Pico onboard regulator -> 3.3 V sensor and logic
```

## SoC and pin assignments

The RP2040 Pico is selected for inexpensive USB CDC, a microsecond timer, and straightforward 3.3 V GPIO without a radio or battery. Use the Raspberry Pi Pico (RP2040) board and SDK 2.1.x.

| Pico signal | GPIO | Connection |
|---|---:|---|
| GP15 | 15 | Phototransistor collector node; 10 kΩ pull-up to 3V3 |
| GP25 | 25 | Status LED through 1 kΩ |
| 3V3(OUT) | — | Pull-up and sensor circuit |
| GND | — | Common return |
| VSYS | — | Pico USB-derived supply only |
| USB | — | Power, programming and CDC output |

All logic is 3.3 V. GP15 is an ordinary digital input; no boot/debug pins are repurposed. The external sensor network must never be driven above 3.3 V.

## Power and BOM

USB 5 V powers the Pico; estimate 20–40 mA including LED, budget 60 mA. No battery, charger, regulator external to Pico, or mains connection. Values are approximate, excluding tools and camera.

See [BOM](hardware/BOM.csv) and [connection netlist](schematic/netlist.md). No custom PCB is provided.

## Firmware and use

Firmware waits for the illuminated sensor state (electrical low), records `time_us_64()`, then waits for darkness (electrical high) and emits `{"duration_us":N,"valid":true}`. A timeout emits a bounded error record; do not leave a shutter blocked while testing. The core timing state machine is host-tested in `firmware/test_timing.c` with GCC; Pico SDK target build was not run in this environment.

Build with Raspberry Pi Pico SDK 2.1.x: set `PICO_SDK_PATH`, run `cmake -S firmware -B build -DPICO_BOARD=pico`, then `cmake --build build`. Flash `build/shutter_tick.uf2` using BOOTSEL. Connect a USB serial terminal or run `python3 scripts/capture.py --port /dev/ttyACM0 --output shots.csv`. Each shot should be triggered manually; repeat at least five times per setting and compare spread. Sensor threshold and hood geometry require adjustment for each camera.

## Assembly and limitations

Follow [assembly guide](docs/assembly.md); protocol is documented in [API reference](docs/api.md). Verify 3.3 V rail and GPIO level before connecting the sensor. The sensor observes light only; it does not touch or actuate the camera. Strong ambient light, translucent curtains and changing aperture may affect threshold timing. This is not a certified shutter tester and cannot measure exposure uniformity or curtain travel. Do not rely on it for safety-critical or commercial acceptance decisions.

The schematic is an explicit netlist pending KiCad capture. Firmware core has been host-compiled and unit-tested, but the Pico-target firmware, complete device, and physical measurements are unverified. Invented and maintained by jayis1. MIT, under the repository license.
