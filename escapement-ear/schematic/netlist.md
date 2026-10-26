# Reference netlist (not ERC-verified)

This is a through-hole/breakout prototype connectivity description, not a PCB schematic. Verify exact package pin numbering against the purchased manufacturer datasheets before wiring.

| Ref | Part | Connections |
|---|---|---|
| J1 | Raspberry Pi Pico | USB input; 3V3(OUT), GND; GP14=TICK, GP4=OLED_SDA, GP5=OLED_SCL |
| PZ1 | passive piezo disc | one side GND, other to R1 input node |
| R1 | 1 MΩ | PZ1 node to +3V3 bias node; piezo node also C1 and U1 non-inverting input via R2 |
| C1 | 10 nF | piezo node to GND (input RF/noise filter) |
| D1,D2 | BAT54S clamp pair | piezo node clamps to GND and +3V3; confirm orientation before assembly |
| U1 | MCP6001 | 3V3/GND supply; non-inverting input via R2; inverting feedback R3=100kΩ from output and R4=10kΩ to GND; output to U2 input |
| R2 | 10 kΩ | piezo node to U1 non-inverting input |
| U2 | LMV331 comparator | 3V3/GND; signal input U1 output; other input threshold pot wiper; output open collector with R5 10kΩ pull-up to 3V3 and net TICK to GP14 |
| RV1 | 10 kΩ potentiometer | ends 3V3/GND; wiper comparator threshold input |
| J2 | SSD1306 I2C OLED module | VCC=3V3, GND, SDA=GP4, SCL=GP5; module pull-ups may be populated; avoid duplicate excessively strong pull-ups |
| R6,R7 | 4.7 kΩ | optional SDA/SCL pull-ups to 3V3, omit if OLED module already includes pull-ups |
| C2,C3 | 100 nF | U1 and U2 supply-to-ground local decoupling |

All GPIO signals are 3.3 V only. A complete production schematic must specify the exact LMV331 package/pinout, clamp topology, comparator polarity, and layout before fabrication. Circuit concept is intentionally not represented as a KiCad file because no KiCad validation was available.
