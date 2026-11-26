# Pin-level reference netlist (not a KiCad schematic)

Prototype is based on a Raspberry Pi Pico module; use the module's documented pin names, not bare RP2040 package pins.

| Ref | Connection |
|---|---|
| J1 (DUT) pin 1 | 3V3 through R1 10 kΩ to GP16; contact input node |
| J1 pin 2 | GND |
| GP16 | R2 1 kΩ series to J1 pin 1; C1 100 nF from GP16-side node to GND |
| D1 BAT54 | Clamp node to GND (anode GND, cathode node) |
| D2 BAT54 | Clamp node to 3V3 (anode node, cathode 3V3) |
| SW1 | GP15 to GND; firmware pull-up |
| D3 status | GP14 through R3 1 kΩ to LED anode; LED cathode GND |
| U1 | Raspberry Pi Pico; USB 5 V input, onboard 3V3 regulator |

Connector labels are functional only; confirm actual module headers and connector polarity before assembly. This is not isolation, overvoltage protection for energized circuits, or a fabrication-ready drawing. Capacitor/input filtering means the stated 1 kHz sampled implementation requires measured settling validation.
