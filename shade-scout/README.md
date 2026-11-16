# Shade Scout

Shade Scout is a USB-powered bench/installation aid for comparing how a venetian-blind slat angle changes transmitted daylight. It samples a TSL2591 lux sensor and an AS5600 magnetic angle sensor, then emits timestamped CSV over USB CDC. It is a low-voltage reference design, not a calibrated lux meter or a finished PCB.

## Why it is distinct
Unlike the repository's room/environment monitors, this fixture measures a controlled optical response against blind angle. It is intended for choosing slat settings, comparing fabrics, and documenting daylight experiments; it has no wireless radio, actuator, or mains connection.

## Block diagram

```text
USB 5 V ── Raspberry Pi Pico (RP2040) ── USB CDC CSV
                    │ I2C0 (3.3 V)
                    ├── TSL2591 lux sensor (0x29)
                    └── AS5600 angle sensor (0x36)
```

## Hardware and power
Use a Raspberry Pi Pico (RP2040) powered from its USB connector. The sensor carrier boards must accept 3.3 V logic and be powered from Pico 3V3(OUT); do not connect 5 V sensor logic to GPIO. The TSL2591 and AS5600 share I2C0 with 4.7 kΩ pull-ups to 3.3 V (remove duplicate carrier pull-ups if parallel resistance becomes too low). USB supplies the entire assembly; there is no battery or charging circuit. Estimated current is under 50 mA, excluding other USB devices.

### Pin assignments

| Pico signal | GPIO | Connection |
|---|---:|---|
| I2C0 SDA | GP4 | TSL2591 SDA, AS5600 SDA |
| I2C0 SCL | GP5 | TSL2591 SCL, AS5600 SCL |
| 3V3(OUT) | — | both sensor VCC |
| GND | — | both sensor GND |
| USB | — | power and CDC data |

Pico BOOTSEL and SWD access remain available. Sensor addresses are fixed and do not conflict. Put the magnet on the blind's tilt shaft and mount the AS5600 board concentrically; use a diffuse light source and fixed sensor geometry for repeatable comparisons.

## BOM
See `hardware/BOM.csv`. The design uses breakout boards to avoid claiming an unreviewed custom PCB. The schematic folder contains a native-format status-note schematic, not a complete drawn electrical schematic or fabrication design; the connector-level wiring is described here and in the assembly guide.

## Firmware
The portable C measurement formatter is in `firmware/src/main.c`. The Pico SDK adapter is in `firmware/src/pico_main.c`; build with Raspberry Pi Pico SDK 2.1.1 and CMake 3.16+. Firmware reports `angle_deg,lux` records once per second; sensor acquisition is intentionally isolated behind two functions so board-specific sensor drivers can be added without changing the CSV core. The supplied target adapter is a bring-up scaffold and currently emits explicit unavailable values until the TSL2591/AS5600 driver routines are implemented; it must not be mistaken for a finished instrument.

Build host formatter test:

```sh
cmake -S firmware -B /tmp/shade-scout-build -DSHADE_SCOUT_HOST_TEST=ON
cmake --build /tmp/shade-scout-build
ctest --test-dir /tmp/shade-scout-build --output-on-failure
```

For Pico target builds, set `PICO_SDK_PATH` and configure with `-DPICO_SDK_PATH=...`; see `docs/assembly.md`.

## Limitations and safe use
This is a reference design; it has not been fabricated, calibrated, or tested on physical hardware. Values are comparative only. Do not use for occupational lighting compliance. Disconnect USB before changing wiring. Never connect this circuit to blind motor wiring or mains. Sensor communication failures should produce `nan` readings rather than plausible values.

## Contents
- `schematic/`: KiCad project and schematic source
- `firmware/`: C formatter, Pico SDK target scaffold, host test, CMake
- `hardware/BOM.csv`: parts and quantities
- `docs/assembly.md`: assembly and bring-up
- `docs/api.md`: serial record format
- `scripts/check_bom.py`: standard-library BOM sanity check

Invented and maintained by [jayis1](https://github.com/jayis1).
