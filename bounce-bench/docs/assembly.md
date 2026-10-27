# Assembly and first power-up

1. With power disconnected, inspect USB-C orientation, U2 orientation, and all capacitor polarity (the listed capacitors are non-polarized).
2. Confirm J2 is isolated from any powered equipment. Connect UART adapter GND/TX/RX to J3 GND/RX/TX; adapter logic must be 3.3 V.
3. Set bench supply to 5.0 V with 100 mA current limit and feed the USB power input. Measure 3.3 V at U1 VDD relative to GND before inserting the MCU if socketed; expected 3.2–3.4 V.
4. Flash the CH32V003 over SWIO using WCH-LinkE. Verify UART boot banner at 115200 8N1.
5. Short TEST to GND through a normally-open dry contact, send `T`, close the contact, then inspect CSV output. No physical test has been performed for this reference design.

Stop if the rail is outside tolerance or the board draws unexpectedly high current. Pin-level assignments and exact package pad mapping must be checked against the WCH datasheet before PCB layout.