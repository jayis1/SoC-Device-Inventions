Design connection list (not a KiCad capture; no ERC performed).

J1 Pico header: 3V3(OUT) -> J2-1; GND -> J2-2; GPIO26/ADC0 <- J2-3 through R1 10 kΩ. C1 100 nF from GPIO26 node to GND; C2 10 µF from 3V3 to GND near J2. SS49E on J2: VCC=3V3, GND=GND, OUT=J2-3. GPIO15 -> R2 1 kΩ -> passive piezo positive; piezo negative -> GND. Use USB-powered Pico; no external 5 V connection. Verify sensor pin order against the exact purchased package datasheet before assembly.
