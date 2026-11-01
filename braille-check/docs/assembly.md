# Assembly notes

1. Mount the Pico in an insulated enclosure with USB accessible; do not expose GPIO pads to the specimen.
2. Install HX711 breakout and load cell per their own datasheets. Confirm logic voltage compatibility before connecting to Pico 3V3.
3. Build a rigid guided probe carriage and index fixture for a standard 2×3 cell. Keep probe geometry repeatable and prevent side loading.
4. Use an independent displacement instrument (for example, a calibrated dial indicator) to obtain base and dot-top values in micrometres. The load cell is only a force-trend channel; its output is not a displacement measurement.
5. Verify switch state, USB connection, and any adapter calibration on a known test piece before collecting specimen data.

No calibration procedure or accuracy claim is provided because no physical prototype has been built. Never use the results to certify a sign or replace an accessibility review.
