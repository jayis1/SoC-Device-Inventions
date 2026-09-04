# Relay Finch Assembly Guide

## 1. Enclosure and Mechanical Layout

Use a two-board stack:

- **main measurement PCB** with STM32, shunt, INA240, power stages, microSD, and OLED
- **wireless mezzanine** carrying the ESP32-C3 and antenna keep-out

Recommended enclosure zones:

1. front panel: OLED, rotary encoder, start button, mode button
2. left edge: USB-C and barrel input
3. right edge: DUT coil connector, contact-sense connector, Hall probe jack
4. rear shell: Li-ion cell and piezo buzzer

Keep the high-current coil loop compact and away from the Hall probe connector.

## 2. High-Current Test Path

1. Place the 10 mΩ shunt close to the coil output return.
2. Kelvin-route the shunt to INA240.
3. Keep MOSFET, diode, and dump path traces short and wide.
4. Add thermal copper under the drive MOSFET.
5. Fit a replaceable fuse or resettable protector on external input power.

## 3. Piezo Click Sensor

- use a 20–27 mm piezo disc
- mount it on a small clip or magnetic puck that touches the DUT body
- shield the cable if testing inside noisy cabinets
- bias the amplifier at mid-rail and add a soft clamp to protect the ADC

## 4. Hall Travel Probe

The Hall probe can be onboard or remote.

### Remote probe option

- small daughterboard with TMAG5273
- JST-SH cable back to main board
- magnetic clip or adhesive target for moving armatures

### Calibration

1. energize the DUT slowly by manual voltage steps
2. record raw Hall counts at rest and full pull-in
3. store a two-point or three-point mapping in preset memory

## 5. Isolated Contact Sense Input

- support dry-contact closure or externally biased logic
- input divider and current limiter should tolerate at least 30 V field mistakes
- use the isolation barrier footprint exactly as specified by the package creepage rules
- never share DUT contact common directly with logic ground

## 6. Bring-Up Order

1. populate buck and 3.3 V regulator first
2. verify USB-C charging and battery fuel gauge
3. flash STM32 test firmware and verify UART console
4. verify ESP32-C3 bridge and BLE advertisement
5. test OLED and encoder
6. validate shunt amplifier with a resistive dummy load
7. validate programmable coil bus with no DUT attached
8. test isolated contact input using a signal generator or switch box
9. verify Hall probe and piezo channels
10. run the relay preset self-test on a sacrificial 12 V relay

## 7. Safety

- disconnect DUT load wiring; Relay Finch is for bench/status testing, not live switching under load
- do not use on unknown mains-powered AC contactor coils without a redesigned isolated front end
- discharge the coil bus before changing leads
- fit reverse-polarity and TVS protection on field connectors
- keep fingers off exposed DUT terminals during boosted-voltage tests

## 8. Suggested Validation Matrix

| Test | Goal |
|------|------|
| dummy resistor sweep | verify current scaling and drive accuracy |
| known-good relay repeated 100 cycles | check timing repeatability |
| weak-spring fault injection | confirm late release detection |
| RC snubber / diode variations | confirm suppression-mode influence on release metrics |
| welded-contact simulation | verify persistent-closure alarm |
| Hall target offset sweep | quantify travel calibration tolerance |
