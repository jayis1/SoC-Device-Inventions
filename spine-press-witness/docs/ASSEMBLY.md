# Assembly and bring-up

1. Mount the straight-bar load cell so press force acts along its rated axis; stop before its rated load and avoid side load. Use a rigid fixture with a known-safe overload stop.
2. Connect the four bridge wires to the HX711 module's marked E+/E-/A+/A- terminals according to the cell documentation; wire colors are not standardized.
3. With USB disconnected, wire Pico GP14 to DOUT, GP15 to PD_SCK, 3V3 to module VCC and bridge E+, GND to module GND and bridge E-. Confirm the module is a 3.3 V compatible variant. Never attach an unverified 5 V logic output to the Pico.
4. Connect Pico by USB and flash the UF2 from the firmware build instructions.
5. Open the USB CDC serial port. With the platform unloaded, send `t` to tare. Apply a known mass gradually and verify repeatable raw-count direction and return to baseline. Do not infer newtons from raw counts until a controlled calibration is performed.

No board layout is supplied. Provide strain relief for the sensor cable. Do not use the logger as a press interlock or safety control. Physical bring-up has not been performed for this reference design.
