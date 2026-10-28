# Assembly and smoke-test procedure

1. With USB unplugged, mount the Pico securely in a nonconductive enclosure. Keep BOOTSEL and SWD pads accessible.
2. Connect a normally-open straight key between GP15 and GND. Connect R1 (1 kΩ) from GP14 to the passive piezo positive terminal and its negative terminal to GND. Inspect for solder bridges before power-up.
3. Build and flash as in the device README. If the SDK build is unavailable, do not improvise firmware binaries.
4. Open USB CDC at 115200 baud. With no key press, expect no output. Tap a test sequence and confirm the character appears. The current firmware uses an 80 ms dot threshold and fixed 560 ms character wait; timing modes and word spacing are not implemented yet.
5. Unplug USB before rewiring. Check that GPIO does not see a voltage above 3.3 V.

Unverified: mechanical endurance, measured current, key-bounce performance across different switches, sound level, EMC, and physical build.
