# Assembly and bring-up

1. Start with a Pico, 3.3 V-compatible HX711 breakout, 5 g four-wire load cell, and display module. Do not connect a module whose DOUT can reach 5 V.
2. With USB unplugged, wire ground and 3V3, then GP0/GP1 to the display, GP2/GP3 to HX711 DOUT/SCK, and the load cell bridge to E+/E-/A+/A-. Resolve load-cell wire colors from its supplier documentation.
3. Keep the sensing platform mechanically isolated from the enclosure and ensure it cannot bottom out against the load cell. Add a hard overload stop that engages before the cell's rated overload limit.
4. Connect USB and check for shorts, stable Pico 3V3, OLED I2C acknowledgement at 0x3C, and HX711 conversion-ready behavior before placing any weight.
5. Tare with the unloaded platform. Add a known calibration mass at the stylus contact point and calculate counts-per-gram from the change in filtered ADC counts.
6. Repeat loading and unloading several times to identify hysteresis/creep. This reference implementation does not claim a particular accuracy.
7. Verify the platform top is level with the record surface and use a stylus guard or equivalent safe handling. Never lower a cartridge needle onto an unprotected load cell edge.
