# Assembly notes

1. Confirm the 5 g load cell's full-scale bridge rating and lead colors from its manufacturer documentation; colors are not universal.
2. Wire the bridge to the HX711 module, then power the module from Pico 3V3 and common GND. Keep DOUT/SCK on GP2/GP3. Do not attach the sensor to yarn before confirming stable unloaded readings.
3. Fit the module and wiring to an insulated carrier. Mount the load cell so only its intended axis is loaded. Avoid friction, knots snagging, and contact with enclosure walls.
4. Apply known calibration masses only after the future hardware adapter is implemented. Current release only provides conversion functions; it does not acquire HX711 readings or emit records from a Pico.
5. For first power-up use a current-limited USB source and inspect for shorts. Verify USB data and GPIO levels before connecting any host-specific wiring.

Acceptance criteria for a future prototype: 100 consecutive unloaded readings show no saturation; repeated known-mass readings are monotonic; measured 10 m specimen and repeated cut lengths agree within a declared calibration tolerance. No tolerance is claimed by this concept design.
