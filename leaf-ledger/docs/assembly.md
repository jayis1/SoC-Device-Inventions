# Assembly and calibration

1. Mount two quadrature encoders orthogonally in a flat XY carriage; couple the blunt stylus to the carriage without excessive friction. This mechanical fixture is a prototype and is not included in the electronics BOM.
2. Connect each encoder's A/B and common to GP2/GP3 or GP4/GP5 and GND. Connect encoder supply to Pico 3V3 only. Connect normally-open trace switch from GP6 to GND. Check wiring continuity before USB connection.
3. Flash firmware using the README procedure. Open the board's USB CDC port. Move the stylus and verify coordinate changes indirectly through a completed trace; firmware reports area only.
4. Calibrate separately by tracing a rectangle of known width and height. For each axis, record encoder counts over a measured straight-line distance and calculate counts/mm. Repeat in both directions to estimate backlash; do not silently assume symmetric scale.
5. Compute physical area from `area_counts2 / (x_counts_per_mm * y_counts_per_mm)`. The device does not store calibration or claim absolute accuracy.

Troubleshooting: no serial output—check CDC selection and USB cable; zero area—trace a closed contour with at least three distinct points; reversed axis—swap that encoder's A/B; noisy counts—shorten leads, improve ground routing, and only then consider external filtering. Test findings must be added only after actual bench work.
