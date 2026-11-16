# Assembly and calibration notes

This prototype is low-voltage and USB-powered. Disconnect USB before wiring. Mount the Pico and sensor breakout on a carrier or breadboard; connect common ground and the 3.3 V rail only to a breakout whose logic-level compatibility is confirmed. Inspect whether the breakout already has I2C pull-ups before fitting R2/R3.

Attach a diametrically magnetized magnet to a rigid moving arm coupled to the door, centered above the AS5600 package. Keep the sensor/magnet gap within the sensor module's documented operating geometry; verify smooth angle changes before taking comparisons. The fixture must not interfere with the hinge or pinch points. Capture BEFORE and AFTER at the same marked door position, with the door supported normally. No absolute alignment threshold is provided; interpret the difference as a repeatability aid only.

First-power checks: verify no short between 3V3 and GND, then power over USB and measure 3.3 V at the sensor. Confirm USB CDC enumerates and that angle readings are in range. Hardware integration firmware is not yet included, so these steps are design guidance rather than completed test results.
