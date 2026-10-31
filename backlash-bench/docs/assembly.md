# Backlash Bench assembly notes

1. Install the two sensor modules facing their diametric magnets; center each magnet over the sensor and maintain the module vendor's recommended air gap.
2. Connect each breakout to a separate TCA9543A downstream channel. Verify sensor power and all logic pull-ups are 3.3 V before attaching the Pico.
3. Wire switch address pins for TCA9543A address 0x70. Connect switch upstream SDA/SCL to Pico GP4/GP5.
4. With USB disconnected, check continuity and absence of shorts between 3V3 and ground. First power from a current-limited USB source; expected total is below 55 mA but must be measured on the actual modules.
5. Flash the SDK-built UF2 via BOOTSEL. Confirm both sensors respond and CSV updates before installing the transmission.
6. Record a no-load forward/reverse trace, then test the specimen at low manual torque. Do not use this uncalibrated fixture for pass/fail safety decisions.

## Native schematic status

No KiCad schematic or PCB has been captured. `netlist.txt` is a human-readable wiring contract only and is not a native CAD deliverable or validated schematic. Treat the design as a prototype concept until an engineer captures and checks the circuit.
