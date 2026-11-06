# Tea Steep connection-level schematic

This is an auditable wiring/netlist specification, not a native KiCad schematic. All grounds join at Pico GND. Use a Pico board (not a bare RP2040 design).

| Ref | Part | Pin/net connections |
|---|---|---|
| U1 | Raspberry Pi Pico | GP4=I2C_SDA, GP5=I2C_SCL, GP2=ONEWIRE, GP15=LED_EN, 3V3=3V3, GND=GND, USB=5V input |
| U2 | TCS34725 breakout | VCC=3V3, GND=GND, SDA=I2C_SDA, SCL=I2C_SCL, INT=NC; address 0x29 |
| U3 | DS18B20 probe | VDD=3V3, GND=GND, DQ=ONEWIRE |
| Q1 | 2N7002 | G=LED_GATE, S=GND, D=LED_K |
| D1 | white LED | A=3V3 through R1, K=LED_K |
| R1 | 1 kΩ | 3V3 to D1 A (nominal ~1 mA; confirm LED Vf and resistor dissipation) |
| R2 | 100 Ω | GP15 to LED_GATE |
| R3 | 100 kΩ | LED_GATE to GND |
| R4 | 4.7 kΩ | I2C_SDA to 3V3; omit if breakout already has suitable 3V3 pull-up |
| R5 | 4.7 kΩ | I2C_SCL to 3V3; omit if breakout already has suitable 3V3 pull-up |
| R6 | 4.7 kΩ | ONEWIRE to 3V3 |
| C1 | 100 nF | U2 VCC to GND, placed at breakout supply pins if absent |
| C2 | 10 µF | 3V3 to GND near sensor connector |

Probe connector J1: pin 1=3V3, pin 2=ONEWIRE, pin 3=GND. Check the exact DS18B20 cable color/pinout against its datasheet/vendor; colors are not guaranteed. Sensor breakout must not drive I2C above 3.3 V. Keep GP0/GP1 available for UART/debug and use Pico SWD pads for debug. No external LED power or battery charging is included.
