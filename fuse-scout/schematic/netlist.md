# Fixture wiring reference (not a native KiCad schematic)

USB VBUS (Pico 5 V) -> RTEST 47 ohm -> JTEST red.
JTEST black -> Q1 collector (2N3904).
Q1 emitter -> GND.
Pico GP2 -> RBASE 10 kohm -> Q1 base; RPD 100 kohm from base to GND.
JTEST red -> RDIV_TOP 100 kohm -> SENSE; SENSE -> RDIV_BOTTOM 100 kohm -> GND; CADC 100 nF SENSE-to-GND.
SENSE -> Pico GP26/ADC0. Pico GND -> common fixture GND.
Optional OLED: Pico GP3 SDA and GP4 SCL, 3V3 and GND. Keep all GPIO-side circuitry at 3.3 V.

RTEST carries the test current and limits it to approximately 106 mA at 5 V for a shorted fixture. Do not attach to an energized circuit. Confirm transistor pinout from its selected manufacturer's datasheet. This text is the wiring record only; no KiCad project, footprint validation, PCB layout, or ERC result is supplied.
