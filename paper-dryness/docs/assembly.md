# Bring-up and assembly

1. Assemble Pico, MCP3008, and optional OLED on a breadboard; check orientation and common ground.
2. Verify 3.3 V between VDD and GND before inserting U2.
3. Build with Pico SDK 2.x using commands in the device README and flash the UF2.
4. Confirm serial output has `sample,adc_raw` then monotonically increasing sample numbers. Inputs are diagnostic ADC values, not moisture percentages.
5. Keep the copper plates parallel with a rigid nonconductive spacer and place the sheet centrally. Record repeated readings and compare only with same paper, geometry, and environmental conditions.
