# Label Peel

USB-powered comparative peel-force logger for pressure-sensitive labels. A small spring carriage draws a label at a repeatable hand-set angle while a 5 kg bar load cell records force through an HX711. It exports raw force samples and peak force over USB CDC CSV so packaging and craft users can compare adhesive batches, dwell times, and substrates without a laboratory tensile tester.

Status: reference prototype; firmware has not been target-built, the force channel is uncalibrated, and no hardware has been assembled. Values are comparative only, not ASTM D3330 compliance or a certified peel-strength measurement. Do not use for product release or safety-critical adhesives.

## Design

A Raspberry Pi Pico (RP2040, 3.3 V GPIO) reads an HX711 load-cell ADC over two GPIOs, reports USB CDC records, and samples a normally-open foot switch. The load cell is mechanically installed in a low-force 90-degree peel fixture with a spring-loaded moving stage; no motor, radio, mains connection, or battery is present. USB powers the Pico; its 3V3 output powers HX711 logic. Use a load-cell excitation supply compatible with the specific bridge and HX711 module; this reference uses the module's 3V3 excitation, not 5 V. Confirm the module's pinout before wiring.

Block diagram: USB host ↔ Pico ↔ HX711 ↔ 5 kg strain-gauge bar load cell; Pico GP14 ← tare/record foot switch. Mechanical fixture provides hand-driven constant-speed carriage and 90° peel geometry.

## Pin assignment

| Signal | Pico pin | Physical pin | Connection |
|---|---:|---:|---|
| HX_DOUT | GP2 | 4 | HX711 DOUT |
| HX_SCK | GP3 | 5 | HX711 SCK; held low except clocking |
| FOOT | GP14 | 19 | switch to GND; internal pull-up |
| 3V3 | — | 36 | HX711 VCC/bridge excitation only if module supports it |
| GND | — | 38 | common logic/load-cell return |
| USB | — | 1/2/3 | host data and regulated board power |

Load-cell wires must be mapped from its datasheet (typically E+/E− excitation, A+/A− signal); wire colors are not standardized. Do not connect the bridge to GPIO. Keep load-cell signal wiring twisted and away from USB cable noise.

## Power and safety

USB 5 V powers the Pico's onboard regulator. A nominal 3.3 V rail feeds HX711 logic and bridge excitation; bridge current depends on the actual load-cell resistance and is not characterized here. Use only a Pico-compatible HX711 module with 3.3 V logic and verify its excitation pins before connecting. The fixture must have a mechanical travel stop and finger clearance. The indicated force includes fixture friction and angle error. No PCB layout is supplied.

## Build and use

1. Fit the load cell between a rigid frame and moving peel carriage with the load axis aligned; avoid side loading. Mount a label backing clamp and substrate table at 90 degrees.
2. With USB disconnected, verify module pin labels, continuity, no 5 V path to Pico GPIO, and correct bridge polarity from the sensor datasheet.
3. Connect as in the table; attach the switch between GP14 and ground.
4. Build firmware with Raspberry Pi Pico SDK 2.1.1 and CMake. Connect USB; the program emits CSV header `ms,raw,peak_raw,event`.
5. Run an empty-cycle check, then compare like samples at matched dwell, substrate, peel width, and hand speed. Establish calibration with certified weights before converting raw counts to force. The firmware intentionally reports raw counts, not invented newtons.

## BOM

See [hardware/BOM.csv](hardware/BOM.csv). Mechanical fixture, clamps, and fasteners are user-fabricated and not included in the electronics BOM.

## Validation status

Host-side core syntax checks and structural checks are the only intended software checks unless a Pico SDK toolchain is installed. Target build, KiCad ERC, calibration, and physical testing remain outstanding. Hardware, force accuracy, peel-rate repeatability, and standards conformance are unverified. Treat the apparatus as a comparative experiment only.
