# Assembly and validation

1. Assemble Pico on a carrier or breadboard; leave USB accessible for BOOTSEL.
2. Wire Hall sensor supply to 5 V and ground to ground. Connect open-collector output through a 10 kΩ pull-up to Pico 3V3 and GP2. Never pull the output to 5 V.
3. Install 100 nF at the sensor and 100 nF from GP2 to ground. Connect normally-open start/stop and reset buttons from GP14/GP15 to ground.
4. Mount sensor rigidly and attach a magnet to the moving fixture. Keep leads short and strain-relieved. Check connector polarity with a meter before plugging in.
5. Compile and flash as described in the device README. Start a session, move magnet slowly, and compare the reported count with a manual count. Repeat 100 cycles and check no missed/double events.
6. Stop session and save host-side USB serial output. No data is stored in the device.

Prototype only: use a guarded, manually operated low-force cable fixture. This logger does not actuate or electrically test the cable. Inspect cable samples and fixture independently; do not infer service life from counts alone.
