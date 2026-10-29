# Schematic status

Electrical connection schedule (schematic capture pending; no KiCad schematic file is represented as complete).

| Ref | Pin/terminal | Connection |
|---|---|---|
| U1 Pico H | GP2 | ENC1 A; internal pull-up enabled |
| U1 Pico H | GP3 | ENC1 B; internal pull-up enabled |
| U1 Pico H | GP14 | S1 output; external 10 kΩ pull-up to Pico 3V3 |
| U1 Pico H | GP15 | SW1 normally-open to GND; internal pull-up enabled |
| U1 Pico H | GP25 | Onboard LED, no external load |
| U1 Pico H | 3V3 | S1 supply |
| U1 Pico H | GND | ENC1 common, S1 ground, SW1 return |
| U1 Pico H | USB | 5 V input and USB CDC data |

S1 is specified as an open-collector interrupter; connect its emitter to GND and collector to GP14, with R1 from GP14 to 3V3. Verify the chosen EE-SX1103 leadout against its datasheet before assembly. Do not substitute a sensor requiring a 5 V logic output without level translation.

This schedule is not an electrical-rule-checked CAD schematic. Verify physical connector pin numbers against the Pico H pinout before wiring.