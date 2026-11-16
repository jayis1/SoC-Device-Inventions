# Mate Trace

Mate Trace is a USB-powered comparative connector mating-force logger for hobby electronics benches. A hand-operated slide pulls a connector pair apart through a small load cell; each stroke records the peak force and travel-independent sample count over USB CDC. It helps compare connector samples, crimp batches, and housing revisions without modifying the test article electrically.

## Distinction and limits

This is a mechanical mating-force comparison fixture, not an electrical continuity tester or automated life-cycle machine. It differs from Label Peel (adhesive peel force), Key Travel (switch force/travel), and Torque Pebble (fastener tightening). It measures one-axis axial force; fixture friction, alignment, speed, and load-cell calibration affect readings. Do not use for product certification or safety-critical connector qualification.

## Architecture

```text
USB 5 V -> Raspberry Pi Pico (RP2040, SC0915)
                    ├─ GP2 / GP3 -> HX711 DOUT / SCK -> 20 N load cell
                    ├─ GP14 -> capture button to GND
                    ├─ GP15 -> status LED through 1 kΩ
                    └─ USB CDC -> CSV records
```

The Pico's 3.3 V rail powers the HX711 module and its logic. Use a module explicitly compatible with 3.3 V logic; never allow a 5 V DOUT signal to reach a Pico GPIO. HX711 breakout pin names and bridge wiring vary; confirm its documentation before connection.

## Pin assignment

| Pico signal | Function | Connection |
|---|---|---|
| GP2 | HX711 DOUT | DOUT, 3.3 V logic |
| GP3 | HX711 SCK | SCK |
| GP14 | Capture | Normally-open button to GND, internal pull-up |
| GP15 | Status | 1 kΩ series resistor, LED anode; cathode to GND |
| 3V3(OUT) | Sensor supply | HX711 module VCC and bridge excitation, if module supports it |
| GND | Common | HX711, button, LED return |
| USB | Power/data | Pico USB connector, host CDC |

GP2/GP3 are ordinary GPIO, not boot-strapping pins. Keep the load-cell cable short and twisted; route it away from USB and LED wiring. Bridge color codes vary by manufacturer: identify E+, E−, A+, A− from the load-cell datasheet rather than assuming colors.

## Power and mechanical design

Powered only from USB 5 V; the Pico supplies regulated 3.3 V. Allow 100 mA at 5 V (design allowance, not measured). No battery, charger, mains, radio, motor, or high-voltage circuit. Build a rigid hand-slide fixture from plate/printed parts, with the load cell in axial tension and a travel stop that prevents overload. Use a universal clamp or replaceable connector-specific jaws; do not use the connector shell as a lever. Keep fingers clear of pinch points. The 20 N cell is a design choice; verify its rated safe overload and fixture capacity before use.

## BOM

| Ref | Qty | Part | Notes |
|---|---:|---|---|
| U1 | 1 | Raspberry Pi Pico, SC0915 | RP2040 USB development board |
| U2 | 1 | HX711 module, 3.3 V logic-compatible | Bridge ADC; breakout variant must be checked |
| L1 | 1 | 20 N strain-gauge load cell | Axial tension/compression type; identify bridge leads from datasheet |
| SW1 | 1 | Momentary normally-open button | Capture force peak |
| D1 | 1 | Green LED | Status indicator |
| R1 | 1 | 1 kΩ resistor | LED current limiter |
| C1 | 1 | 100 nF ceramic | Local decoupling at HX711 supply |
| J1 | 1 | USB data cable | Pico supply and CDC |
| M1 | 1 | Hand-slide fixture, clamps, fasteners | Non-electrical mechanical assembly, sized for load cell |

## Firmware and data

`firmware/src/mate_trace.c` is a portable C measurement state core. Build and run its host tests with `cmake -S firmware -B /tmp/mate-trace-build && cmake --build /tmp/mate-trace-build && ctest --test-dir /tmp/mate-trace-build --output-on-failure`. It consumes signed HX711 samples supplied by a hardware adapter; this repository does not include a Pico SDK/HX711 target adapter, so it is not flashable firmware yet. The intended target stack is Raspberry Pi Pico SDK 2.1.1, but target integration and build have not been verified.

Each capture emits `run_id,peak_counts,sample_count`; counts are raw ADC counts, not newtons. Calibrate with known axial weights and record a calibration factor externally before displaying force. Zero before each run; saturating values are invalid. USB output must escape/error-check host-side parsing; no network, credentials, or persistent storage are used.

## Schematic and assembly

`schematic/mate-trace.kicad_sch` is a conceptual connectivity drawing; it is not fabrication-ready and has not passed KiCad ERC. Build from a Pico development board and a documented 3.3 V-compatible HX711 breakout using short connections. Verify 3.3 V supply and GPIO levels before connecting the Pico; check DOUT stays within 0–3.3 V. Fix the sensor body to the frame and the load end to the slide so the full connector force passes axially through the cell. Align connector halves, zero the unloaded sensor, and make slow manual strokes. Stop if the reading approaches the sensor/fixture rating.

## Verification status and safety

Host state-core tests and compilation are the available verification. No Pico target build, KiCad ERC, load-cell calibration, or physical test has been performed. This is a low-voltage, uncalibrated comparative reference design, not a certified force gauge. Inspect the fixture and load cell before each use; never exceed their ratings.

## License

MIT; authored and maintained by jayis1.
