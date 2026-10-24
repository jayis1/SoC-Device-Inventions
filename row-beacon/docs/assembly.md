# Assembly guide

This is a module-based reference assembly, not a fabricated PCB. Confirm the chosen Pico, Hall switch variant, and OLED module datasheets before wiring.

1. Mount the genuine Raspberry Pi Pico on perfboard; leave USB connector, BOOTSEL, and SWD pads accessible.
2. Wire Pico 3V3 and GND rails. Add C1 (100 nF) at U1 and C2 (10 uF) near OLED power.
3. Wire U1 DRV5032 supply to 3V3/GND, output to GP2, with R1 (10 kOhm) from output to 3V3. Confirm the specific suffix implements the expected digital output behavior.
4. Wire OLED SDA/SCL to GP4/GP5 and power it from 3V3. Ensure any onboard pull-ups terminate at 3V3; never allow a 5 V pull-up.
5. Wire the encoder common terminals to GND and A/B/push to GP10/GP11/GP12. Use software pull-ups.
6. Before powering, check continuity for shorts and verify polarity. Apply USB power and measure 3V3 rail.
7. Move a secured marker magnet past the sensor repeatedly. Check the sensor's active level and gap with a meter/logic analyzer before mounting.
8. Route the sensor wire and attach it to a stable fabric-edge clip. Ensure no wire or magnet can catch yarn, skin, or the moving needle.

No target firmware image or display driver is supplied in this reference version. Do not interpret a successful core syntax check as a complete build or validated counter.
