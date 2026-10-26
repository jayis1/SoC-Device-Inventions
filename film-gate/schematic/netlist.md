# Connection reference (not a KiCad schematic)

This text netlist is the design source of truth. Electrical review and ERC have not been performed.

| Ref | Connections |
|---|---|
| U1 Pico | USB 5 V onboard; GP14 sensor digital output; GP15 switch; GP16 -> R1 -> U3 LED anode; GND common only on logic side |
| U2 optical module | VCC 3V3, GND, OUT -> GP14; adjust threshold so film gap changes state |
| SW1 | GP15 to GND; firmware enables internal pull-up |
| U3 LTV-817S | LED cathode -> GND; phototransistor collector/emitter -> camera remote contacts; no connection to Pico ground |
| R1 | 330 ohm series from GP16 to U3 LED; LED current is approximately (3.3 V - Vf)/330 ohm |
| R2 | Optional pull-up from U2 OUT to 3V3 if module lacks one |
