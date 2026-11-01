# Hinge Sag

USB-powered comparative sag and friction logger for cabinetmakers and furniture repairers. A small clamp fixture holds a door or lid at a repeatable opening angle; the operator releases it and the unit records angular settling and return motion. This helps compare hinge adjustments and identify binding without pretending to be a calibrated force or safety instrument. It differs from the repository's single-point hinge resistance fixture: this measures dynamic angle over time, not force versus angle.

## Design status
Reference prototype design; not fabricated or physically validated. The firmware algorithm and host build are verified; target GPIO/ADC acquisition is still an adapter boundary. No KiCad capture or PCB is supplied: `schematic/netlist.md` is the authoritative connection list. Not a structural or safety certification instrument.

## Architecture
```text
USB 5 V -> Raspberry Pi Pico (RP2040) -> 3-axis accelerometer (I2C)
                                      -> OLED (I2C)
                                      -> USB CDC CSV
```
A rigid clamp fixture attaches the Pico enclosure to the moving panel. The accelerometer's X/Y gravity vector estimates tilt; angular change is sampled for a configurable observation window. The board is stationary with the panel; no external wiring or mains connection is involved.

## Connections
Use Raspberry Pi Pico module, powered only by USB. I2C0 SDA=GP4 (physical 6), SCL=GP5 (7), shared by LIS3DH (U2, address 0x18; SA0 low) and SSD1306 128x32 OLED (U3, address 0x3C). 4.7 kΩ pull-ups from SDA/SCL to 3V3; both peripherals are 3.3 V. GP15 (pin 20) is a normally-open capture button to GND with internal pull-up. GP25 (onboard LED) is status. Connect Pico 3V3(OUT) to sensors, GND common. Do not apply external voltage to GPIO. Pico module provides USB, regulator, RP2040 and flash; no bare-chip layout is implied.

## BOM
See `hardware/BOM.csv`. LIS3DH breakout must be a 3.3 V I2C-compatible board with pull-ups disabled or compatible with the listed 4.7 kΩ pair. OLED is a 3.3 V I2C module. Module-level BOM is a prototype selection, not a verified sourcing quote.

## Power
USB 5 V powers the Pico; its onboard 3.3 V rail supplies sensors. No battery or charging circuit. Estimated sensor current <10 mA plus Pico; actual USB current depends on module and OLED brightness. No quantified runtime claim.

## Firmware
Host core uses C11 and CMake:
`cmake -S firmware -B /tmp/hinge-sag-build -DBUILD_TESTING=ON`
`cmake --build /tmp/hinge-sag-build`
`ctest --test-dir /tmp/hinge-sag-build --output-on-failure`
The pure math/state core builds and tests with host GCC. This is not an RP2040 target build: Pico SDK/toolchain and hardware I2C/display acquisition are not included. A target adapter must connect sensor samples to the core and implement USB CDC output.

## Use and output
Mount rigidly, place panel near 20–70° from horizontal, press capture, then release without pushing. Record stable start and subsequent samples. CSV fields: `ms,angle_mdeg,delta_mdeg`; readings are comparative only. `scripts/analyze.py FILE.csv` reports elapsed time, peak absolute excursion, and final offset from first sample. It rejects malformed/non-finite inputs.

## Limitations and safety
Accelerometer tilt is sensitive to acceleration during release; wait for settling and repeat trials. Friction, hinge geometry, panel flex, clamp slip, and surface alignment affect results. No accuracy, repeatability, calibration, or physical test is claimed. Use a separate mechanical support when a heavy door could fall; never rely on this logger to hold a panel.

## License
MIT; authored by jayis1.
