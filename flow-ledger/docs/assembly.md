# Assembly guide

1. Install the Raspberry Pi Pico SDK 2.1.x and toolchain; identify the exact 40-pin Pico board variant.
2. Assemble the sensor head on perfboard following `../schematic/connections.md`. Observe TEPT4400 polarity (verify its datasheet lead orientation); put R2 between GP15 signal and 3V3. Observe IR LED polarity and put R1 in series.
3. Connect head to Pico 3V3, GND, GP15 and GP12. Connect status LED to GP17 through R3. Add C1 across sensor VCC/GND. Never connect a meter conductor or external voltage to the Pico.
4. With USB disconnected, check for shorts between 3V3 and GND and inspect LED polarity. Power by USB.
5. Build/flash per the root README. Open USB CDC at 115200 baud. The firmware waits briefly at boot, then counts falling edges; move a reflective target across the optical path to test.
6. Mount in a removable, non-obstructive clamp. Shield sensor from ambient light and adjust distance. Compare counts against a known number of complete dial rotations before use.

This prototype has no enclosure, PCB, reverse-polarity protection on external wiring, or environmental rating. Keep it dry and accessible. Do not attach to sealed or utility-restricted areas.
