# USB serial and calibration protocol

The target USB CDC application is not yet implemented. The portable firmware library accepts signed raw HX711 counts and converts them using `ss_grams(raw, zero, counts_per_gram)`. Host-side target integration should expose one reading per line as JSON, e.g. `{"raw":12345,"grams":1.25,"valid":true}`; that is a proposed wire format, not current firmware behavior.

Calibration procedure: record unloaded median count as `zero`; place a known mass in grams on the platform and record `loaded`; set `counts_per_gram=(loaded-zero)/mass`. Reject zero or negative coefficients and repeat with a second mass to check linearity. Store calibration only after bounds and checksum validation in the future target application.

The included C core has no transport parser, display driver, tare storage, or command protocol; those functions must be added and tested as part of Pico SDK integration.
