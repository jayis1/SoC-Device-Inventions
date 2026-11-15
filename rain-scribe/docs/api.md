# Firmware core API

Portable implementation: `firmware/rain_scribe.c`.

- `rain_scribe_start(ms)`: begin a session; emits BEGIN and resets duplicate-level suppression.
- `rain_scribe_level(ms, level)`: while active, emits a LEVEL record for levels 0–4 when the value changes. Invalid values and repeated levels are ignored.
- `rain_scribe_mark(ms)`: while active, emits a MARK record.
- `rain_scribe_end(ms)`: emits END once and closes the session.

The application adapter must supply monotonically wrapping `uint32_t` millisecond ticks, debounce reed/button inputs, map the active switch to level, and route stdout to USB CDC. The API is synchronous and not thread-safe. Elapsed time uses unsigned subtraction and therefore supports ordinary 32-bit tick wraparound. No persistence is provided by the core.
