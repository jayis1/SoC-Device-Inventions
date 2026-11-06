# Fret Meter

Offline guitar-fretboard setup comparator for repeatable string-to-fret clearance checks. A small RP2040 board reads a low-force probe carriage and reports relative clearance in USB CDC; it is a measurement aid, not a luthiery-certified gauge. Invented by jayis1.

## Purpose and novelty

A player or repairer can compare action at several frets without estimating against a ruler. Unlike the repository's string-pitch fixture, this measures geometric clearance, not vibration or pitch. Unlike load/angle fixtures, the probe is a low-force displacement comparator. No string is electrically driven and there is no wireless link.

## Readiness

Reference design; hardware unbuilt and uncalibrated. The host-independent C core builds with CMake. The pin-level netlist is supplied for review; no native KiCad ERC or physical validation is claimed. Not a replacement for a calibrated feeler gauge. Expected useful resolution depends on probe mechanics and ADC calibration.

## Architecture

Raspberry Pi Pico (RP2040, 3.3 V GPIO) is USB powered. A 10 kΩ linear potentiometer attached to a spring-return probe carriage forms a 3.3 V divider; its wiper feeds GPIO26/ADC0 through 1 kΩ, with 100 nF to ground at the ADC. A normally-open pushbutton on GPIO14 to ground starts a reading (internal pull-up). USB CDC emits filtered raw ADC counts and calibrated relative millimetres. The probe contacts the underside of a string while the base rests on the fretboard; use a rounded, non-marring tip and minimal force.

### Pin assignment

| Pico pin | Signal | Connection |
|---|---|---|
| 36 / 3V3(OUT) | VREF | Potentiometer high end |
| 33 / AGND | GND | Pot low end, capacitor, button |
| 31 / GP26/ADC0 | PROBE_ADC | 1 kΩ series from pot wiper; 100 nF to GND |
| 19 / GP14 | SAMPLE_N | Button to GND, internal pull-up |
| USB | CDC + power | Host computer, regulated 5 V input |

Do not feed external voltage into GPIO or the 3V3 rail. GP26 is an ADC input; USB and SWD programming remain available. The Pico onboard regulator supplies the potentiometer. Approximate divider current is 0.33 mA. No battery, charger, RF, mains, or hazardous actuator is used.

## Build and use

Requires Raspberry Pi Pico SDK 2.1.1, CMake, Ninja, and ARM GNU toolchain supported by that SDK. From this directory: `cmake -S firmware -B build -DPICO_SDK_PATH=/path/to/pico-sdk -DPICO_BOARD=pico -G Ninja`, then `cmake --build build`. Flash `build/fret_meter.uf2` by holding BOOTSEL while connecting USB. Firmware prints raw ADC values; calibration and physical travel must be established before interpreting millimetres.

Place the tool on a stable fretboard, verify probe returns freely, then press SAMPLE. Repeat at the same marked string/fret positions. No automated adjustment is performed. Do not use a probe force that deflects the string.

## Calibration and limitations

The core uses a two-point affine calibration; calibrate at two known carriage positions using gauge blocks, save coefficients on the host, and verify against an independent feeler gauge. ADC conversion is inherently relative until calibrated. Mechanical backlash, probe angle, string deflection, temperature, fret wear, and operator placement dominate accuracy. No accuracy specification is claimed. Stop if the probe binds or marks the instrument.

## Files

- `schematic/netlist.md`: pin-level wiring reference (not a KiCad schematic)
- `hardware/BOM.csv`: parts and quantities
- `firmware/`: Pico SDK C application and portable measurement core
- `docs/ASSEMBLY.md`, `docs/API.md`: assembly and serial protocol
- `scripts/calibrate.py`: host-side two-point calibration helper

## License
MIT. Invented by jayis1.
