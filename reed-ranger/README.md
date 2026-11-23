# Reed Ranger

Offline reed-switch event logger for comparing manually operated mechanical counters, latches, and small mechanisms. A magnet on the moving part actuates a normally-open reed switch; the RP2040 timestamps qualified transitions and streams JSONL over USB CDC. It is a measurement aid, not a safety monitor or calibrated instrument.

## Design distinction
Unlike the repository's rotating-device counters, Reed Ranger records both closure duration and open-to-open cycle intervals. This exposes slow return, missed releases, and timing variation in a dry-contact mechanism without a motor, radio, or permanent installation.

## Hardware
- Controller: Raspberry Pi Pico (RP2040 module), powered from USB 5 V; GPIO operates at 3.3 V.
- Sensor: normally-open, 2-wire reed switch between GP2 and GND; GP2 uses internal pull-up. Add 1 kΩ series resistor at the connector and 100 nF from GP2 to GND for transient filtering.
- Controls: GP14 run/pause button to GND, GP15 clear button to GND; internal pull-ups.
- All interfaces are low voltage. Do not connect to mains, powered circuits, or safety-critical mechanisms. Use a magnet to trigger the reed; avoid excessive force on glass reed capsules.
- Power budget: USB source rated at least 500 mA; Pico workload is supplied by USB, no external load or actuator. No battery.

The wiring schedule is `schematic/netlist.md`; it is an explicit wiring reference, not a native KiCad schematic. BOM is `hardware/BOM.csv`. No PCB is supplied, and this design has not been fabricated or physically tested.

## Firmware
C11/Pico SDK 2.1.1. Set `PICO_SDK_PATH`, then `cmake -S firmware -B build -DPICO_SDK_PATH="$PICO_SDK_PATH" && cmake --build build`; flash `build/reed_ranger.uf2` using BOOTSEL. Firmware outputs one JSON object per accepted closure, with `closed_ms` and `interval_ms` (null for first event). Pause suppresses output but preserves count; clear resets session count/timing. Contact bounce is suppressed with a 30 ms lockout. No flash persistence.

## Use and limitations
Attach the reed switch so the magnet passes once per operation. Start capture with GP14, operate the mechanism manually, then stop. Confirm one event per physical operation before collecting comparisons. The lockout limits accepted events to about 33/s. Reed orientation and magnet spacing are fixture-dependent. Timing is MCU monotonic time, not calibrated metrology. This project does not infer force, wear, or failure probability.

See `docs/ASSEMBLY.md`, `docs/API.md`, and `schematic/netlist.md`. Author: jayis1.
