# Serial interface (planned)

USB CDC at 115200 baud. Current scaffold emits a startup banner and sensor status lines:

- `slide_scout,1`
- `status,not_implemented`
- `status,sensor_detected_register_reader_pending` or `status,sensor_error,rc=<n>,id=<n>`

Future measurement record format: `sample,<milliseconds>,<reference_counts>,<sample_counts>,<optical_density>`. No calibration write protocol is implemented. Counts must be dark-offset corrected, finite, and positive; invalid/below-floor readings must be rejected.
