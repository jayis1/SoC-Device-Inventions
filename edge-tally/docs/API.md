# Core API

`edge_tally_init(state, initial_high, now_ms)` initializes the state. Call `edge_tally_sample(state, contact_high, now_ms)` periodically with monotonic millisecond ticks. A stable high-to-low transition increments the saturating 32-bit count after 25 ms debounce. Unsigned subtraction permits tick wraparound. This module performs no GPIO, USB, or storage operations.
