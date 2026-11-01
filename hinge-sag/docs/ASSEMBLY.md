# Assembly and bring-up

1. Verify U2 breakout accepts 3.3 V and supports I2C; set SA0 low and disable its on-board I2C pullups if those are already fitted.
2. Wire the Pico module, LIS3DH, OLED, 4.7 kΩ shared pull-ups, and capture switch exactly as in `../schematic/netlist.md`. Keep wiring short.
3. With USB disconnected, check for shorts between 3V3 and GND. Power from USB only; confirm sensor supply is approximately 3.3 V before connecting the modules.
4. Host-build the core and run CTest as in the device README. This checks math/state logic only; it does not program the Pico.
5. A target adapter must first verify the sensor WHO_AM_I response and I2C addresses, then stream acceleration samples. Confirm static gravity magnitude is near 1 g and that rotating the fixture changes reported angle before collecting comparative trials.
6. Clamp securely to an unloaded panel. Do not use the fixture on a heavy or unsupported door.

No physical bring-up has been performed. Module pinouts vary; inspect vendor documentation before wiring.
