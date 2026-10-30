# Connectivity reference (not a KiCad schematic)

- J1: Pico USB connector, 5 V and data.
- U1: Raspberry Pi Pico module; module regulator supplies 3V3.
- RV1 ends: U1 3V3(OUT) and GND; wiper: GP26/ADC0. Use a 10 kOhm linear potentiometer; ensure no mechanical end-stop drives ADC outside 0–3.3 V.
- SW1: GP2 to GND, active low, internal pull-up enabled in target adapter.
- SW2: GP3 to GND, active low, internal pull-up enabled in target adapter.
- J2/J3 are optional carrier-board connectors; observe pin labels, do not infer connector orientation from this list.
- No external voltage, battery, charger, protection network, or isolation barrier is present.

All GPIO connections are 3.3 V domain. Add local 100 nF decoupling at any carrier-board ICs (none required by this minimal circuit). This document is not a netlist export or fabrication drawing.
