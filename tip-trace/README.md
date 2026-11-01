# Tip Trace

Status: reference design; firmware core is portable C and syntax-checkable, but the assembly is not fabricated or calibrated.

Who: electronics repair technicians and makers who need to compare soldering-iron tip recovery after a joint.
Pain: a handheld thermometer gives isolated readings and cannot show whether a tip recovers consistently under repeated thermal loads.
Why not COTS: this is a low-cost, offline logger for comparative bench checks, not a costly calibrated station analyzer.

Tip Trace samples a K-type thermocouple probe mounted in a sacrificial soldering tip using a MAX31855KASA+ digital converter and RP2040 (Raspberry Pi Pico). It streams timestamped temperature readings over USB CDC; a host can record CSV. This is an external contact probe only: do not open or electrically modify an iron, and do not treat readings as calibration or safety certification.

## Functional block diagram

```text
K-type fine-wire junction -> MAX31855KASA+ (3.3 V SPI) -> Pico RP2040 -> USB CDC -> host CSV
                                      ^
                              local 100 nF bypass
```

## Power and limits

USB 5 V powers the Pico board; its onboard regulator provides 3.3 V for the converter. No battery, heater, mains interface, radio, or switched load. Estimated converter current is under 2 mA; Pico current depends on USB activity (budget 30 mA for this prototype). Use a current-limited, compliant USB supply. Thermal range and accuracy are limited by the MAX31855 variant and probe construction; observe the selected device datasheet limits. Probe wiring must be electrically isolated from the iron and workpiece; verify isolation with a meter before use. Never connect a probe to an energized heater circuit.

## Pin assignment

| Pico GPIO | Connection | Notes |
|---|---|---|
| GP18 | MAX31855 SCK | SPI0 SCK |
| GP16 | MAX31855 SDO | SPI0 RX; converter output only |
| GP17 | MAX31855 CS | active low |
| 3V3(OUT) | converter VCC | common 3.3 V rail |
| GND | converter GND | common ground |
| USB | host | power and CDC serial |

The MAX31855 is read-only; no MOSI connection. Confirm Pico board pin labels, not physical pad numbers, during assembly.

## BOM

See `hardware/BOM.csv`. The thermocouple extension wire must be K-type and polarity-consistent. The MAX31855KASA+ is a 3.3 V-compatible SOIC-8 variant; verify current datasheet/availability before ordering. The Pico is a development board, not a bare RP2040 design.

## Schematic and firmware

The checked connectivity/netlist is in `schematic/netlist.md`; it is intentionally not represented as a native KiCad schematic. Firmware in `firmware/src/main.c` demonstrates the sampling and conversion logic behind a small platform HAL. The RP2040 SDK adapter/build integration is not included, so it is not yet a complete Pico SDK application. The portable core can be syntax-checked with a host C compiler. No claim of target build or physical testing is made.

## Assembly and use

See `docs/assembly.md`. First validate supply and SPI wiring without connecting a hot probe. Use a thermocouple simulator or known-temperature bath for calibration; compare only after accounting for junction placement, contact pressure, and response lag. Logging is intended for relative recovery profiles, not metrology.

## Validation record

- Firmware core: host C syntax check (no RP2040 SDK build).
- Python host logger: syntax check and `--help` smoke test.
- Schematic: textual pin-to-net connectivity only; no KiCad/ERC run.
- Hardware: not assembled, calibrated, or tested.
