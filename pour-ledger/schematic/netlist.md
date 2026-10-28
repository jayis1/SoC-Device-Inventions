# Electrical netlist / schematic reference

Not a native KiCad schematic. GPIO names are Pico header GPIO numbers. Verify module pin labels against the specific supplier board before wiring.

| Ref | Pin/net | Connection |
|---|---|---|
| U1 Pico | GP14 | U2 DOUT |
| U1 Pico | GP15 | U2 SCK |
| U1 Pico | GP4 / GP5 | DS1 SDA / SCL, I2C0 |
| U1 Pico | GP10 / GP11 | SW1 / SW2, each closes to GND |
| U1 Pico | 3V3 / GND | U2 logic supply, DS1 supply, common ground |
| U2 HX711 | E+/E-/A+/A- | Load-cell excitation and differential bridge input per cell datasheet |
| U1 Pico | USB VBUS | USB 5 V input; do not connect external 5 V to 3V3 |

Use local 4.7 kΩ pull-ups only if the OLED module does not already contain them. Keep load-cell sense wiring twisted and away from USB and display switching currents. HX711 breakout layouts and load-cell wire colors vary; confirm before power-up.
