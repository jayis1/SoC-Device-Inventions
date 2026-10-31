# Thread Gauge

Design status: Concept — firmware core host-compiled; hardware unbuilt and uncalibrated.

Thread Gauge is an offline comparator for hand-spinners, weaving studios, and textile classrooms that need to compare yarn linear density without a precision laboratory balance. A measured 100 m length is weighed on a low-force load cell; firmware reports tex (g/km) and denier (g/9 km), preserving raw mass and length so users can compare lots. It does not claim standardized conditioned-atmosphere testing or replace a calibrated textile laboratory instrument.

## Features
- RP2040 / Raspberry Pi Pico controller, USB-powered and offline.
- 100 m specimen path on a smooth, low-friction measuring wheel; manual winding, no motor.
- 5 g load-cell + HX711 bridge front end; buttons set tare, length and sample record.
- USB CDC CSV output; firmware core has no network, credentials, or automatic flash behavior.
- Design status: Concept. No PCB or validated KiCad schematic is supplied; see the electrical netlist and pin map in `schematic/`.

## Block diagram
```text
100 m yarn -> low-friction path -> 5 g load cell -> HX711 (DOUT/SCK) -> Pico
                                                        buttons -> Pico
                                                        Pico USB CDC -> host CSV
```

## SoC choice
The RP2040 on the Raspberry Pi Pico is inexpensive, widely available, has USB device support and ample timers for button sampling and robust serial output. The reference uses the complete Pico module (including its flash and clock), not a bare RP2040; GPIO assignments avoid the module's QSPI pins. This deliberately targets a manual, offline textile measurement rather than the repository's environmental sensors or general-purpose instrument designs.

## Pin assignments
See `schematic/pinmap.csv`. GPIO 2/3 connect HX711 DOUT/SCK, GPIO 14/15 are tare/record buttons with internal pull-ups, and USB CDC uses the Pico's onboard USB connector. No GPIO is multiply assigned. All external signals are 3.3 V logic; never connect 5 V signals to GPIO.

## Power architecture
USB 5 V powers the Pico; its onboard regulator provides 3.3 V for HX711 and the bridge. No battery or charger. Estimated design targets, not measurements: Pico idle 20–35 mA from USB, active sampling 25–45 mA, peak below 60 mA; peripheral budget is approximate and depends on board and HX711 module. Use a USB source rated at least 100 mA. Power budget and runtime: not applicable to battery operation.

## BOM
See [`hardware/BOM.csv`](hardware/BOM.csv). Approximate prototype parts cost excluding tools, enclosure, shipping, and USB host: USD $12–25. Module breakouts and load-cell wiring vary; verify load-cell polarity and bridge ratings before assembly.

## Firmware architecture
`firmware/main.c` contains the platform-independent acquisition and conversion core; `firmware/CMakeLists.txt` builds a host verification executable, not RP2040 target firmware. The hardware adapter remains to be implemented. Conversion uses integer micrograms and millimetres: tex = mass_g × 1,000,000 / length_mm; denier = mass_g × 9,000,000 / length_mm. Build/test: `cmake -S firmware -B /tmp/thread-gauge-build && cmake --build /tmp/thread-gauge-build && ctest --test-dir /tmp/thread-gauge-build --output-on-failure`.

## Helper scripts
`scripts/thread_gauge.py --help` displays CSV conversion utility usage. The script validates positive sample lengths and mass values and can convert exported CSV to tex/denier; it does not access hardware. Python bytecode caches are excluded from the release.

## Assembly summary
Mount the Pico and HX711 module on an insulated carrier. Suspend the load-cell free end without side loading; attach a small cup/hook and ensure the yarn path does not touch the enclosure. Connect bridge excitation/output according to the load-cell datasheet and HX711 module labeling; confirm with a multimeter before powering. Use current-limited USB for first power-up. Calibrate with known masses and a measured yarn length before interpreting comparisons. Keep the force low; this is a non-certified bench aid, not a tensile-strength tester.

## Verification status and limitations
The conversion core is host compiled and unit tested; helper script syntax and `--help` are checked. No hardware simulation, KiCad ERC, target Pico SDK build, prototype, calibration, or physical testing has been performed. The electronics netlist is a design reference, not a fabrication-ready schematic. Manual specimen length, tension, humidity, and conditioning affect results. Not for regulated or safety-critical use.

## License
MIT. Authored and maintained by jayis1.
