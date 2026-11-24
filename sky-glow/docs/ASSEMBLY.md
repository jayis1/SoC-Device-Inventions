# Assembly and bring-up

1. Connect Pico 3V3(OUT), GND, GP4/SDA and GP5/SCL to the TSL2591 breakout. Optional OLED shares these nets. Confirm module logic/pull-ups are 3.3 V; remove redundant pull-ups if necessary.
2. Inspect polarity and shorts. Connect USB only to the Pico; do not inject external 5 V.
3. Build using the Pico SDK 2.x commands in the main README and flash UF2 with BOOTSEL. Open USB CDC and verify rows. Firmware currently initializes and reads TSL2591 and does not initialize the OLED.
4. Fit a matte-black baffle tube, point away from direct lights, and allow readings to settle. Calibrate against a suitable reference instrument before claiming absolute sky brightness.

No PCB, enclosure, calibration, fabrication output, target build or physical test is supplied. Treat this as a schematic-stage reference.
