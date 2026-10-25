# Spool Scout

Status: reference design; not fabricated or hardware-validated.

## Problem and user
- User: a desktop 3D-printing operator who wants to catch filament feed interruptions before a long print fails.
- Current workaround: watch the printer or infer a failure from an under-extruded part after the fact.
- Difference: an inline, printer-independent sensor reports actual filament movement and spool rotation locally over USB; no cloud, camera, printer firmware modification, or mains connection.

## Overview
Spool Scout is a low-voltage inline filament-motion monitor for 1.75 mm filament. A slotted measuring wheel and AS5600 magnetic angle sensor measure feed; a slotted optical interrupter observes spool rotation. Disagreement can indicate slack, a snag, or a feed-path jam. It is a monitoring aid, not a printer safety device, and cannot prove that filament reaches the hot end.

## Block diagram
```text
Filament -> grooved 20 mm wheel -> AS5600 angle sensor --I2C--+
Spool flange -> slotted flag -> EE-SX671 optical switch --GPIO-+-> RP2040 Pico -> USB CDC JSONL
                                                              +-> SSD1306 OLED (I2C)
USB 5 V -> Pico regulator/3V3 -> sensors and display
```

## SoC and interfaces
Raspberry Pi Pico (RP2040, module with onboard flash/regulator), USB CDC for logs and configuration. The design uses no radio and no battery. The RP2040 has no internal flash; the Pico module supplies it.

## Pin map
| Pico GPIO | Signal | Peripheral | Notes |
|---|---|---|---|
| GP0 | I2C0 SDA | AS5600, SSD1306 | 4.7 kΩ pull-up to 3V3 |
| GP1 | I2C0 SCL | AS5600, SSD1306 | 4.7 kΩ pull-up to 3V3 |
| GP2 | spool_slot | EE-SX671 output | External 10 kΩ pull-up to 3V3; open collector |
| GP3 | run_led | LED D1 | 1 kΩ series resistor, active high |
| GP4 | user_button | SW1 | Pull-up, active low |
| USB | CDC + supply | Pico | Pico USB connector; do not apply external 5 V simultaneously |

I2C address map: AS5600 0x36; SSD1306 0x3C (7-bit). Shared bus has no address collision. AS5600 is mounted to sense the measuring wheel magnet; verify magnet orientation and air gap per sensor datasheet. OLED module must accept 3.3 V logic.

## Power architecture and budget
USB 5 V powers the Pico; Pico onboard 3.3 V regulator powers low-current peripherals. Do not exceed the selected Pico board's 3V3 output rating. No external power input or charging circuit.

| Mode | Estimated current at 5 V | Basis |
|---|---:|---|
| Idle, display dim | 35 mA | Pico and small OLED; estimate only |
| Active, display lit | 55 mA | Pico + OLED + two sensors |
| Peak design allowance | 100 mA | USB-powered transient allowance; verify with chosen display |

No battery-life claim (USB powered). These estimates are design targets, not measured values.

## BOM summary
See [hardware/BOM.csv](hardware/BOM.csv). Named IC/module parts have manufacturer identifiers; passive and mechanical parts specify standard values/specifications. Costs are omitted rather than fabricated.

## Firmware
The firmware samples wheel angle at 20 Hz, debounces spool pulses, calculates feed and spool displacement, and emits bounded JSONL records over USB CDC. Missing sensor, invalid angle, and stalled motion are reported as states; they never actuate the printer. The C source is a portable algorithm core, not a complete Pico SDK application.

Build/check: `gcc -std=c11 -Wall -Wextra -Werror -fsyntax-only firmware/main.c`. This checks the portable core only. Pico SDK integration, USB CDC, target build, and hardware behavior have not been verified. The repository's requested CMakeLists.txt is present as an integration outline.

## Assembly and bring-up
1. Assemble the low-voltage sensor harness on a breadboard before mounting the filament path.
2. Confirm Pico 3V3 and ground, then verify I2C pull-ups and 0x36/0x3C acknowledgements.
3. Rotate the measuring wheel by hand and confirm angle changes monotonically; check the spool optical flag produces clean edges.
4. Compare 100 mm of manual filament travel against the reported wheel circumference; calibrate the effective circumference in firmware.
5. Run unloaded for 30 minutes and check for false jam indications before attaching to a printer.

## Validation and limitations
Firmware algorithm syntax check is intended; physical assembly, sensor calibration, USB target integration, KiCad capture/ERC, and print trials are not performed. The schematic directory contains a pin/net reference, not a native KiCad schematic. Wheel slip, spool inertia, reverse feed, optical contamination, and filament path geometry affect measurements. This design only reports observations and must not be relied on to prevent equipment damage.

## Safety and license
Use only USB 5 V SELV power. Keep filament guides rounded and away from hot ends, belts, and moving axes; mount so the monitor cannot snag or obstruct printer motion. No medical, security, or regulatory claims. MIT; see repository license.
