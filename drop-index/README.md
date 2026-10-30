# Drop Index

Offline emitter uniformity tester for a smallholder grower or irrigation technician checking micro-drip lines at the field edge. A removable cup sits on a 1 kg load cell; firmware reports collected mass and average mass-flow rate over a timed interval over USB serial. It is a comparative maintenance tool, not a calibrated flow meter.

## Why this rather than a commercial inline flow meter?
Inline meters report total line flow and require plumbing changes; Drop Index compares individual outlets without cutting or reconnecting tubing. A kitchen scale plus stopwatch can approximate the task, but does not automatically timestamp or export repeatable outlet-by-outlet readings.

## Targets and limits
- Detect a 1 g change in accumulated water mass after calibration (design target; bench performance not established).
- Fixed 60-second example collection, one reading per second; change the compile-time loop bound to alter duration.
- BOM target about US$15–25 at quantity one, excluding enclosure and tools; estimate only.
- Not suitable for potable-water certification, billing, fertilizer dosing, unattended outdoor use, or claims of absolute accuracy. Dry indoor prototype; keep electronics away from spilled water.

## Block diagram

```text
Emitter -> removable cup -> 1 kg strain-gauge load cell -> HX711 -> RP2040 Pico
                                                            | USB CDC CSV
                                                            + start button
```

## Hardware
The reference uses a Raspberry Pi Pico (RP2040, 3.3 V GPIO), HX711 board and a four-wire 1 kg load cell. HX711 breakout variants differ: check supply and pin labels before wiring. Do not connect 5 V logic to the Pico. Use a splash shield and keep the cup mechanically isolated from cables and the enclosure.

### Pin map
| Pico signal | Connection | Notes |
|---|---|---|
| GP2 | HX711 DOUT | 3.3 V logic required |
| GP3 | HX711 SCK | Idle low; pulse high to clock |
| GP15 | Momentary start button to GND | Internal pull-up |
| GND | HX711 GND, button return | Common ground |
| 3V3(OUT) | HX711 VCC only if breakout supports 3.3 V | Never assume a module's VCC is logic-safe |

Load cell E+/E-/A+/A- connect to matching HX711 excitation and channel-A bridge terminals per its markings; wire colors are not standardized. The Pico's USB supplies the system; no battery/charger or regulator is included. The schematic directory contains an honest connection netlist, not a captured KiCad schematic or PCB.

## Firmware
Pico SDK 2.x project; build on a host with `PICO_SDK_PATH` set and a supported ARM GCC toolchain:

```sh
cmake -S firmware -B firmware/build -DPICO_BOARD=pico
cmake --build firmware/build
```

Firmware emits one CSV row per second during a 60-second example run. The initial unloaded reading is the tare; set `COUNTS_PER_GRAM` in `main.c` using a known calibration mass and verify at multiple masses. Calibration is compile-time only and is not persisted. No radio, network, or remote update code is present.

## Assembly and use
See [assembly guide](docs/ASSEMBLY.md) and [serial protocol](docs/API.md). Zero the unloaded cup, verify with known weights, direct one outlet into the cup for a timed run, and repeat per outlet. Compare mass rates; water density is close to 1 g/mL near room temperature but this device reports grams, not volume.

## Verification status
Reference design only; no physical prototype tested. Firmware source and helper were syntax-checked where toolchain availability permits. Pin choices are ordinary RP2040 GPIO, not flash pins; electrical compatibility of a particular HX711 module must be confirmed by the builder. KiCad capture, PCB layout, calibration characterization, and SDK-target build are not claimed as completed.