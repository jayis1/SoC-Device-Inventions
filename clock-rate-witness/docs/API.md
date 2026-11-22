# API reference

`clock_rate.h` is the portable interval core.

- `cr_init(cr_state_t *s)`: resets count and disarms. Null is ignored.
- `cr_edge(s, timestamp_us, &interval_us)`: first timestamp arms and returns 0; later strictly increasing timestamps return 1 and interval; invalid pointers return -1; nonmonotonic timestamps return -2 without changing state. Caller supplies monotonic microseconds.
- `cr_error_ppm(interval_us, nominal_us)`: signed integer parts-per-million deviation; zero nominal returns zero. Intended for intervals within signed 64-bit range.

The core does not acquire ADC data, debounce optical edges, print USB CDC, or verify calibration; those remain target integration responsibilities.
