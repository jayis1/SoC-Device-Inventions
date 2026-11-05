# Wick Watch connection-level schematic

This is a connection-level netlist, not a native KiCad schematic or PCB.

- J1 Pico board: GP2..GP6 to J2 pins S1..S5; GP14 to SW1; GP15 to R1; 3V3(OUT), GND.
- J2 five digital reflective sensor modules: each VCC to 3V3, GND common, DO to its signal input. Confirm onboard output pull-up is tied to 3V3; otherwise add R2..R6 10 kΩ from output to 3V3. Do not connect modules configured for 5 V.
- SW1 momentary normally-open switch between GP14 and GND.
- LED1 driven by GP15 through R1 1 kΩ to GND.
- USB power enters through Pico USB connector; no external power connection.

Keep sensor wiring short and route signal/ground as pairs. Five separate threshold pots/modules are adjusted independently. Board pinout must be checked against the purchased Pico revision before wiring.
