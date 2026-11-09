Reference wiring (not KiCad):

| Ref | Part | Connection |
|---|---|---|
| U1 | Raspberry Pi Pico, RP2040 | USB 5 V via board connector; board GND common |
| SW1 | Momentary MARK button | GP14 to GND when pressed |
| SW2 | Momentary NEXT button | GP15 to GND when pressed |
| R1 | 100 kΩ optional pull-up | GP14 to 3V3 if not using firmware pull-up |
| R2 | 100 kΩ optional pull-up | GP15 to 3V3 if not using firmware pull-up |

Configure internal pull-ups; omit R1/R2 in that configuration. No external power injection. Add local button debounce in the target adapter. Board-native USB provides power and CDC; SWD pads provide debug. This document is a textual wiring reference, not a complete KiCad schematic or PCB.
