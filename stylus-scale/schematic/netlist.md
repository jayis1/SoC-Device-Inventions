# Stylus Scale connection reference

KiCad capture and PCB layout are not included or validated; this connection list is the design reference, not a fabrication schematic.

```text
USB 5V/GND -> Pico USB connector
Pico GP0/GP1 -> SSD1306 SDA/SCL (I2C0, 3V3 pull-ups)
Pico GP2 <- HX711 DOUT
Pico GP3 -> HX711 SCK
Pico 3V3/GND -> HX711 logic supply and OLED logic supply
Load-cell E+/E-/A+/A- -> HX711 bridge excitation/input (verify actual wire colors)
Pico GP4 <- pushbutton -> GND (internal pull-up)
```

Place 100 nF between HX711 VCC and GND at its breakout. A bare HX711 design needs its datasheet-recommended analog supply, decoupling, and bridge routing; using a module does not remove the requirement to verify its input/output voltage domains. Do not connect a 5 V DOUT signal to the RP2040.
