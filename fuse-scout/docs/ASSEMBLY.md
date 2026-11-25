# Assembly and verification

1. Assemble the 47 ohm resistor in series between Pico VBUS and the red test clip; do not bypass it.
2. Wire the black clip to Q1 collector and Q1 emitter to GND. Check 2N3904 pinout against the purchased part's datasheet; TO-92 pin order varies.
3. Connect the base through 10 kohm from GP2 and add 100 kohm base-to-emitter pulldown.
4. Build the 100 kohm / 100 kohm divider from red clip to ground; connect its midpoint to GP26/ADC0 and add 100 nF to ground.
5. Inspect for shorts before plugging USB. With TEST_EN low, use a DMM to verify no unintended connection from USB 5 V to any GPIO.
6. Use only a removed, unpowered fuse or test resistor. Start with a current-limited USB source; verify open, short, and intermediate known resistor states.

The included C file is a portable classification/averaging core, not an end-to-end firmware image. Hardware acquisition, enclosure, and display code remain future integration work. This wiring note is not a KiCad schematic and has not undergone ERC or physical test.
