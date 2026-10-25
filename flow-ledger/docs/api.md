# Serial API

Transport: USB CDC serial at 115200 baud, newline-delimited JSON. Device emits one object per accepted falling edge:

```json
{"t_ms":12345,"revolutions":1}
```

`t_ms` is milliseconds since boot and wraps as an unsigned 32-bit value; `revolutions` is the unsigned count since boot. No commands are implemented. Missing sensor and noisy/slow signals are not diagnosed in firmware. The 80 ms edge lockout suppresses rapid repeat edges, but does not establish meter accuracy. Host software must handle device reconnects and reset (count returns to zero).
