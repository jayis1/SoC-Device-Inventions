# Wiring reference (not KiCad-native)

Pico 3V3 → HX711 VCC, AS5600 VCC; Pico GND → both module grounds, load-cell excitation ground, switch return. HX711 DOUT→GP2; SCK→GP3. AS5600 SDA→GP4 and SCL→GP5; 4.7 kΩ pullups to 3V3 only if absent from modules. AS5600 address 0x36. Button GP6→normally-open switch→GND. Load cell E+/E− and A+/A−→HX711 matching bridge terminals per module labels. USB powers Pico; Pico 3V3 powers sensors. Never apply 5 V to GPIO. Verify exact breakout pin labels and circuitry; no PCB or ERC exists.
