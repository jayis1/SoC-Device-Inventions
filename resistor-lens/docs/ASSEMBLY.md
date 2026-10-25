# Assembly

1. Use a Raspberry Pi Pico and a TCS34725 breakout with 3.3 V I2C pull-ups. Verify breakout pin labels and supply limits against its documentation.
2. Connect GP4/SDA and GP5/SCL, 3V3, and GND. Keep wiring short; confirm no breakout pull-up goes to 5 V.
3. Connect GP6 to a 2N7002 gate with 100 kΩ gate-to-GND; source to GND; drain to LED cathode. Connect LED anode to 3V3 through 330 Ω.
4. Inspect for shorts, power by USB, and run firmware. Verify `sensor_not_found` does not occur. If I2C fails, disconnect power and check address 0x29 and SDA/SCL.
5. Place one resistor against a matte neutral background. Shield the sensor from ambient light and record channel values. The current firmware does not automatically locate bands or compensate color; use the separate band decoder only after manual identification.

No mains or battery wiring is involved. This is a sorting aid, not a precision measurement instrument.
