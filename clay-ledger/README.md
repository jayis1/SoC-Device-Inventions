# Clay Ledger

Offline ceramic test-bar drying-shrinkage logger built around a Raspberry Pi Pico (RP2040). It records the changing distance between two embedded magnetic targets on a clay test bar, along with local temperature and relative humidity, and exports timestamped CSV over USB CDC. It is a comparative studio instrument, not a calibrated dimensional metrology system or a kiln controller.

## What it solves

Potters and ceramic-materials students often compare clay bodies and drying schedules by measuring bars with calipers at intervals. Clay Ledger keeps a continuous, repeatable record without cameras, cloud accounts, or mains-connected machinery. Each bar is mounted in a low-force linear fixture; a diametrically magnetized slider drives an AS5600 magnetic angle sensor through a pulley. Firmware unwraps the angular position into carriage travel. The sensor measures fixture motion, not absolute clay dimensions: calibrate the fixture against a ruler and validate repeatability for each setup.

This differs from the repository's moisture, shrinkage-adjacent and process loggers: the primary measurement is displacement of a dedicated ceramic test bar during drying, with environment context, not moisture content or room/clay sensing alone.

## Design status and safety

Reference design; not fabricated, calibrated, or target-built. The schematic is a human-readable connection/netlist drawing, not a native KiCad schematic. No accuracy, shrinkage percentage, or performance claim is validated. Low-voltage USB-powered electronics only; keep the sensor board away from wet clay and wipe-down liquids. No mains or kiln connections. Use a non-load-bearing, guarded carriage and avoid pinching fingers in the fixture.

## Hardware

- MCU: Raspberry Pi Pico / RP2040, 3.3 V GPIO, Pico SDK 2.1.1 target.
- Position sensor: AS5600L magnetic rotary position sensor breakout, I2C address 0x36, 3.3 V logic. A small diametrically magnetized magnet is fixed to a belt/pulley carriage; one full turn maps to the configured travel range.
- Environment: SHT31-D temperature/RH breakout at I2C address 0x44.
- Interface: USB CDC console and CSV; onboard LED indicates sampling.
- Supply: USB 5 V to Pico, onboard 3.3 V regulator supplies both 3.3 V sensor breakouts. Total sensor load is expected below 20 mA; verify the selected breakout's regulator/level-shifter configuration before assembly. I2C pull-ups are 4.7 kΩ to 3.3 V on this board only.

### Block diagram

```text
USB 5 V ── Pico RP2040 ── USB CDC CSV
                 │ 3V3
                 ├── I2C0 ── AS5600L ── magnet + pulley + test-bar carriage
                 ├── I2C0 ── SHT31-D ── local air temperature / RH
                 └── GP25 ── status LED
```

### Pin assignments

| Pico signal | Pin | Connection |
|---|---:|---|
| I2C0 SDA | GP4 / physical 6 | AS5600 SDA and SHT31 SDA |
| I2C0 SCL | GP5 / physical 7 | AS5600 SCL and SHT31 SCL |
| 3V3(OUT) | physical 36 | Both sensor breakout VCC (3.3 V only) |
| GND | physical 3 | Both breakout grounds, common return |
| LED | GP25 / onboard | Sampling heartbeat |
| USB | micro-USB | Power and USB CDC data |

The Pico pin numbers refer to Raspberry Pi Pico, not the RP2040 package. Do not connect sensor breakout VCC to VBUS. Avoid breakouts that pull I2C up to 5 V. Add 100 nF local bypass at each module if not present on the breakout. Keep I2C leads short; no external connector is specified for wet-area use.

## Repository contents

- `schematic/netlist.md` — connection-level reference schematic.
- `hardware/BOM.csv` — parts and quantities.
- `firmware/` — Pico SDK C application and CMake project.
- `docs/ASSEMBLY.md` — fixture, wiring, and first-power procedure.
- `docs/API.md` — serial protocol and CSV format.
- `scripts/plot_log.py` — optional CSV plotting helper.

## Build and run

Install Raspberry Pi Pico SDK 2.1.1 and its documented ARM GCC/CMake/Ninja dependencies. Set `PICO_SDK_PATH` to that SDK checkout, then:

```sh
cmake -S firmware -B build -DPICO_BOARD=pico
cmake --build build
```

Copy `build/clay_ledger.uf2` to the Pico in BOOTSEL mode. On boot the program scans the I2C bus, starts USB CDC, and samples once per second. Missing sensors are reported as `ERR` rather than blocking. Send `z` followed by Enter to set the current position as zero; send `h` for help. Ctrl-C is not required; unplug to stop.

The firmware and plot helper have host-side syntax checks only in this reference release. Target compilation requires the Pico SDK; physical behavior and sensor calibration are unverified. Raw angular displacement is converted using configured pulley circumference, not a calibrated length standard. RH near condensation, fixture backlash, clay slip, magnet misalignment, and sensor dropout can bias results.

## License and authorship

MIT. Invented and maintained by [jayis1](https://github.com/jayis1).
