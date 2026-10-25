# Flow Ledger

Status: design reference; firmware not target-built; hardware not fabricated or calibrated.

Flow Ledger is for a facilities technician or homeowner who needs to verify water use at a mechanical dial meter without opening a pipe or trusting a cloud-connected utility portal. It clamps an optical sensor over the meter's low-flow indicator, counts rotations locally, and exports timestamped pulse records over USB serial. Unlike a leak alarm such as Pipe Halo, its purpose is auditable usage measurement and offline meter verification. After flashing, turn the meter's test wheel by hand and confirm one JSONL record per revolution in the serial monitor.

## Architecture

RP2040 (Raspberry Pi Pico) samples a reflective optical interrupter at 1 kHz, rejects short pulses, accumulates revolutions, and emits newline-delimited JSON over USB CDC. Calibration (litres per revolution) is configured in firmware. The sensor head is mechanically adjustable; compatibility depends on the meter's indicator geometry and must be tested.

```text
meter test wheel -> IR LED + phototransistor -> Pico GPIO15
Pico GPIO25 status LED; USB CDC -> host capture/analysis script
USB 5 V -> Pico onboard regulator -> 3.3 V sensor/logic
```

## Pin assignments

| Pico pin | GPIO | Signal | Notes |
|---|---:|---|---|
| J1-1 | 3V3(OUT) | VCC | Sensor only; keep total external load under Pico limits |
| J1-3 | GND | GND | Common ground |
| J1-20 | GP15 | OPT_OUT | 3.3 V logic, external 10 kΩ pull-up |
| J1-16 | GP12 | IR_LED | 220 Ω series resistor, active high |
| J1-22 | GP17 | STATUS_LED | 1 kΩ series resistor |
| J1-36 | VSYS | USB input | Power from USB only; no battery circuit |
| J1-2 | GPIO0 | SWDIO | Debug header |
| J1-3 | GPIO1 | SWCLK | Debug header (shared physical ground pin varies by header; use Pico SWD pads instead) |

Note: RP2040 GPIO0/1 are available, but debug/programming should use the Pico's SWD pads and a correctly keyed adapter. The table's J1 header references are for 40-pin Pico; use the schematic connection list as authority. No GPIO is connected to utility wiring.

## Power

USB 5 V, typically 20–35 mA for Pico plus continuously driven IR LED (approx. (3.3 V−1.2 V)/220 Ω = 9.5 mA) and status LED when active (approx. 1.3 mA). Budget 60 mA worst-case for this low-duty prototype; no battery-life claim. Use a certified USB supply. The optical head is electrically isolated from plumbing.

## Bill of materials

See [BOM.csv](hardware/BOM.csv). Cost is an indicative small-quantity estimate, availability/pricing varies. The Raspberry Pi Pico board is the build target; the schematic describes its connections, not a custom carrier PCB.

## Firmware build and flash

Requires Raspberry Pi Pico SDK 2.1.x, CMake, Ninja or Make, and `arm-none-eabi-gcc`. Configure `PICO_SDK_PATH`, then `cmake -S firmware -B build -DPICO_BOARD=pico -DPICO_SDK_VERSION=2.1.1`, build with `cmake --build build`, and copy `build/flow_ledger.uf2` to BOOTSEL drive. Default calibration is 1.0 litre/revolution and may be changed in `main.c`; calibrate against a known volume before interpreting records. Firmware currently emits revolution count and monotonic milliseconds, not calibrated billing-grade volume.

## Host capture

Connect to the Pico USB CDC serial port and run `python3 scripts/capture.py --port /dev/ttyACM0 --output readings.jsonl`. Each input line is bounded and validated; the script never scans or opens a port without an explicit `--port`.

## Safety and limitations

This is a low-voltage prototype for non-invasive observation. Do not install where the assembly could obstruct meter access, violate utility rules, or interfere with a meter seal. Do not use records for billing or leak-safety decisions without independent calibration and validation. Optical reflectance varies by meter and ambient light; enclosure geometry and threshold calibration are application-specific. There is no wireless radio, cloud service, or mains connection. No PCB layout is supplied; assemble on a Pico and perfboard. Design has not been physically tested.

## License

MIT, consistent with the repository license. Invented and maintained by jayis1.
