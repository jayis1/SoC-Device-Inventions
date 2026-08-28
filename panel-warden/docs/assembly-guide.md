# Panel Warden Assembly Guide

## 1. Safety First

Panel Warden is installed **only when the target panel is fully de-energized, locked out, and verified dead**.

- follow site electrical safety procedures
- wear appropriate PPE
- do not mount or reposition the device inside an energized enclosure
- never allow the CT accessory lead to contact live conductors directly

## 2. Mechanical Build Order

1. Populate the power section first: USB-C, charger, buck-boost, battery connector.
2. Bring up the ESP32-S3 and verify USB service console operation.
3. Populate the I2C sensors: MLX90640, SHT45, TMP117 x2, OLED.
4. Populate the ADC and analog front end.
5. Install the ultrasonic receiver on silicone grommets.
6. Attach the magnetic contact tongue with Kapton insulation on all exposed metalwork.
7. Add the battery pouch and insulated magnet feet.

## 3. Calibration Flow

### Thermal

- point the unit at a matte black reference target at two known temperatures
- record offset/gain correction for the MLX90640 hottest-pixel estimator
- store coefficients in NVS

### Ultrasonic

- use a 40 kHz speaker or reference ultrasonic leak source
- verify band-pass gain and envelope response
- set the baseline threshold so quiet enclosures read < 12 dB equivalent

### RF Burst Detector

- inject a near-field burst source or use a handheld RF comb generator
- verify AD8318 output slope and clipping margin

### Condensation Margin

- compare SHT45/TMP117 against a known hygrometer and contact thermometer
- validate dewpoint and dew-margin computation in software

## 4. Installation Inside a Panel

1. Open and lock out the enclosure.
2. Select a mounting spot with line-of-sight to the most failure-prone lugs or breakers.
3. Keep at least 20 mm clearance around the unit.
4. Route the optional CT lead away from sharp sheet-metal edges.
5. Press the contact temperature tongue against the cabinet wall or designated monitored metal bracket.
6. Close the panel and restore power.

## 5. Recommended Alarm Thresholds

| Condition | Suggested threshold |
|-----------|---------------------|
| Loose-lug suspect | delta-T > 25 C with load < 50% expected |
| Overload heating | delta-T > 20 C and CT load sustained high |
| Corona | ultrasonic > 28 dB equivalent for > 10 s |
| Surface tracking | ultrasonic > 30 dB and dew margin < 4 C |
| Arc | RF > -40 dBm equiv bursts with ultrasonic coincidence |
| Condensation | dew margin < 2 C |

## 6. Maintenance Notes

- clean the IR aperture and acoustic membrane monthly in dusty sites
- replace the battery every 2-3 years in high-heat enclosures
- inspect magnets and insulation sleeves for cracking
- review event logs after any breaker trip, nuisance alarm, or moisture intrusion event
