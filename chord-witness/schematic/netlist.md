# Chord Witness reference netlist

Not a native KiCad drawing; this connectivity list is the authoritative conceptual netlist. Verify component ratings and layout before fabrication.

| Ref | Part/value | Connections |
|---|---|---|
| J1 | Piezo pickup, 2-pin | pin 1 to R1; pin 2 to GND |
| R1 | 100 kΩ series | J1.1 to U1A non-inverting input |
| R2,R3 | 100 kΩ, 1% | +3V3 to VBIAS; VBIAS to GND |
| C1 | 10 µF | VBIAS to GND; buffered by U1B as voltage follower |
| U1 | MCP6002-I/P, DIP-8 | VDD +3V3, VSS GND; U1A input at J1 after R1; feedback gain 1 + 100k/100k (R4/R5); output to R6 |
| R4,R5 | 100 kΩ | U1A output to inverting input; inverting input to VBIAS |
| R6 | 1 kΩ | U1A output to J2.1 |
| D1,D2 | BAT54S dual Schottky | clamp J2.1 to GND and +3V3 |
| J2 | Pico header | pin 1 signal to GP26/ADC0; pin 2 GND |

Important: MCP6002 is powered from 3.3 V; its input common-mode and output range are rail-to-rail. The amplifier is configured around VBIAS. Confirm clamp diode orientation and input loading in an actual schematic/layout. This text netlist has not undergone ERC.

Power comes from the Pico 3V3 output and shares ground. The 100 kΩ/100 kΩ divider draws approximately 16.5 µA at 3.3 V, excluding amplifier current. Observe the Pico's aggregate 3V3 budget.
