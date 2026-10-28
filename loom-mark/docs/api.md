# USB CDC event format

Each confirmed ordered A-then-B crossing emits one newline-terminated CSV row:

`pick_number,uptime_ms`

The stream has no header; connect at the USB CDC device's enumerated port (baud setting is ignored by USB CDC). `pick_number` starts at 1 per power-up. `uptime_ms` is milliseconds since boot as an unsigned 32-bit counter. The firmware ignores events until A is interrupted, accepts B within one second, and enforces 35 ms between accepted transitions. Host software should preserve raw lines and identify device reconnection separately; there is no persistent storage or authentication.
