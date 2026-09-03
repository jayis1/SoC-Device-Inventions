# Torque Pebble Assembly Guide

## 1. Mechanical Stack

### Core body

- CNC machine a two-part aluminum housing with a steel internal torsion shaft.
- Leave a reduced gauge section on the shaft for the strain gauges.
- Support the shaft with two compact radial bearings so torque passes cleanly through the gauge section.
- Keep the magnet target centered over the AS5600 within 0.5-2.0 mm gap.

### Drive interfaces

- input: female 1/4 in square or inline hex socket
- output: male 1/4 in square or hex quick-change stub
- optional interchangeable nose pieces for 3/8 in service work

## 2. Strain Gauge Bonding

1. Polish the torsion section to 600-1000 grit.
2. Degrease with acetone, then neutralize with gauge-prep conditioner.
3. Bond four 350 Ω foil gauges at +/-45° to the shaft axis.
4. Wire as a full bridge to reject bending and temperature drift.
5. Overcoat with gauge protection epoxy and silicone strain relief.

Recommended wiring:

- SG1 and SG3: +45°
- SG2 and SG4: -45°
- use 32-36 AWG flexible wire
- twist excitation and sense pairs separately

## 3. PCB Bring-Up Order

1. Populate the buck regulator and charger first.
2. Verify 3V3 and 3V0 analog rails.
3. Program the ESP32-S3 bootloader and verify USB/UART console.
4. Check I2C discovery for AS5600, TMP117, DS3231, MAX17048, and OLED.
5. Check SPI communication with IMU and microSD.
6. Zero the bridge front end with the shaft unloaded.
7. Run torque calibration with known lever-arm weights.

## 4. EMI and Analog Layout Notes

- route bridge traces as a tightly coupled differential pair
- guard INA333 inputs with analog ground copper where practical
- keep Wi-Fi antenna away from the bridge front end and SD traces
- place the buck inductor far from the strain-gauge amplifier
- use a star connection for analog ground return into the ADC/INA section

## 5. Calibration Fixture

A simple fixture works well:

- clamp the housing body in a vise
- insert a 200 mm calibration arm into the output square drive
- hang calibrated masses from the arm
- measure both CW and CCW loading
- record raw counts and fit linear coefficients

Useful torque points with 200 mm arm:

| Mass | Torque |
|------|--------|
| 2.55 kg | 5.0 N·m |
| 5.10 kg | 10.0 N·m |
| 7.65 kg | 15.0 N·m |
| 10.20 kg | 20.0 N·m |

## 6. Safety

- do not exceed the designed torque limit of the shaft
- do not use with impact drivers or hammers
- keep LiPo isolated from the torsion section and any metallic swarf
- if the device is used on energized electrical equipment, treat it only as an indicator; it is not an insulated torque tool

## 7. Suggested Validation Tests

- repeated zero return after 100 load cycles
- creep at 80% full scale for 60 s
- clockwise / counterclockwise hysteresis
- angle wraparound across 0° boundary
- back-off event detection with deliberate reverse motion
- battery runtime during logging + BLE streaming
