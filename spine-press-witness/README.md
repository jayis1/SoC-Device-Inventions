# Spine Press Witness

A low-voltage press-force logger for small-book binders and conservators. It records load-cell readings during a clamping session so users can compare repeatability between otherwise manual book-spine pressing operations. It is a process aid, not a calibrated force instrument.

## Product definition

- User: small-batch bookbinder or conservation workshop using a hand-operated screw press.
- Problem: applied clamp force and dwell time are difficult to reproduce and document.
- Capability: USB-powered, single-point force trace with tare, timestamped CSV over USB CDC.
- Acceptance: boots without a sensor; reports `sensor_error` rather than hanging; tare zeroes a stable unloaded platform; data rows are parseable CSV and can be captured by the included script.
- Not intended to control a press, certify binding strength, measure force on a person, or operate unattended.

## Architecture

```text
5 V USB -> Raspberry Pi Pico (3V3 logic) -> HX711 -> 5 kg strain-gauge load cell
                                      \-> USB CDC CSV -> host capture script
```

Pico SDK 2.x / RP2040. HX711 module uses its 2.7–5.5 V supply range; this design powers it from 3V3, keeping DOUT and PD_SCK in the Pico's 3.3 V domain. Load-cell excitation is likewise 3V3. Use a module with accessible E+/E-/A+/A- connections. The bridge sensor's excitation and signal wiring must follow its datasheet.

## Pin assignments

| Signal | Pico GPIO | Physical pin | Notes |
|---|---:|---:|---|
| HX711 DOUT | GP14 | 19 | Input, 3V3 module output |
| HX711 PD_SCK | GP15 | 20 | Output; kept low except clock pulses |
| USB CDC | USB | — | Native Pico USB; no external UART |
| 3V3 | — | 36 | HX711 VCC and load-cell excitation |
| GND | — | 18 | Common ground |

No RP2040 boot/debug/flash pins are repurposed. No I2C devices or address conflicts.

## Power and operating limits

USB 5 V powers the Pico board; the Pico's onboard regulator supplies 3V3. The HX711 and bridge are powered from that 3V3 rail. Expected board plus converter current is below 30 mA typical (design estimate, not measured); verify the selected Pico board regulator and USB source. No battery, radio, heater, motor, or external high-voltage load. Do not connect 5 V logic to GPIO.

## BOM

See `hardware/BOM.csv`. Pico board and HX711 module are orderable module-level items; supplier-specific module construction varies. Check HX711 module output-high voltage while powered at 3V3 before connecting. The load cell capacity is nominal and must be mechanically installed so press force is transmitted axially without bending or overload.

## Firmware and use

Build instructions: `firmware/build_notes.md`. Run the firmware, open USB CDC at 115200 baud (baud is ignored by native USB), and send `t` plus newline with the press unloaded to tare. Each valid sample emits `ms,raw,relative_counts`; `sensor_error` indicates a timeout. The raw scale factor is intentionally not presented as newtons: calibrate with known masses and a suitable fixture before interpreting values. Sample rate is about 10 Hz.

Host capture: `python3 scripts/capture.py /dev/ttyACM0 output.csv`. On Windows, provide the COM port. The script uses only Python's standard library.

## Schematic status

`schematic/netlist.md` is the connection-level schematic source of truth. Native KiCad symbol/schematic files and PCB layout are not included; do not treat this package as a fabrication-ready board.

## Verification status

Firmware is written for Pico SDK 2.x. In this run no Pico SDK, ARM cross-compiler, or KiCad CLI was available, so target build and ERC were not run. Python syntax and CSV parsing are checked; electrical operation, force calibration, mechanical behavior, and physical hardware remain unverified. Design intent only; not a calibrated measurement device.
