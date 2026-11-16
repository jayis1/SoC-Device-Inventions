# Hinge Witness proposed net connections

This is a textual prototype connectivity record, not a KiCad schematic and not suitable for fabrication.

- U1: Raspberry Pi Pico module. USB VBUS powers the module; 3V3(OUT) supplies U2.
- U2: 3.3 V-compatible AS5600 breakout. VCC=3V3, GND=GND, SDA=GP4, SCL=GP5. I2C address 0x36.
- R2: 4.7 kΩ from SDA to 3V3; R3: 4.7 kΩ from SCL to 3V3 (omit if equivalent breakout pull-ups are already fitted).
- SW1: GP14 to GND; configure GP14 internal pull-up.
- GP15 -> R1 (1 kΩ) -> D1 anode; D1 cathode -> GND.
- C1 100 nF between U2 VCC and GND, placed adjacent to U2.

Check module pin labels against the actual breakout before wiring. Sensor/magnet mechanical alignment is part of the measurement system.
