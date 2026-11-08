# Cooldown Clerk connectivity reference

This is an explicit hand-wired reference netlist, not a KiCad schematic or PCB. Verify module pin labels and Pico header orientation before assembly.

| Ref | Part/pin | Net |
|---|---|---|
| J1 | Pico GP2 | OW_DATA |
| R1 | 4.7 kΩ, 1%, 0603; 1-2 | 3V3 / OW_DATA |
| J2 | waterproof DS18B20 red/VDD | 3V3 |
| J2 | black/GND | GND |
| J2 | yellow/DQ | OW_DATA |
| J3 | SSD1306 SDA | I2C0_SDA (GP4) |
| J3 | SSD1306 SCL | I2C0_SCL (GP5) |
| J3 | VCC/GND | 3V3/GND |
| SW1 | one contact | GP15 |
| SW1 | other contact | GND |
| J1 | Pico USB connector | USB 5 V input via onboard regulator |

OLED address expected 0x3C (7-bit); confirm module straps. No other devices share the bus. Ensure OLED module's pull-ups, if fitted, terminate at 3.3 V. No-connect: Pico unused GPIOs and probe shield (insulate; do not use as signal return).