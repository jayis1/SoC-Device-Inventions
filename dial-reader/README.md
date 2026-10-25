# Dial Reader — clip-on optical analog-gauge reader

A low-voltage retrofit that estimates the angle of a high-contrast analog gauge pointer without opening the instrument or adding a pressure port. Intended for workshop technicians recording legacy pneumatic/regulator gauges. It is a reading aid, not a safety instrument; retain the original gauge and all rated protections.

## Use case and limits

A technician clips the sensor head over a compatible gauge face, aligns its center mark, and reads the pointer angle as a normalized fraction over a manually configured sweep. It supports static readings, not certified pressure measurement or alarm/shutdown control. Unlike camera-based readers, it uses a ring of 16 reflective optical channels and local USB CDC output; no images or network are captured. It requires a face/pointer contrast pattern that can be calibrated.

## Design

SoC: Raspberry Pi Pico board (RP2040) at 3.3 V. The RP2040 scans a 16-channel analog mux and selects the sensor with the lowest raw ADC reading (the design assumes the pointer reflects more IR than the dial); a fixed calibration maps that coarse sector to gauge units. Two momentary switches set zero and span; USB CDC emits newline-delimited JSON. No battery, radio, hazardous voltage, or gauge modification.

Block diagram:

`16× IR LED + reflective phototransistor channels -> 74HC4067 mux -> Pico ADC0 -> RP2040 -> USB CDC`

`ZERO/SPAN switches -> GPIO inputs; 3V3/GND -> sensor ring and mux`

### Pin assignments

| Pico GPIO | Signal | Connection |
|---|---|---|
| GP26 / ADC0 | MUX_SIG | 74HC4067 common analog output |
| GP2 | MUX_S0 | mux address bit 0 |
| GP3 | MUX_S1 | mux address bit 1 |
| GP4 | MUX_S2 | mux address bit 2 |
| GP5 | MUX_S3 | mux address bit 3 |
| GP6 | ZERO_BTN | switch to GND, internal pull-up |
| GP7 | SPAN_BTN | switch to GND, internal pull-up |
| GP0/GP1 | UART0 TX/RX | optional 3.3 V debug header; not USB CDC |

GP26 is a valid RP2040 ADC input. GPIOs are 3.3 V only; do not connect 5 V. Pico board supplies regulated 3V3. Each phototransistor is wired as a 3V3 pull-up (10 kΩ) and sensor-to-ground variable path; verify polarity and adjust resistor after prototype measurements. One mux, no I2C bus.

### Power and BOM

USB 5 V powers the Pico board; its onboard regulator supplies 3V3 to the mux, sensors, and pull-ups. Estimated electronics current: Pico ~20–40 mA plus 16 continuously lit IR LEDs at roughly 1–2 mA each and sensor current (estimate; measure on prototype), roughly 40–75 mA total. No battery-life claim.

See `hardware/BOM.csv`. Part numbers are manufacturer identifiers; optical response and face compatibility need bench validation. The optical ring and clip require a custom carrier, not included as a fabrication-ready PCB.

### Firmware and use

Build using Raspberry Pi Pico SDK 2.1.1:

`cmake -S firmware -B firmware/build -DPICO_SDK_PATH=/path/to/pico-sdk -DPICO_BOARD=pico`

`cmake --build firmware/build`

Flash `dial_reader.uf2` by holding BOOTSEL while connecting USB. At boot, output reports `{"status":"uncalibrated"}`. Position pointer at desired zero and press ZERO; position at full-scale and press SPAN. Subsequent records contain `angle_deg`, `fraction`, and `value`; set `FULL_SCALE` in `firmware/include/board_config.h` for the target scale. USB CDC is the user API; see `docs/api.md`.

## Status

Design reference, not physically validated. Firmware source and CMake project are provided; CMake configuration was attempted but could not proceed because the Pico SDK is unavailable here. Optical angle accuracy, ambient-light rejection, mechanical fit, and gauge-specific calibration have not been measured. No KiCad files are claimed; `schematic/netlist.md` is the connectivity-level design record. Never use this accessory as the sole basis for a safety-critical decision.

Designed and documented by jayis1.
