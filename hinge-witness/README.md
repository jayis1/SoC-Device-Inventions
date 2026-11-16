# Hinge Witness

Hinge Witness is a USB-powered comparative cabinet-door hinge alignment logger. It measures the angle of a door at two repeatable mechanical positions using a low-cost magnetic angle sensor and reports the readings over USB CDC, helping a cabinetmaker compare sag before and after adjustment. It is a measurement aid, not an automatic adjustment system.

## Distinction

Unlike Hinge Cycle, which records motion events from an AS5600, Hinge Witness samples two static, manually marked positions to compare alignment. Unlike Hinge Meter, it does not measure opening force or use a load cell. It uses a single magnetic angle sensor and a simple reference fixture.

## Architecture

```text
USB 5 V -> Raspberry Pi Pico module -> RP2040
                                  ├─ I2C0 -> AS5600 angle sensor
                                  ├─ GP14 -> capture button
                                  ├─ GP15 -> status LED
                                  └─ USB CDC -> timestamped CSV
```

## Pin assignment

| Pico signal | Function | Connection |
|---|---|---|
| GP4 | I2C0 SDA | AS5600 SDA, 4.7 kΩ pull-up to 3V3 |
| GP5 | I2C0 SCL | AS5600 SCL, 4.7 kΩ pull-up to 3V3 |
| GP14 | Capture button | Button to GND, internal pull-up |
| GP15 | Status LED | 1 kΩ series resistor to LED, LED cathode GND |
| 3V3(OUT) | Sensor supply | AS5600 module VCC; module must be 3.3 V compatible |
| GND | Common | Sensor and button return |

Use a breakout explicitly rated for 3.3 V logic; do not use a breakout with 5 V pull-ups. The AS5600 I2C address is 0x36. The sensor's magnet must be centered over its package; fixture geometry and calibration determine accuracy.

## Power and limits

USB 5 V powers a Raspberry Pi Pico (SC0915); its onboard regulator provides 3.3 V. No battery, charger, mains connection, motor, or radio is used. Allow 100 mA at 5 V for the module and sensor. This is a design allowance, not a measured consumption figure. Sensor readings are relative; this reference design does not claim calibrated absolute-angle accuracy or characterize magnetic/fixture error.

## BOM

| Ref | Qty | Part | Notes |
|---|---:|---|---|
| U1 | 1 | Raspberry Pi Pico, SC0915 | RP2040 module, USB-powered |
| U2 | 1 | AS5600 magnetic angle sensor module | I2C, 3.3 V logic, verify breakout pull-ups |
| SW1 | 1 | Momentary normally-open pushbutton | Capture position |
| D1 | 1 | Green LED | Status |
| R1 | 1 | 1 kΩ resistor | LED current limit |
| R2-R3 | 2 | 4.7 kΩ resistors | I2C pull-ups if not populated on module |
| C1 | 1 | 100 nF ceramic capacitor | Local sensor decoupling |
| J1 | 1 | USB cable | Pico power and serial data |

## Firmware

`firmware/src/hinge_witness.c` is a portable C capture-state core. Build the host object with `cc -std=c11 -Wall -Wextra -Werror -c firmware/src/hinge_witness.c -o /tmp/hinge_witness.o`. It is not a complete Pico SDK application: GPIO/I2C/USB CDC bindings and a target build configuration remain to be added. Do not flash this core directly.

A capture is recorded as raw angle counts with a label (`BEFORE` or `AFTER`); host tooling can compare paired captures. Firmware-side validation should reject sensor errors and stale samples rather than treating them as valid angles.

## Schematic and assembly

`schematic/netlist.md` is the connectivity reference for this prototype; it is not a native KiCad schematic and is not fabrication-ready. Connect the AS5600 module over short I2C wires, common the grounds, and keep 5 V away from sensor GPIO. Attach the sensor and diametrically magnet to a rigid printed or machined bracket so the magnet follows the door's rotation. Mark one repeatable closed-door reference position. Before use, verify 3.3 V at the sensor supply and confirm the reported angle changes smoothly through the intended travel.

## Data format

USB CDC output is intended as CSV: `label,angle_raw,angle_degrees`. The reference firmware core only stores labeled raw counts; USB serialization is an integration task. No network or persistent storage is included.

## Verification status and safety

This is an unbuilt, uncalibrated reference design. Host C compilation verifies syntax only; no target firmware build, KiCad ERC, bench test, or calibration was performed. It is not a precision metrology instrument. Use only on non-powered cabinet furniture; do not use to assess fire doors, machine guards, or other safety-rated assemblies.

## License

MIT; authored and maintained by jayis1.
