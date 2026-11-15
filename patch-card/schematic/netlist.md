# Connectivity reference (not a PCB schematic)

This pin-by-pin wiring schedule is the canonical schematic reference; module pin order must be checked against the exact purchased breakout.

| Net | Connections |
|---|---|
| USB_5V | Pico USB input only |
| +3V3 | Pico 3V3(OUT), AS5600 breakout VCC, two optical module VCC pins; all modules must accept 3.3 V |
| GND | Pico GND, sensor grounds, LED cathodes through resistors |
| I2C0_SDA | Pico GP4, AS5600 SDA, 4.7 kΩ to +3V3 |
| I2C0_SCL | Pico GP5, AS5600 SCL, 4.7 kΩ to +3V3 |
| GATE_A_N | Optical module A comparator output, Pico GP14 with pull-up |
| GATE_B_N | Optical module B comparator output, Pico GP15 with pull-up |
| LED_OK | Pico GP16 -> 330 Ω -> LED -> GND |
| LED_ERR | Pico GP17 -> 330 Ω -> LED -> GND |

Place 100 nF ceramic from +3V3 to GND at each module. Do not join any 5 V output to a Pico GPIO. This reference is not an editable KiCad schematic and is not ERC-checked; the project is a wiring reference, not a fabrication-ready design.
