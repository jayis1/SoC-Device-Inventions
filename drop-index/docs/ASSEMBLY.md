# Assembly and test

1. Mount the load cell as specified by its supplier; do not constrain both ends or preload beyond the rated load. Fit a removable cup platform that cannot touch the enclosure.
2. With USB disconnected, wire the bridge to the HX711 module labels. Connect the module to Pico as in README and pinmap.csv. Confirm module VCC and DOUT voltage compatibility before attaching to the Pico.
3. Inspect polarity and continuity. Keep water physically separated from the electronics; use a spill tray.
4. Connect by USB, flash the UF2 produced by the Pico SDK build, and open a 115200-baud serial terminal (USB CDC ignores baud setting on most hosts).
5. Place the empty cup and tare by restarting/resetting the firmware. Use a known mass to determine signed counts per gram. Replace `COUNTS_PER_GRAM` in `main.c`, rebuild, and confirm at two known masses; this reference code has no interactive calibration.
6. Place one drip outlet over the cup, press the start button, and read 60 seconds of CSV. Do not interpret noisy single-second rates as a calibrated flow result. Repeat measurements and compare mean collected mass.

No mains connection, battery charging, or exposed high-energy output exists. This is an untested reference assembly, not an enclosure or spill-resistant product.
