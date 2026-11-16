# Assembly and bring-up

1. Mount the TSL2591 breakout facing the light source, fixed relative to the blind.
2. Couple a diametrically magnetized AS5600-compatible magnet to the tilt shaft; center the sensor IC beneath it. Keep the magnet-to-chip gap within the carrier guidance.
3. With USB disconnected, wire Pico GP4 to both SDA pins, GP5 to both SCL pins, 3V3(OUT) to both VCC pins, and GND to both grounds. Confirm carrier voltage compatibility before connecting.
4. Ensure I2C pull-ups terminate at 3.3 V. Avoid stacking multiple carrier pull-ups without checking their parallel resistance.
5. Connect Pico USB. Scan for 0x29 and 0x36 before enabling acquisition. If either is absent, report a sensor fault; do not interpret missing data as zero.
6. Compare repeated readings at fixed light-source and sensor geometry. This design is uncalibrated and unsuitable for compliance measurements.

Disconnect USB before rewiring. No mains, blind-motor, battery, or charger connection is part of this design.
