# Assembly and first power-up

1. Inspect Pico orientation, connector polarity, and probe cable continuity with USB disconnected.
2. Connect the three-wire DS18B20: VDD to 3V3, GND to ground, DQ to GP2. Fit R1 from DQ to 3V3. Never connect DQ to 5 V.
3. Connect SW1 between GP15 and GND. Attach only a 3.3 V-compatible SSD1306 module to GP4/GP5/3V3/GND; note current firmware does not drive the display (USB output is authoritative).
4. Inspect for shorts, then power from a current-limited 5 V USB source (100 mA limit sufficient for initial no-display test). Firmware prints CSV header at boot. Press button to toggle measurement.
5. Test probe in room-temperature water, then a stirred ice-water reference; do not immerse cable joints/electronics. Compare against a trusted thermometer. No calibration is provided.

Disconnect if the Pico or wiring heats, USB current exceeds expectations, or liquid approaches electronics. Probe sanitation and temperature limits are the probe maker's responsibility. This is not a food safety instrument.
