# Twist Index

Offline comparative yarn-twist counter for textile sampling. A hand-turned specimen passes through a low-force guide; an optical interrupter counts one flag on a calibrated spindle per revolution while a RP2040 records elapsed time and operator-entered specimen length. The device reports turns per metre and emits CSV over USB CDC. It is a comparative workshop aid, not a textile certification instrument.

## Distinction and scope

The inventory contains textile force, stretch, linear-density, and loom-event tools, but no yarn-twist-per-length comparator. Twist Index measures spindle revolutions over a manually set specimen length; it does not infer twist from force or image appearance. It has no motor, battery, network, or mains connection.

## Design

USB-powered Raspberry Pi Pico (RP2040), GP15 digital optical-gate input, GP14 active-low capture button, GP25 onboard LED, USB CDC output. A slotted encoder disk mounted to the hand-turned spindle interrupts a 3.3 V logic photogate. The gate output must be 3.3 V push-pull or open-collector pulled up to 3V3; never apply 5 V to GPIO. A 100 nF capacitor is fitted across the sensor supply at the connector. The button connects GP14 to ground and uses the internal pull-up. No external supply or actuator is present.

Block diagram: slotted disk + photogate -> GP15 / RP2040 -> USB CDC CSV. Capture button -> GP14.

| Signal | Pico header | GPIO | Notes |
|---|---:|---:|---|
| Gate output | 20 | GP15 | 3.3 V logic only; rising edge counted |
| Gate VCC | 36 | — | 3V3 OUT |
| Gate GND | 38 | — | common ground |
| Capture button | 19 | GP14 | momentary to GND |
| LED | onboard | GP25 | indicates capture state |

## Operation

Enter the measured specimen length in millimetres over the serial console (`L <mm>`; valid range 10–10000). Send `S` to start, turn the spindle by hand, then press the capture button or send `E` to end. Firmware reports edge count and calculated turns/meter; output is a single CSV header followed by records. Keep fingers clear of the disk and spindle; use a guard around rotating parts. USB disconnects abort a session without storing a partial record.

## Build

Requires Raspberry Pi Pico SDK 2.x and CMake. Set `PICO_SDK_PATH`; run `cmake -S firmware -B build -DPICO_BOARD=pico`, then `cmake --build build`. Flash `build/twist_index.uf2` using BOOTSEL. The portable calculation core has host tests (`firmware/tests/test_core.c`) buildable with `cc -std=c11 -Wall -Wextra -Werror firmware/src/core.c firmware/tests/test_core.c -o /tmp/twist-test && /tmp/twist-test`.

## Status and limitations

Reference design. No PCB layout, physical assembly, calibration, target build, or metrological validation is claimed. A real build must verify sensor threshold, flag geometry, edge rejection, and spindle alignment; the reported value is only as accurate as the manually measured length and the mechanical fixture. Schematic folder contains a connector-level netlist reference, not a KiCad-native drawing. Use guarded, low-speed manual operation only.

## Files

- `schematic/netlist.md` — wiring reference.
- `hardware/BOM.csv` — parts list.
- `firmware/` — portable calculation core and Pico SDK adapter.
- `docs/ASSEMBLY.md`, `docs/API.md` — assembly and serial protocol.
- `scripts/analyze.py` — summarize exported CSV records.

MIT. Invented and maintained by [jayis1](https://github.com/jayis1).
