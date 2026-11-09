# Wiring reference (not a KiCad schematic)

USB 5V → Pico VSYS/VBUS power path and onboard regulator → 3V3 rail. Pico GP4/SDA ↔ AS5600 SDA; GP5/SCL ↔ AS5600 SCL. SDA and SCL each have 4.7 kΩ pull-up to 3V3. Pico 3V3(OUT) → AS5600 breakout VCC; all grounds common. GP14 ↔ momentary pushbutton ↔ GND (internal pull-up enabled). GP15 → 1 kΩ → LED anode; LED cathode → GND. USB D+/D− remain connected through the Pico board for CDC/programming.

Use only a 3.3 V-compatible AS5600 breakout. The sensor module and Pico share the regulated 3.3 V domain. Verify the selected breakout does not contain pull-ups to 5 V. No external motor or hinge load is electrically driven. This is a connection diagram in text; no native KiCad schematic or PCB is supplied.
