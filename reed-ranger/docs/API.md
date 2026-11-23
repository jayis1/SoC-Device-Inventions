# USB CDC JSONL API

Each accepted active-low closure while capture is enabled emits one line:

`{"event":"closure","count":3,"closed_ms":42,"interval_ms":817}`

`interval_ms` is null for the first event. `closed_ms` measures switch closure duration; `interval_ms` is the time from the previous accepted closure. Values are unsigned milliseconds since boot. No command protocol or persistent storage is implemented.
