# Sky Glow wiring netlist (schematic-stage reference)

Not a native KiCad drawing or fabrication release. Use only 3.3 V-compatible breakout boards and check the exact board revision.

| Ref | Part pin | Net |
|---|---|---|
| J1 Pico, pin 36 | 3V3(OUT) | +3V3 |
| J1 Pico, pin 38 | GND | GND |
| J1 Pico, pin 6 | GP4 | I2C_SDA |
| J1 Pico, pin 7 | GP5 | I2C_SCL |
| J1 Pico, pin 25 | GP25 | LED_STATUS |
| J2 TSL2591 breakout | VIN, GND, SDA, SCL | +3V3, GND, I2C_SDA, I2C_SCL |
| J3 optional SSD1306 breakout | VCC, GND, SDA, SCL | +3V3, GND, I2C_SDA, I2C_SCL |
| R1/R2 optional | 4.7k from SDA/SCL to +3V3 | I2C pull-ups |
| C1/C2 optional | 100nF across module supply | +3V3/GND |

Typical addresses: TSL2591 0x29 and display 0x3C; verify straps. Fit pull-ups only if modules do not provide them; parallel pull-ups can become too strong. USB connects to Pico only. No charging or reverse-input circuit is included. Check rail polarity/shorts before connection.
