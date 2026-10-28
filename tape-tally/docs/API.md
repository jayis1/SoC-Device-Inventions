# Event API

`tt_sample(level, now_ms, &event)` receives comparator state and monotonic millisecond time. It emits a pulse on initial/high samples separated by at least 20 ms, then emits one stall event once the gap reaches 500 ms. `TT_PULSE.value_ms` is the timestamp; `TT_STALL.value_ms` is elapsed gap. Caller owns USB formatting and sampling schedule. Use unsigned wrap-safe time arithmetic; sample at 1 ms for the stated timing target. This function is a small event core, not a complete USB firmware application.
