# Splice Scout

Offline cassette-tape splice and leader transition recorder. It measures reflective contrast at a stationary sensing head while a cassette is turned by hand; paired manual index marks let a technician locate suspect transitions without modifying or electrically connecting to the deck. Unlike Tape Tally, which logs reel motion/stalls, Splice Scout records optical reflectance against user-marked tape travel.

Status: reference design; firmware has not been target-built and hardware has not been assembled or calibrated. Not a tape-deck accessory during playback. Do not insert the probe into a powered transport.

## Architecture

RP2040 Raspberry Pi Pico (3.3 V logic) samples a TCRT5000 reflective IR phototransistor through a 10 kΩ load resistor. A 10 kΩ potentiometer provides an adjustable LED current limit (3.3 V rail); the phototransistor collector is connected to ADC0/GP26 with 10 kΩ pull-up to 3V3, emitter to GND. A normally-open momentary button from GP14 to GND marks a transition; GP15 button marks an index point. USB CDC emits CSV. No radio, battery, motor, or connection to the cassette deck.

Block diagram: USB host ↔ RP2040 / Pico ↔ ADC0 ← reflective optical head; GP14/GP15 ← buttons. Power comes only from the Pico USB VBUS regulator. Head is positioned manually in a printed/DIY fixture with a 1–3 mm working gap; avoid contact with the tape oxide.

| Signal | Pico pin | Connection |
|---|---:|---|
| ADC_REFLECT | GP26 / ADC0, physical 31 | sensor collector, 10 kΩ pull-up to 3V3 |
| MARK | GP14, physical 19 | switch to GND, internal pull-up |
| INDEX | GP15, physical 20 | switch to GND, internal pull-up |
| 3V3 | physical 36 | sensor LED series network and pull-up |
| GND | physical 18 | sensor emitter and switches |

## Build and use

Install Raspberry Pi Pico SDK 2.1.1 and its documented ARM GNU toolchain/CMake/Ninja prerequisites. From `firmware`: `cmake -S . -B build -DPICO_SDK_PATH=/path/to/pico-sdk -DPICO_BOARD=pico` then `cmake --build build`. Flash `splice_scout.uf2` to a Raspberry Pi Pico. Open USB CDC at 115200 (baud is advisory for USB CDC), move tape manually past the head, and press MARK to record transitions or INDEX to record a location. CSV columns: uptime_ms,event,adc_raw. The firmware uses a 10 ms debounce and emits raw ADC counts only; it does not claim calibrated optical density or identify splice material.

## BOM (prototype, not fabrication-ready)

See `hardware/BOM.csv`. Connect sensor as described above; confirm sensor pinout from the purchased TCRT5000 datasheet/module before powering. Pico header numbering is standard Raspberry Pi Pico. No PCB or KiCad-native schematic supplied: `schematic/splice-scout.txt` is a net-level reference, not a manufacturing schematic.

## Validation and limitations

The firmware is intentionally conservative and USB-local. Buttons are debounced; no actuator exists. Verify sensor polarity, LED current, ADC range, repeatability at a fixed gap, and lack of tape contact on a bench fixture before use. Host-side syntax check: `gcc -std=c11 -Wall -Wextra -Werror -fsyntax-only firmware/main.c` (Pico headers unavailable to host; this check requires no target SDK only if Pico SDK headers are present, therefore it is not a substitute for a target build). Hardware, optical performance, fixture, target compilation, and calibration remain unverified.