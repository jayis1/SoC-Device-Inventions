# Assembly and safe-use notes

1. Inspect MAX31855KASA+ current datasheet for footprint, pin numbering, thermocouple range, fault behavior, and absolute maximum ratings before ordering/layout.
2. Fit U1, C1, keyed K-type connector, and Pico board according to the netlist. Keep C1 adjacent to U1. Verify connector polarity and continuity with power disconnected.
3. Check that thermocouple metal is isolated from the iron's mains/heater wiring. Do not open or modify an iron. Stop if isolation is uncertain.
4. Connect USB through a current-limited supply. Verify +3V3 is 3.1–3.4 V relative to GND before inserting U1 or probe. Stop on unexpected heating, odor, or excess current.
5. Confirm SPI wiring and test with a thermocouple simulator or known-temperature reference. A room-temperature reading alone does not calibrate the complete probe assembly.
6. Log only after establishing polarity and plausible ambient temperature. Treat the device as comparative bench instrumentation; not a calibrated thermometer.

No PCB layout, enclosure, firmware adapter, calibration certificate, or hardware test is included.
