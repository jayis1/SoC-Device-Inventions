Reference: signal wiring only; use Pico board module, not bare RP2040.

USB VBUS/VSYS powers Pico; do not connect an external supply to VSYS simultaneously.
Pico 3V3 rail -> 330R -> LED anode; LED cathode -> GPIO16.
GPIO15 -> normally-open float switch -> GND. Enable internal pull-up in firmware.
Pico GND shared with switch and LED.
No sensor power rail, charger, pump driver, or exposed mains connection.
