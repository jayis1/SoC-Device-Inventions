# Assembly and first-power procedure

1. Confirm Pico H pin labels and mount it so USB is accessible. Keep the yarn fixture mechanically independent of the USB connector.
2. Wire encoder A/B/common to GP2/GP3/GND. Check rotation changes both signals and that neither exceeds 3.3 V.
3. Mount the photointerrupter so yarn reliably interrupts the optical slot. Wire its open-collector output to GP14, emitter to GND, and add 10 kΩ from GP14 to 3V3.
4. Wire the normally-open zero button between GP15 and GND. No external supply is required.
5. Before plugging USB in, use a multimeter to check for shorts between 3V3 and GND. Confirm the sensor output is no more than 3.3 V.
6. Flash the UF2 using the firmware README instructions. Open USB CDC at 115200 baud (CDC ignores physical baud rate); expected output is one JSON line per completed sample.
7. Clear the optical slot, press zero, and rotate the knob exactly one revolution. Confirm the reported raw count matches the configured encoder convention. Verify yarn-present state changes when yarn enters/leaves the slot.
8. Calibrate using a known-length marked specimen and compare manually counted turns. Record sample tension, fiber, length, and encoder configuration with every measurement.

Stop if the Pico resets, a component heats, or GPIO exceeds 3.3 V. No mechanical or measurement accuracy has been validated on hardware.