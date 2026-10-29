# Twist Meter

A low-voltage bench instrument for hand spinners and yarn makers: count turns per metre (TPI/TM) in a short yarn sample and show whether twist insertion is stable across repeated samples. Existing yarn evaluation is usually manual; this fixture makes repeatable twist measurement accessible without connecting to a motorized spinning machine.

This is a reference prototype, not a calibrated textile laboratory instrument. It uses a manually operated twist-restraint clamp and a measured 100 mm gauge length. The MCU counts encoder turns while the operator inserts twist; a second optical gate confirms the yarn is present. No powered actuator touches the yarn.

## Differentiation

Unlike Spool Scout (filament feed interruption) and Loom Mark (shuttle crossings), Twist Meter measures twist density in a manually prepared yarn specimen. It uses a rotary encoder and optical presence gate rather than spool motion or loom event sensing.

## Block diagram

```text
Yarn specimen ── optical gate (GP14) ─┐
Hand twist knob ─ rotary encoder (GP2/3) ├─ RP2040 Pico ─ USB CDC result
Zero button (GP15) ─────────────────────┘
```

## Pin assignments

| Pico GPIO | Signal | Notes |
|---|---|---|
| GP2 | Encoder A | Pull-up enabled; switch contacts to GND |
| GP3 | Encoder B | Pull-up enabled; switch contacts to GND |
| GP14 | Yarn-present photointerrupter output | 3.3 V logic only; open collector with 10 kΩ pull-up |
| GP15 | Reset/zero button | Active low; pull-up enabled |
| GP25 | Onboard LED | Status only |
| GND, 3V3 | Sensor supply | Do not apply external voltage to Pico 3V3 rail |

Use a Raspberry Pi Pico board (RP2040, 3.3 V GPIO); pin numbers above are GPIO numbers, not connector positions. USB supplies the board. Do not connect 5 V sensor outputs directly to GPIO.

## Measurement procedure and calculation

1. Clamp a straight 100 mm yarn sample between the fixed anchor and the freely rotating twist knob, without stretching it.
2. Press zero with the yarn gate clear. Insert one twist direction consistently; the firmware counts quadrature transitions.
3. Record the twist count when the sample reaches the chosen endpoint state. Repeat at least five times and report the median.
4. TPI = turns / measured length in inches; for the nominal 100 mm sample, inches = 100 / 25.4. For metric reporting, turns per metre = turns / 0.1.

The prototype reports raw encoder counts and calculated turns using  detents_per_turn=20 (one count per detent, configured for a 20-detent encoder). Validate that count convention on the selected encoder before interpreting results. Twist relaxation, fiber type, sample tension, and operator technique affect results; this is a comparative tool, not a standards-compliant test method.

## BOM (summary)

See `hardware/BOM.csv` for quantities, exact parts and cost estimates. Core parts: Raspberry Pi Pico (RP2040 module), 20-detent mechanical quadrature encoder, slotted photointerrupter, 10 kΩ pull-up, momentary zero button, and USB cable. Prices are illustrative estimates, not live quotations.

## Firmware

Pico SDK 2.1.1, CMake and arm-none-eabi toolchain. From `firmware/`: `cmake -S . -B build -DPICO_SDK_PATH=/path/to/pico-sdk`, `cmake --build build`. Copy `build/twist_meter.uf2` to the BOOTSEL-mounted Pico. USB CDC emits newline-delimited JSON records. No network or credentials are used.

## Safety and limitations

USB 5 V is confined to the Pico board; external sensor signals must remain within 0–3.3 V. Keep fingers clear of the yarn clamp and avoid excessive tension. No mains, battery, motor, or medical function. The result depends on fixture calibration, gauge length, specimen tension, encoder indexing, and operator technique; no accuracy claim is made.

## Verification status

Firmware source and project files are provided, but this run did not have the Pico SDK, ARM toolchain, or KiCad CLI installed; target compilation and ERC were not run. Hardware has not been fabricated or tested. BOM and pin definitions were manually cross-checked against the firmware assignments. This is not a fabrication-verified design.
