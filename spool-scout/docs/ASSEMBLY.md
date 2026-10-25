# Assembly guide

1. Use a Raspberry Pi Pico module and power it only through USB during prototype testing.
2. Wire GP0/GP1 to both I2C modules, with one 4.7 kΩ pull-up per line. Confirm 3.3 V logic before connecting.
3. Install the AS5600 above a diametric magnet fixed concentrically to the 20 mm effective-circumference measuring wheel. Adjust air gap per the AS5600 datasheet.
4. Mount EE-SX671 so the slotted spool flag crosses the optical gap freely. Verify sensor output type/polarity for exact suffix and add the specified pull-up.
5. Add local decoupling at each sensor; inspect for shorts before USB connection.
6. Scan I2C addresses (expected 0x36 and 0x3C) and rotate the wheel slowly. Confirm monotonic angle readings.
7. Calibrate circumference against a ruler-measured filament length. The nominal value is only a starting point; wheel slip affects it.
8. Run a no-load and loaded feed test, including reverse feed, optical blockage, and sensor disconnection. Record false-alert behavior before any unattended use.

Test points: 5V USB, 3V3, GND, SDA, SCL, SPOOL_SLOT, RUN_LED. No PCB, enclosure, clearances, or assembly has been physically validated.
