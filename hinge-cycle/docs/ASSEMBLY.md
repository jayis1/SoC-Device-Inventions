# Assembly guide

1. Use a Pico board and a breakout whose I²C pull-ups are tied to 3.3 V. Check the breakout documentation before wiring.
2. Wire GP4/GP5 to SDA/SCL; 3V3 and GND to sensor power. Add 4.7 kΩ pull-ups only if the breakout lacks them. Wire the GP14 button and GP15 LED exactly as described in the root README and schematic/netlist.md.
3. Attach the magnet to one hinge leaf and the sensor to the other, centered and parallel. Keep the magnet gap within the sensor-board vendor's stated operating range. Do not drill or permanently modify the hinge.
4. Flash a Pico USB-CDC adapter implementing the firmware interface. This repository contains only the portable event core; no complete Pico SDK application is supplied.
5. Verify serial output using controlled manual movement before collecting comparative data. If angle jumps at wraparound or misses reversals, stop: the portable core does not unwrap sensor rollover and the adapter must implement that behavior.

Power is USB 5 V through the Pico board and regulated 3.3 V to the sensor. No rechargeable cell or exposed high-energy circuitry is present. Do not use as a certified mechanical safety or lifetime test.
