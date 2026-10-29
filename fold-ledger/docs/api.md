# USB CDC event API

The firmware emits one UTF-8 JSON object and newline per event. `ms` is monotonic milliseconds since boot (not wall-clock time); `type` is one of `foot`, `optical`, or `batch_start`.

Example:

```json
{"ms":1432,"type":"foot"}
{"ms":8000,"type":"optical"}
{"ms":14300,"type":"batch_start"}
```

Foot events are emitted on release after a debounced press shorter than two seconds. A press held for at least two seconds produces `batch_start` and suppresses a `foot` event on release. Optical input is emitted on its debounced active edge. Host software should tolerate reconnects, partial lines, and millisecond counter wrap; start a fresh file per boot. There is no command channel or persistent storage.
