# API and CSV

The portable function `beat_window_crossing(state, timestamp_us, &interval_us)` returns false on the first edge (baseline) and true on subsequent edges. `timestamp_us` must be monotonic modulo unsigned 64-bit wrap. Interval is unsigned microseconds between successive events. Null state/output pointers return false.

A target application should emit one CSV row per returned interval, e.g. `event,interval_us` followed by a monotonically increasing event index and interval. ADC threshold, hysteresis, edge filtering and USB CDC transport are target responsibilities and are not supplied here.
