# Authoritative connection list (schematic capture pending)

U1 is a Raspberry Pi Pico module. USB supplies VBUS; use U1 3V3(OUT) only for the 3.3 V peripherals. Common ground.

| Net | Connections |
|---|---|
| I2C_SDA | U1 GP4, U2 SDA, U3 SDA, R1 to 3V3 |
| I2C_SCL | U1 GP5, U2 SCL, U3 SCL, R2 to 3V3 |
| 3V3 | U1 3V3(OUT), U2 VCC, U3 VCC, R1/R2 upper ends |
| GND | U1 GND, U2 GND, U3 GND, SW1 pin 2 |
| CAPTURE_N | U1 GP15, SW1 pin 1; firmware internal pull-up |
| STATUS | U1 GP25 onboard LED |

R1/R2 = 4.7 kΩ. U2 LIS3DH SA0 low (0x18), SDO/CS configured for I2C per selected breakout; verify module schematic before purchase. U3 SSD1306 address 0x3C. This is a netlist, not a KiCad schematic or fabrication package. Verify the exact breakout pinout before wiring.
