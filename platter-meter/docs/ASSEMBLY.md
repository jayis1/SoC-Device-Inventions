# Assembly and bring-up

1. Attach one matte reflective label near the platter rim; ensure it is secure and does not foul the tonearm or stylus.
2. Power a Raspberry Pi Pico from USB. Connect a 3.3 V logic TCRT5000 module as shown in `../schematic/netlist.md`; confirm OUT high is <=3.3 V before connecting GP2.
3. Build using Pico SDK 2.1.1 and ARM GNU toolchain. Flash UF2 by holding BOOTSEL during USB connection.
4. Position sensor 2–5 mm over the label path and secure the cable so it cannot contact moving parts.
5. Open USB CDC at 115200 baud; slowly rotate platter and confirm one edge per revolution. A reported reading requires eight valid revolution intervals.
6. Compare measurements against a trusted tachometer at multiple speeds before using numeric readings.

This is a low-voltage USB-only prototype. Never connect it to mains or turntable audio circuits. Remove the marker before normal playback if it could detach or touch the stylus. No physical assembly or test has been performed for this reference design.
