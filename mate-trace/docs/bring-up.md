# Bring-up and data reference

1. Confirm the exact HX711 breakout supports 3.3 V supply and logic, and verify load-cell bridge labels from its datasheet.
2. With USB disconnected, inspect wiring and continuity. Power Pico alone, then confirm 3.3 V before connecting the ADC module.
3. Confirm DOUT idles and switches only between ground and a maximum of 3.3 V. No HX711 hardware driver is included in this reference firmware.
4. Mechanically align the sensor with the connector axis; fit a travel stop within the load-cell and fixture rating.
5. Calibration is not supplied: apply known axial loads, log raw counts, derive a scale and zero offset, and document uncertainty before interpreting force.

The portable core API is in `../firmware/src/mate_trace.h`: initialize state, start with a nonzero run ID, submit signed raw samples, then finish after at least one sample. `INT32_MIN` denotes adapter-detected invalid input. Counts are absolute peak magnitude; adapter code must also detect HX711 timeout, saturation, and disconnected-cell conditions. Suggested CDC record: `run_id,peak_counts,sample_count`. No target adapter currently emits this record.

Never exceed the load-cell rating, use the fixture for certification, or place fingers in the moving jaw path. This design is not physically tested.
