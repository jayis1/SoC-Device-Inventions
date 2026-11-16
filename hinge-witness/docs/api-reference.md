# Core API

- `hw_session_init(hw_session_t *)`: clear both capture slots.
- `hw_capture(session, label, raw, sensor_ok)`: accept a 12-bit reading for BEFORE or AFTER; rejects null session, failed sensor status, invalid label, or values above 4095.
- `hw_delta_counts(session, &delta)`: requires both valid captures and returns the shortest signed circular delta in encoder counts.

The portable core performs no hardware I/O, scheduling, debounce, I2C fault recovery, or USB serialization. The adapter must only call `hw_capture` after a successful fresh sensor transaction and must report bus errors rather than replay stale values.
