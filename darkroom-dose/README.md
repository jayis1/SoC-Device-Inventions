# Darkroom Dose

A low-voltage, open-reference enlarger exposure timer for community darkrooms. The operator places a photodiode probe at the easel; the RP2040 integrates light during a timed exposure and drives a safe extra-low-voltage enlarger-enable output. It is a measurement aid, not a mains switch or calibrated photographic instrument.

## User, problem, success target

For a volunteer darkroom operator who currently estimates exposure by stopwatch and repeated test strips, Darkroom Dose aims to make repeatable exposure timing easier across sessions. Design target: repeat a stored exposure dose within 2% under unchanged lamp, paper, aperture, geometry, and probe placement; this target is unverified.

## Status

Design-stage reference. Firmware, electrical design, and procedure have not been validated on hardware. No PCB is included; schematic is a textual connection-level reference, not a KiCad design or ERC-checked schematic. Do not connect the output to mains voltage or safety-critical equipment.

## Why this is distinct

The collection already covers color sensing, optical film-gate timing, and several measurement instruments, but not photographic exposure-dose integration. Unlike a shutter pulse logger or color sampler, this measures integrated illumination at the print plane and gates an enlarger through an isolated low-voltage interface. RP2040 is selected for its accessible ADC and PIO-capable timing; it avoids radio and associated setup.

## Block diagram

```text
Photodiode probe -> OPA333 transimpedance amplifier -> RC filter -> RP2040 ADC0
                                                     RP2040 GPIO -> optocoupler -> isolated SELV enable contact
USB 5 V -> Pico board regulator -> 3V3 logic; USB CDC -> host calibration/logging
Buttons + OLED (I2C) <-> RP2040
```

## Components and interfaces

The design targets a Raspberry Pi Pico (RP2040 board, not bare chip), powered from USB 5 V. The amplifier is intended as a low-bias OPA333 transimpedance stage with BPW34 photodiode, but the feedback network and bias remain an explicit design hold point (see schematic/netlist); do not build this analog section from an assumed resistor value. The output uses an LTV-817 optocoupler with a floating phototransistor output, not a relay contact; it requires a separately designed low-voltage receiver. Never put mains on the Pico or this prototype board.

| Function | Pico pin | RP2040 GPIO | Notes |
|---|---:|---:|---|
| Photodiode TIA output | 31 | 26 / ADC0 | Keep 0–3.0 V; analog ground return |
| OLED SDA | 6 | 4 | I2C0; 4.7 kΩ pull-up to 3V3 |
| OLED SCL | 7 | 5 | I2C0; address 0x3C |
| Start/stop button | 9 | 6 | Active-low, internal pull-up |
| Mode button | 10 | 7 | Active-low, internal pull-up |
| Isolated enable LED drive | 11 | 8 | Series resistor, fail-off at reset |
| USB CDC | USB | USB | Pico native USB |

GPIO26 is an RP2040 ADC input; GPIO4/5, 6/7, and 8 are not QSPI-reserved pins. All logic is 3.3 V. Check the exact Pico board schematic before assembly; pin numbers above are Pico header positions.

## Power and measurement

USB 5 V powers the Pico board; its onboard regulator supplies 3V3. No battery or charger is fitted. Estimated draw: Pico 25–45 mA while active (board-dependent), OLED 10–20 mA, sensor/indicator under 5 mA; estimated total 40–70 mA. These are design estimates, not measurements. USB current limit and cable quality govern operation. The photodiode current-to-voltage conversion depends on feedback resistor and illumination; initial firmware uses a bounded ADC reading and reports raw integrated counts rather than claiming lux or paper exposure units. Calibrate per enlarger and paper with test strips.

## Build

Requires Raspberry Pi Pico SDK 2.x and CMake. From this directory:

```sh
cmake -S firmware -B firmware/build -DPICO_SDK_PATH=/path/to/pico-sdk
cmake --build firmware/build
```

Flash `darkroom_dose.uf2` using the Pico BOOTSEL USB mass-storage procedure. Firmware uses USB CDC for status; exposure starts only on an explicit button press, times out after 120 seconds, and output GPIO is low on boot and on error. The current source is a minimal firmware core; OLED driver and calibrated dose menu are not implemented. See docs/API.md for the present serial protocol and limitations.

## Bring-up

1. Assemble and inspect with USB disconnected; confirm no short between 3V3 and GND.
2. Power the Pico only. Measure 3V3 at the board test point; expected 3.2–3.4 V.
3. With external enlarger disconnected, flash firmware and observe USB CDC startup text.
4. Confirm output LED is off at boot, turns on only during an exposure, then turns off at timeout or stop.
5. Check ADC raw counts change when the probe is shaded/illuminated. Do not infer calibrated dose without test-strip calibration.

## Files

- `schematic/netlist.md` — textual reference wiring (not KiCad/ERC validated)
- `hardware/BOM.csv` — provisional component list and indicative costs
- `firmware/` — Pico SDK C application
- `docs/ASSEMBLY.md`, `docs/API.md` — assembly and interface notes
- `scripts/dose_calibrate.py` — offline dose-log inspection helper

## Limits and safety

This is not a certified enlarger controller, photometer, or electrical safety device. Do not switch or expose mains conductors with this prototype. The optocoupler is signal isolation only; its output must drive an appropriately rated external SELV interface designed and enclosed by a qualified person. Validate isolation, creepage, and exposure performance before any real use. Firmware compilation, ERC, optical calibration, and hardware tests remain outstanding.
