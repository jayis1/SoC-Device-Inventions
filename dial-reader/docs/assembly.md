# Assembly and bring-up

1. Build the 16-channel sensor ring on a prototype carrier; confirm Q1-Q16 pinout from vendor datasheet. The optical parts must view the dial surface and pointer at a fixed standoff.
2. Wire mux address pins and output to the Pico per the device README. Keep every signal at 0–3.3 V.
3. Connect ZERO/SPAN switches from their GPIO inputs to ground; keep USB unplugged while checking continuity.
4. Connect USB, flash the UF2 using the Pico BOOTSEL procedure, open the USB CDC serial port at any baud setting (USB CDC ignores baud).
5. With the pointer at the desired minimum, press ZERO; then at maximum, press SPAN. Confirm readings increase monotonically across the sweep.

Initial prototype check: log all 16 raw ADC values on a temporary test build under bright, dim, and diffuse lighting. Adjust sensor resistor/standoff and add a shroud if ambient light causes ambiguous minima. Do not use on gauges whose pointer/face lacks usable contrast. Calibration does not unwrap an angular sweep crossing 0° in this firmware. There is no calibration traceability or safety certification.

Assembly documentation by jayis1.
