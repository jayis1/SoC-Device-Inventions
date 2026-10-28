# Tape Tally

Offline, USB-powered RP2040 optical cassette-reel motion logger for diagnosing intermittent transport stalls in vintage cassette decks. A reflective IR sensor watches a removable paper marker on a reel; USB CDC emits timestamped transitions and stall-duration events. It observes motion only and does not drive or electrically connect to the deck.

## Status
Design reference, not physically validated. Firmware is portable C event-processing core; no Pico SDK target build was attempted. Schematic is a text netlist/pin map, not a KiCad drawing. No PCB or fabrication files. Optical geometry and thresholds require bench calibration.

## Novelty and use
For tape-restoration hobbyists who currently watch reels by eye, this provides a repeatable time history of transport pauses without modifying the mechanism. Unlike a general tachometer, it records low-speed motion/stall intervals from one reel with an offline USB serial interface. Relative to the repository's optical shutter/dial/flow loggers, the target is tape transport diagnosis and the measured event is reel rotation.

Success criterion: report a stall lasting at least 500 ms within 50 ms of its end, at reel speeds from 0.2 to 5 revolutions/s, after calibration. This is a design target, not measured performance.

## Architecture
`Reflective marker → TCRT5000L phototransistor + LMV331 comparator → RP2040 GPIO2 → USB CDC JSONL`.

Pico board is powered from USB 5 V. Its onboard regulator supplies 3.3 V; do not connect the sensor circuit to 5 V logic. TCRT5000 emitter is powered from 3.3 V through 220 Ω; phototransistor collector uses 10 kΩ pull-up to 3.3 V and feeds the comparator. LMV331 comparator is powered at 3.3 V; open collector output has 10 kΩ pull-up. 100 nF local decoupling. Add 100 kΩ/10 kΩ divider and 10 nF filter for adjustable threshold from the 3.3 V rail (trim pot). Sensor-to-marker spacing and ambient light shielding are mechanical/calibration concerns.

### Pins
| Signal | Pico GPIO | RP2040 pad | Notes |
|---|---:|---:|---|
| comparator output | GP2 | 4 | falling/rising edge input |
| USB CDC | USB | — | onboard connector |
| SWD | SWDIO/SWCLK | — | onboard debug connector |
| 3V3 sensor rail | — | 36 | board output, sensor only |
| GND | — | 38 | common ground |

RP2040 GPIO2 is not a boot-strapping pin. Keep GP2 unconnected during firmware programming if external circuitry causes startup interference. Pico board details: Raspberry Pi Pico datasheet, https://datasheets.raspberrypi.com/pico/pico-datasheet.pdf .

### Power estimate
USB 5 V input, Pico board regulator → 3.3 V. Estimated active: Pico 20–40 mA plus IR LED ~9 mA (3.3 V minus ~1.2 V divided by 220 Ω), total 29–49 mA at 3.3 V equivalent; sleep is not a supported operating mode. USB source must provide at least 100 mA. No battery or charging circuit.

## Build and use
The core event logic is independent C11. Integrate `firmware/src/tape_tally.c` into a Pico SDK project and call `tt_sample(level, ms)` every 1 ms; send returned records as JSONL over USB CDC. Connect the comparator output to GP2, attach a high-contrast removable marker to the reel, shield the sensor from room light, then use the threshold adjustment to obtain one clean pulse per marker passage. Never put adhesive or a loose marker where it can contact moving tape.

Output records are `{"event":"pulse","ms":N}` and `{"event":"stall","duration_ms":N}`. This design does not infer tape speed from one mark per turn; it reports rotation intervals and motion gaps. No audio or recording access.

## Verification
Host C syntax check: run `gcc -std=c11 -Wall -Wextra -Werror -fsyntax-only firmware/src/tape_tally.c`. BOM CSV and helper script can be checked with Python 3. Physical performance and target SDK build remain unverified.
