Reference, not fabrication-ready. Net/pin list:

- J1 Raspberry Pi Pico module: USB power; GP26/ADC0 input from R1/R2 midpoint; GP15 output through R3 to D1; 3V3(OUT) powers sensor divider; GND common.
- Q1 phototransistor: emitter to GND, collector to GP26 node.
- R1 10 kΩ from 3V3(OUT) to GP26 node; R2 10 kΩ from node to GND. This creates a light-dependent divider; verify polarity and ADC range on the assembled sensor.
- GP15 -> R3 1 kΩ -> D1 red LED -> GND.

Pico ADC0 is GP26; no 5 V signal is permitted. USB provides power, with the Pico board's onboard regulation. This connection specification is not a KiCad schematic, ERC result, or PCB layout.
