# Tape Tally electrical sketch (text netlist; not a KiCad schematic)

- USB powers Raspberry Pi Pico board; no external 5 V connection.
- Pico 3V3(OUT) -> R1 220R -> D1 TCRT5000L IR LED anode; cathode -> GND.
- TCRT5000L phototransistor emitter -> GND; collector -> node SENSE.
- R2 10k from SENSE to 3V3; SENSE -> LMV331 non-inverting input.
- RV1 10k trim pot between 3V3 and GND; wiper -> LMV331 inverting input; C2 10n from wiper to GND.
- LMV331 VCC -> 3V3, GND -> GND; C1 100n at supply pins.
- LMV331 open collector output -> GP2; R3 10k from output to 3V3.
- Pico GND -> common GND.

Comparator input polarity may need swapping to make the marker edge active-high; verify with a scope before use. This is a topology note, not a validated schematic or PCB layout.
