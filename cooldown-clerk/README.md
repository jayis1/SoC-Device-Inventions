# Cooldown Clerk

USB-powered, offline drink-cooling curve logger for comparing mugs, lids, and insulation. A Raspberry Pi Pico (RP2040) samples a waterproof DS18B20 probe once per second and emits newline-delimited CSV over USB CDC. A reserved I²C OLED connector is documented for a future display revision; current firmware does not drive it. Press the button to start/stop a run; no radio, battery, heater, or mains connection is present. This is a comparative kitchen/education instrument, not a food-safety thermometer.

## Readiness

Design reference only. Firmware core and host checks are not target-build or hardware validation. No PCB fabrication, calibration, or accuracy claim. Use a separately calibrated food thermometer for food-safety decisions.

## Why it is distinct

The repository includes environmental loggers, kitchen timers/scales, and multiple temperature probes, but none records an offline, user-triggered beverage cooling curve for side-by-side vessel comparison. It does not control heating or infer safety.

## Architecture

Pico VBUS supplies the board and 3.3 V sensor logic. No display is implemented in firmware; the connector is reserved. Sensor readings outside −20…110 °C or disconnected are reported as `#sensor_error` and omitted from numeric records. CSV fields: `elapsed_s,temp_c`; USB CDC is the only export interface.

### Pin assignment

| Pico signal | Connection |
|---|---|
| GP2 | DS18B20 DQ, 4.7 kΩ pull-up to 3V3 |
| GP4 / GP5 | OLED SDA / SCL, I²C0 |
| GP15 | momentary switch to GND, internal pull-up |
| 3V3(OUT) | DS18B20 VDD, OLED VCC, pull-up |
| GND | common ground |
| VBUS | USB 5 V input to Pico board only |

Use an SSD1306-compatible 128×64 module explicitly rated for 3.3 V I/O. The Pico board's USB connector is the sole power source. Do not connect external 5 V to sensor or GPIO. Typical board current is module-dependent; budget up to 100 mA at USB input. Probe sleeve may be immersed; keep cable joint and electronics dry.

## Build

Target: Raspberry Pi Pico SDK 2.1.1, CMake 3.16+, ARM GNU toolchain supported by that SDK. From firmware directory: `cmake -S . -B build -DPICO_SDK_PATH=/path/to/pico-sdk -DPICO_BOARD=pico` then `cmake --build build`. Flash `cooldown_clerk.uf2` via BOOTSEL. This reference firmware uses SDK GPIO/stdio APIs; target build has not been run in this environment.

## Contents

- `schematic/netlist.md`: explicit connectivity reference (not native KiCad CAD)
- `hardware/BOM.csv`: parts and quantities
- `firmware/`: C implementation and CMake project
- `docs/ASSEMBLY.md`: safe assembly/bring-up
- `docs/API.md`: serial protocol
- `scripts/plot_log.py`: host CSV plotter

## Limitations

No physical prototype, calibration, KiCad ERC, or target build has been performed. Sensor accuracy depends on the selected genuine DS18B20 and probe construction; thermal lag and placement affect comparisons. Use cleanable, food-contact-rated probe hardware and follow its manufacturer's temperature limits. Author: jayis1.
