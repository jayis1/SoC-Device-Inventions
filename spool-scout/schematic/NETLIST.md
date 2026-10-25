# Connections reference (not a KiCad schematic)

| Ref | Part | Pin/signal | Net | Destination |
|---|---|---|---|---|
| U1 | Pico | 3V3(OUT), GND | 3V3, GND | U2/U3/U4, pull-ups, decoupling |
| U1 | Pico | GP0 / GP1 | I2C0_SDA / I2C0_SCL | U2 AS5600 SDA/SCL; U4 SSD1306 SDA/SCL |
| U1 | Pico | GP2 | SPOOL_SLOT | U3 output; R3 10k pull-up to 3V3 |
| U1 | Pico | GP3 | RUN_LED | R4 1k then D1 to GND |
| U1 | Pico | GP4 | USER_BUTTON_N | SW1 to GND; firmware pull-up |
| U2 | AS5600 | VDD/GND | 3V3/GND | local C1 100nF |
| U3 | EE-SX671 | Vcc/GND/output | 3V3/GND/SPOOL_SLOT | Verify selected suffix output polarity and wiring against datasheet |
| U4 | OLED | VCC/GND/SDA/SCL | 3V3/GND/I2C0 | local C3 100nF; verify module is 3.3 V compatible |
| J1 | Pico USB | VBUS/D+/D-/GND | USB | Use Pico onboard connector only |

R1/R2 (4.7k) pull SDA/SCL to 3V3. C2 100nF at optical sensor; C4 10uF bulk across 3V3/GND. This connection schedule is a reference for later CAD capture, not a fabricated or electrically checked schematic. Confirm exact package pin numbering and EE-SX671 variant before board design.
