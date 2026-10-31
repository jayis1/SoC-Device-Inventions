# Device connections (authoritative)

Controller is a Raspberry Pi Pico module; use its board-level schematic and datasheet rather than duplicating the RP2040 support circuitry.

| Ref | Part pin | Net | Pico connection |
|---|---|---|---|
| U1 | GPIO26 / ADC0 | ADC_SENSE | Header pin 31 |
| U1 | 3V3 | SENSOR_BIAS | Header pin 36 -> R1 |
| U1 | AGND | GND_A | Header pin 33 |
| U1 | GPIO25 | LED | Onboard LED |
| D1 | Cathode | SENSOR_BIAS | R1 100 kΩ to 3V3 |
| D1 | Anode | ADC_SENSE | Also R2 10 kΩ to ADC0; C1 100 nF to GND_A |
| J1 | USB | VBUS/D+/D-/GND | Pico onboard USB connector |

D1 is BPW34. Verify polarity and physical pinout from the manufacturer datasheet before assembly. Keep ADC_SENSE within 0–3.3 V. This passive network is a low-cost relative-light probe, not a calibrated transimpedance photometer.
