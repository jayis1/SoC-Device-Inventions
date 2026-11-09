# Assembly and bring-up

1. Confirm the sensor breakout accepts 3.3 V on VIN and that SDA/SCL pull-ups terminate at 3.3 V. Do not use a 5 V pull-up module.
2. Connect Pico GP4 to SDA, GP5 to SCL, 3V3(OUT) to VIN, and GND to GND. Inspect polarity before USB connection.
3. Fit the sensor in a matte-black shroud aimed consistently at the vessel through a fixed viewing aperture. Keep electronics above/outside splash risk. No board or housing has been qualified for chemical exposure.
4. Build and flash following the device README. Open CDC and confirm CSV header or `error,sensor_missing` diagnostic. A detected I2C ID does not prove optical accuracy.
5. With a stable vessel and lighting, collect blank readings; record volume at each manual burette reading using `volume,<mL>`. Take color readings with `sample,<label>` and operator annotations with `mark,<label>`.
6. Compare readings against a known procedural reference. This design has no calibration model and must not replace validated analytical methods.
