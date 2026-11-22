# Proposed wiring reference (not a KiCad schematic)

| Net | Endpoints | Notes |
|---|---|---|
| USB_VBUS | Pico USB connector; onboard regulator input | 5 V module input only |
| 3V3 | Pico 3V3 output, sensor VCC, C1.1 | Do not use external 5 V sensor supply |
| GND | Pico GND, sensor GND, C1.2, SW1.2 | Common return |
| SENSOR_AOUT | Sensor analog output to Pico GP26/ADC0 | Confirm 0–3.3 V range at selected breakout |
| RUN_N | Pico GP15 to SW1.1 | Internal pull-up, button to GND |

Sensor optical emitter/receiver arrangement and resistor values depend on the selected analog breakout. Select a breakout with documented 3.3 V-compatible analog output; do not fabricate from the sensor alone using this table. C1 is a 100 nF local bypass. This reference is not native KiCad, ERC-checked, or fabrication data.
