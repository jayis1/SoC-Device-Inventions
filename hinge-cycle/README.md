# Hinge Cycle

Offline hinge-life and alignment logger for small cabinet, case, and enclosure hinges. It counts open/close cycles and records the angle at which motion starts and stops, helping compare hinges before and after lubrication or adjustment. It is a comparative workshop instrument, not a certified durability tester.

## Design

A Raspberry Pi Pico (RP2040) samples a low-cost AS5600 magnetic angle sensor over I²C. A small diametrically magnetized disk on a temporary clip follows the hinge leaf; the sensor board clips to the stationary leaf. USB supplies the device and provides CDC serial output. A tactile button starts/stops a run; an LED indicates recording. No battery, radio, mains connection, or actuator is used.

The magnet position is calibrated against the closed position. Firmware reports relative angle in degrees (0–359.9°), opening/closing direction, cycle count, and maximum observed travel. Sensor resolution is not the same as fixture accuracy: alignment, magnet gap, hinge play, and missed slow transitions dominate. Use readings for relative comparison only.

## Pin assignments

| Pico signal | GPIO | Connection |
|---|---:|---|
| I2C0 SDA | GP4 | AS5600 SDA, 4.7 kΩ pull-up to 3V3 |
| I2C0 SCL | GP5 | AS5600 SCL, 4.7 kΩ pull-up to 3V3 |
| Run button | GP14 | Button to GND, internal pull-up |
| Status LED | GP15 | 1 kΩ series resistor to LED, then GND |
| Power | 3V3(OUT) | AS5600 breakout VCC (3.3 V only) |
| Ground | GND | Sensor and button return |

AS5600 default 7-bit I²C address is 0x36. Use a breakout without a 5 V pull-up; never connect 5 V logic to Pico GPIO. USB provides 5 V to the Pico board regulator; the sensor runs from regulated 3.3 V.

## Build and use

Firmware core is portable C11 and host-testable. Target hardware adapter/build is not included in this reference design; compile `firmware/src/hinge_cycle.c` with a C11 compiler and use the documented event API. Connect a host-side adapter to poll the AS5600 and pass readings to the core. The design has not been target-built, fabricated, or electrically validated.

1. Clip the sensor and magnet on opposite hinge leaves with the magnet centered over the sensor.
2. Connect Pico by USB and open its CDC serial console at 115200 baud.
3. Press the button to start a session; move the hinge through its normal range several times; press again to stop.
4. Save the emitted CSV lines and compare identical fixture placements.

## Contents and maturity

- `schematic/netlist.md`: connection-level reference schematic and wiring checks.
- `hardware/BOM.csv`: parts and sourcing/footprint notes.
- `firmware/`: portable event-processing core and host test.
- `docs/ASSEMBLY.md`, `docs/API.md`: fixture and interface details.

Reference design only. No KiCad-native schematic, PCB layout, calibration certification, or physical test is supplied. The operator must validate polarity, mounting, and sensor range before relying on measurements.

## Novelty

Unlike the repository's latch cycle counter, this device measures hinge angular travel and run-to-run start/stop variation rather than merely recording switch-open dwell. It is also unlike the bicycle spoke-frequency and hinge-sag designs: it observes a rotational joint directly with a magnetic encoder.
