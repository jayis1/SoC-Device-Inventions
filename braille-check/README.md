# Braille Check

A USB-powered tactile-sign quality comparator for volunteer sign makers and accessibility auditors. A guided probe carriage samples the height of six embossed dots against a nearby reference surface, and reports the measured dot-to-base displacement and row/column spacing over USB CDC. It helps catch visibly flat or uneven dots before a label is installed; unlike a Braille embosser, this is a small inspection fixture, not a text production machine.

## Design status

Reference prototype; not fabricated, calibrated, or physically validated. The C measurement core and host tests are verified with GCC. `schematic/netlist.md` is the authoritative connection plan; no KiCad capture or PCB layout is supplied. The load-cell mechanical fixture and target ADC acquisition remain integration work. Not an accessibility compliance certification instrument.

## Architecture

```text
USB 5 V -> Raspberry Pi Pico (RP2040) -> HX711 + 1 kg load cell probe carriage
                                      -> 2 tactile switches (X/Y index)
                                      -> USB CDC measurement CSV
```

A spring-loaded probe foot travels over a 2×3 Braille cell. At each position, the operator records unloaded base and dot-top readings using the same probe. The firmware reports differential height; the load cell only senses probe force trend and cannot determine geometry by itself. The reference fixture uses a dial indicator or mechanical height gauge as the actual displacement input during initial prototype evaluation. The firmware core accepts calibrated micrometre samples from a target adapter; HX711 acquisition is deliberately not represented as a finished height sensor.

## Connections and pin assignment

Raspberry Pi Pico module, USB powered. HX711 DOUT=GP16, SCK=GP17; X index switch=GP14 to GND; Y index switch=GP15 to GND; onboard LED=GP25. USB CDC uses the Pico USB connector. 3V3(OUT) and GND are available for the HX711 logic only. Do not connect load-cell excitation to Pico GPIO; use the HX711 module's E+/E-/A+/A- terminals. Confirm the selected HX711 breakout's logic supply and pin labels before wiring. GP14/15 use internal pull-ups. No wireless or network service.

## Power

USB 5 V powers the Pico; 3.3 V powers HX711 logic. No battery, charger, or mains circuitry. Estimated load-cell bridge excitation is supplied by the HX711 breakout; its current and the board's actual USB draw depend on the selected module. No runtime or accuracy claim is made.

## Build and test

```sh
cmake -S firmware -B build -DBUILD_TESTING=ON
cmake --build build
ctest --test-dir build --output-on-failure
```

This is a host build of the portable C11 measurement core, not an RP2040 firmware image. Pico SDK target support and physical ADC/scale calibration are future integration work.

## Use and protocol

The portable core accepts six paired base/dot readings in micrometres and computes each dot's differential height plus min/max/mean. The planned target protocol emits CSV columns `dot,base_um,top_um,height_um`; invalid or incomplete cells must not be reported as pass/fail. Use `scripts/check_csv.py FILE.csv` to validate exported numeric records. Values depend on fixture calibration and probe force; compare specimens under identical conditions.

## BOM and assembly

See `hardware/BOM.csv` and `schematic/netlist.md`. Assembly notes are in `docs/assembly.md`; protocol and core API are in `docs/api.md`. The mechanical probe guide is a prototype fixture, not a metrology-grade displacement sensor. Establish the travel zero and calibrate against gauge blocks before interpreting measurements.

## Limitations and safety

This design does not establish compliance with any Braille standard, nor does it assess readability, spacing standards, tactile contrast, or installation quality. Do not use its results as a substitute for accessibility review. Avoid excessive probe force that could deform paper or soft labels. Prototype force/mechanical accuracy and electrical behavior remain unverified.

## License

MIT; authored by jayis1.
