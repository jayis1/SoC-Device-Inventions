# Platter Meter

Offline turntable speed and rotational-stability measurement reference design. A removable reflective marker on the platter passes an optical sensor; the RP2040 timestamps each revolution and reports mean RPM and revolution-interval coefficient of variation over USB CDC. It does not contact the spindle, connect to audio wiring, or require a phone/network.

Status: schematic-stage prototype reference, not fabricated, calibrated, or physically validated. The optical method measures average revolution timing and does not claim standardized wow/flutter compliance. Ambient light, marker placement, platter eccentricity, and spindle speed variation affect results.

## Behavior
- 1 pulse/revolution; 10–60 RPM operating range.
- USB CDC outputs one JSON line per completed 8-revolution window: `{"rpm":33.333,"interval_cv_pct":0.12,"samples":8}`.
- Rejects intervals outside 0.5–7 seconds; startup and invalid intervals are not reported as measurements.
- USB-powered 5 V; RP2040 board regulates to 3.3 V. Sensor output is open-collector pulled up to 3.3 V.

## Build / bring-up
Use Raspberry Pi Pico (RP2040), Raspberry Pi Pico SDK 2.1.1, CMake 3.20+, and ARM GNU toolchain. Configure with `cmake -S firmware -B build -DPICO_SDK_PATH=/path/to/pico-sdk`, then `cmake --build build`. Flash `platter_meter.uf2` by holding BOOTSEL while connecting USB. Host tests: `cc -std=c11 -Wall -Wextra -Werror -DPLATTER_HOST_TEST firmware/src/platter_meter.c firmware/test/test_core.c -lm -o /tmp/platter-test && /tmp/platter-test`.

## Wiring
Use the RP2040 Pico board's GP2 as sensor input and GP25 as status LED. Sensor board is a TCRT5000 reflective sensor module operated at 3.3 V (or raw phototransistor circuit below); verify the module output is never above 3.3 V. GND common. USB supplies board power. Place a matte white 10×10 mm marker on the platter and sensor 2–5 mm above it. Avoid connecting to mains or turntable audio circuitry.

## Power
USB 5 V powers Pico; its onboard regulator supplies 3.3 V. Sensor module current target <10 mA. No battery, charger, motor driver, or high-voltage circuitry. The Pico's onboard USB protection/regulation is relied upon; this design adds no external power input.

## Pin assignments
| Signal | Pico pin | RP2040 GPIO | Notes |
|---|---:|---:|---|
| Sensor OUT | 4 | GP2 | Falling edge; 10 kΩ pull-up to 3V3 |
| Sensor VCC | 36 | 3V3(OUT) | Sensor module only |
| Ground | 38 | GND | Common reference |
| Status LED | onboard | GP25 | SDK board LED |
| USB CDC | USB | — | JSONL output |

## Contents
- `schematic/netlist.md`: connection-level schematic definition.
- `hardware/BOM.csv`: parts and references.
- `firmware/`: Pico SDK application and portable timing/statistics core with host test.
- `docs/ASSEMBLY.md`, `docs/API.md`: assembly and protocol.

## Limitations
No optical geometry, RPM accuracy, or interval-CV accuracy has been established on hardware. This is a comparative diagnostic aid, not a certified audio measurement instrument. Validate against a traceable tachometer before relying on absolute results. The host test checks arithmetic and interval rejection only; target firmware build and physical test remain outstanding.
