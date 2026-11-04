# Leaf Ledger

Offline leaf-area tracing digitizer for comparative botany, crop scouting and classroom experiments. Trace the outline of a detached leaf on a flat digitizing surface with a low-force stylus; a two-axis quadrature cursor records a closed polygon and emits its signed-grid area over USB CDC. Unlike the repository's environmental probes, this measures specimen geometry directly without optics, image processing, batteries, radio, or biological/diagnostic claims.

Status: reference prototype. Firmware source and design files are provided; no target build, electrical-rule check, calibration, fabrication, or bench test has been performed. Measurements are encoder-count squared until a ruler calibration establishes counts/mm. Do not treat the output as validated botanical measurement.

## Architecture

RP2040 Raspberry Pi Pico module, USB-powered at 5 V through the module's onboard regulator. Cursor has two mechanical quadrature encoders (X and Y), each powered from 3V3 and read with internal pull-ups. A normally-open momentary button to ground starts/stops a trace. No external actuator or analog front end. Pico GPIO inputs are 3.3 V only; never connect 5 V logic. Host computer supplies USB; no battery or mains circuitry.

Block: USB host → Pico USB CDC / RP2040 → four encoder channels + trace button → shoelace polygon-area calculation → JSONL stdout.

| Function | Pico GPIO | Wiring |
|---|---:|---|
| X encoder A/B | GP2 / GP3 | 3V3, GND, A, B |
| Y encoder A/B | GP4 / GP5 | 3V3, GND, A, B |
| Trace button | GP6 | button to GND; internal pull-up |
| USB CDC | onboard USB | host connection / power |

Use a Pico-compatible module with accessible GPIO and onboard USB/regulator. Encoder A/B phase order affects direction only; swap A/B if desired. Cursor mechanics and encoder mounting must prevent backlash and provide repeatable orthogonal motion. Add 100 nF decoupling at each encoder connector if leads are long; inputs remain 3.3 V.

## Build and operation

Requires Raspberry Pi Pico SDK 2.1.1 and arm-none-eabi toolchain. From this directory:

    cmake -S firmware -B build -DPICO_SDK_PATH=/path/to/pico-sdk
    cmake --build build

Flash `build/leaf-ledger.uf2` by holding BOOTSEL while connecting the Pico, then copying the UF2 to RPI-RP2. Connect serial USB CDC at 115200 baud. Each button press toggles tracing. First press begins, subsequent encoder coordinates are captured; next press closes the polygon and prints `area_counts2`, the absolute shoelace area in encoder-count². Keep trace under 2048 recorded points; overflow aborts safely. A trace with fewer than three points has zero area. Calibrate each axis by moving a known ruler distance, recording counts, then compute `area_mm2 = area_counts2 / (x_counts_per_mm * y_counts_per_mm)` on the host.

## Files

- `schematic/leaf-ledger.net.csv`: explicit signal-to-pin wiring reference (not a KiCad schematic).
- `hardware/BOM.csv`: prototype parts.
- `firmware/`: Pico SDK C program and build files.
- `scripts/area.py`: JSONL host-side area conversion helper.
- `docs/assembly.md`: assembly and calibration notes.

## Limitations and safety

This is not fabrication-ready: no PCB layout, KiCad-native schematic, ERC, target compilation, or physical validation. Encoder resolution, mechanical orthogonality, stylus tracking, USB host behavior and measurement repeatability remain to be tested. Use only detached specimens and a blunt stylus; this is not a medical or crop-treatment instrument. Author: jayis1.
