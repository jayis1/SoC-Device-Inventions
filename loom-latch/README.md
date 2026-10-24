# Loom Latch

A USB-powered RP2040 retrofit that detects loom shuttle strikes and converts each verified pick into a timestamped USB-serial event. Its distinctive mechanism is a paired optical gate: direction and pulse width distinguish a real shuttle pass from vibration or a hand crossing one beam. Unlike a commodity counter, the Pico can classify the two beam transitions locally without network access.

## Problem and users

Small hand-loom weavers need repeatable pick counts and fault evidence but often rely on manual tally marks that are lost during interruptions. Loom Latch is an observational accessory; it does not control or modify loom motion.

## Architecture

```text
Two 3.3 V slot photointerrupters -> RP2040 GPIO -> ordered-edge classifier
                                               -> USB CDC JSONL -> host logger
```

Use a Raspberry Pi Pico (RP2040). This reference design is USB-powered, low voltage, and unbuilt. Connect two open-collector optical sensors with pull-ups; verify sensor output polarity before assembly. The schematic directory contains the explicit connection netlist; it is not a KiCad project.

## Pin assignments

| Signal | Pico GPIO | Direction | Notes |
|---|---:|---|---|
| GATE_A | GP14 | input | Optical gate, external 4.7k pull-up to 3V3 |
| GATE_B | GP15 | input | Optical gate, external 4.7k pull-up to 3V3 |
| USB CDC | USB | bidirectional | Pico native USB; no external USB-UART |

Both GPIO inputs are 3.3 V only. Never connect 5 V sensor outputs. Avoid powering from USB and another source simultaneously.

## Power

USB 5 V powers the Pico; its onboard regulator supplies 3V3. Each sensor must draw at most 10 mA from 3V3 (design target); worst-case add-on load is 20 mA plus Pico consumption. No battery, charger, mains, actuator, or wireless radio is used. Typical current and runtime depend on the chosen Pico board and sensors and have not been measured.

## BOM

See [hardware/BOM.csv](hardware/BOM.csv). Estimated prototype cost: approximately US$10–18 excluding loom and host computer; verify local prices before purchase.

## Firmware

Portable C event classifier and Pico SDK adapter are in `firmware/`. Configure a Pico SDK build with `PICO_SDK_PATH`, then run `cmake -S firmware -B build` and `cmake --build build`; see `firmware/BUILD.md`. Host-side test: `cc -std=c11 -Wall -Wextra -Werror firmware/classifier.c firmware/test_classifier.c -o /tmp/loom-test && /tmp/loom-test`.

Serial output is newline-delimited JSON: `{"event":"pick","direction":"A>B","duration_us":1234}`. Events outside 300–500000 µs are rejected; only A→B or B→A edge order counts. No network, telemetry, or remote control.

## Assembly and limits

See [docs/assembly.md](docs/assembly.md). Mount the gates so the shuttle reliably interrupts both in sequence without obstructing its path. Begin at low loom speed, validate with a manual count, and stop if the mount can snag moving parts. This tool is not a safety interlock or production metrology instrument. Hardware operation is not validated; no board layout or fabrication files are supplied.

## Verification status

Design reference, not fabricated. Host classifier tests and script syntax are checked before publication; Pico SDK target build and physical timing performance require external hardware/toolchain verification. Optical sensor part selection is intentionally left generic until the beam geometry is measured.

## License

MIT, consistent with the repository.