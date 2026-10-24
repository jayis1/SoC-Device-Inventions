# Assembly

1. Mount two slotted optical interrupters on a rigid, non-moving bracket spanning the shuttle path. Do not attach wires to moving loom parts.
2. Wire sensor supply to Pico 3V3 and ground to GND. Each open-collector output goes to GP14 or GP15 and has a 4.7 kΩ pull-up to 3V3. Add a 100 nF capacitor across each sensor's supply at the sensor.
3. Confirm sensor datasheet pinout and output topology before connection. Measure outputs; GPIO must never exceed 3.3 V.
4. Connect Pico to host via USB, flash firmware, and verify CDC JSON output while manually moving the shuttle slowly.
5. Compare counts against a manual tally at low speed before normal use. Adjust alignment if either gate chatters or events are missed.

Disconnect USB before rewiring. This is a logger only, not a guard, emergency stop, or loom control. Stop testing if the fixture risks snagging the shuttle.
