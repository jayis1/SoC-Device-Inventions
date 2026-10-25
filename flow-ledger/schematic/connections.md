# Flow Ledger electrical connection list

Authoritative low-voltage wiring reference for the Pico development-board prototype. Pin numbering below is the physical 40-pin Pico header where stated; confirm against the official board silkscreen before wiring. All signal levels are 3.3 V.

| Ref | Pin 1 / connection | Pin 2 / connection | Function |
|---|---|---|---|
| U1 Pico | 3V3(OUT) | GND | Regulated sensor supply |
| J2 optical head | 3V3 | GND | Phototransistor collector supply |
| Q1 TEPT4400 | Collector -> GP15, 10 kΩ -> 3V3 | Emitter -> GND | Ambient/reflective light sensing; add 100 nF from GP15 to GND if cable noise requires filtering |
| J3 IR illuminator | GP12 -> R1 220 Ω -> D1 anode | D1 cathode -> GND | IR illumination, active high |
| D2 status | GP17 -> R3 1 kΩ -> anode | Cathode -> GND | Status indicator |
| C1 | 3V3 | GND | 100 nF bypass at J2 |
| USB | Pico USB connector | host | Power and CDC data |

No circuit connects to the meter electrically. Optical alignment, ambient-light shielding, sensor polarity, and optical distance must be validated on the specific meter. This folder does not contain a native KiCad project or PCB; the connection table is the explicit prototype schematic/netlist and is not a fabrication-ready design.
