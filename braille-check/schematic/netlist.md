# Connection plan (not a KiCad schematic)

This text netlist is authoritative for the reference prototype; it is not a validated schematic or PCB.

- Pico VSYS/USB: USB 5 V through onboard module; Pico GND common.
- Pico 3V3(OUT) -> HX711 VCC (only if breakout specifies 3.3 V-compatible logic); GND -> HX711 GND.
- GP16 <- HX711 DOUT; GP17 -> HX711 SCK.
- Load-cell red/black -> HX711 E+/E- and white/green -> A+/A- only after confirming the actual sensor lead colors against its datasheet; colors are not guaranteed.
- GP14, GP15 each connect to a normally-open switch to GND (internal pull-ups).
- GP25 onboard LED.
- USB CDC via Pico USB connector.

The load cell measures probe force, not displacement. A calibrated mechanical height comparator must provide the micrometre readings to the portable firmware core; no HX711-to-height conversion is included. Avoid external voltage on GPIO. Verify breakout pinout and bridge wiring before power-up.
