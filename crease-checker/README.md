# Crease Checker

Offline paper-fold repeatability comparator for bookbinders, origami educators, print shops, and paper-conservation labs. A flat pressure bar is lowered onto a folded sample; a load cell records clamp force while a magnetic angle sensor records lever position. It emits raw force/angle samples over USB CDC for offline comparison across repeated folds. Unlike the repository's paper-grain, crease-transmission, and fold-event devices, this measures mechanical crease closure under a controlled manual load rather than optical transmission, bending direction, or event count.

Status: reference prototype; no target build, calibration, PCB, KiCad-native schematic, electrical-rule check, or physical test. This is a comparative fixture, not a paper-quality standard. Results depend on platen parallelism, sample placement, loading speed, and calibration.

## Design

RP2040 Raspberry Pi Pico powered only by USB (5 V input, onboard 3.3 V regulator). A 5 kg full-bridge load cell feeds an HX711 module at 3.3 V logic; an AS5600 magnetic angle sensor measures travel by reading a diametrically magnetized magnet on a pivot arm. The lever converts bar motion into rotation; travel is derived from the calibrated arm radius and angle. A normally-open button marks sample start/end. No battery, radio, mains, heater, or actuator. Use a compliant, parallel pressure bar and a mechanical stop so the load cell cannot be overloaded.

Block: USB → Pico/RP2040 → HX711 load-cell force + AS5600 I2C angle + button → timestamped CSV over USB CDC.

| Signal | Pico pin | Notes |
|---|---:|---|
| HX711 DOUT | GP2 | 3.3 V logic |
| HX711 SCK | GP3 | 3.3 V logic; idle low |
| AS5600 SDA | GP4 | I2C0, 4.7 kΩ pull-up to 3V3 |
| AS5600 SCL | GP5 | I2C0, 4.7 kΩ pull-up to 3V3 |
| Capture button | GP6 | button to GND, internal pull-up |
| USB CDC/power | onboard | host supplies 5 V |

Use a Pico-compatible module with accessible GPIO. HX711 excitation must be 3.3 V so its digital outputs never exceed RP2040 input limits. Check the breakout board regulator/level-shifter arrangement before wiring. AS5600 address is fixed at 0x36. Keep load-cell analog wiring twisted and away from USB; tare and calibrate with known masses. Linear travel requires a fixed pivot radius and a documented angle-to-displacement calibration; do not infer absolute force or distance without calibration.

## Build

Requires Raspberry Pi Pico SDK 2.1.1 and ARM GNU toolchain. `cmake -S firmware -B build -DPICO_SDK_PATH=/path/to/pico-sdk && cmake --build build`. Host-independent arithmetic smoke test: `cc -std=c11 -Wall -Wextra -Werror firmware/core.c firmware/test_core.c -o /tmp/crease-checker-test && /tmp/crease-checker-test`.

Flash the UF2 by holding BOOTSEL while connecting the Pico, then copy `build/crease-checker.uf2` to RPI-RP2. Connect USB CDC at 115200 baud. Press the capture button to toggle a run; firmware emits `millis,raw_force,angle_raw` CSV lines and a run-end marker. Samples are raw counts; host calibration converts them to physical units. Device firmware does not actuate the press.

## First power and use

Inspect for shorts; power the Pico from a current-limited USB source (initial limit 100 mA) with sensors disconnected. Confirm 5 V USB and 3.3 V rail, then connect the 3.3 V sensor modules one at a time and verify no part warms. Stop if the rail collapses or any component heats. Secure the load cell mechanically before placing paper. Calibrate force with known masses across the intended range, and travel with a ruler at several positions. Keep hands clear of pinch points; the lever is manually operated and must have a hard stop.

## Files and limitations

- `schematic/netlist.md`: explicit wiring/net reference, not a native KiCad drawing.
- `hardware/BOM.csv`: prototype parts and alternatives.
- `firmware/`: RP2040 SDK C target and host-testable core.
- `scripts/calibrate.py`: convert raw captures using user-provided calibration constants.
- `docs/assembly.md`, `docs/API.md`: fixture assembly, calibration, serial format.

The sensor arrangement is a proposed reference design. Module-level HX711/AS5600 breakout circuits vary; inspect their schematics before assembly. No performance, accuracy, or repeatability claims are validated. Author: jayis1.