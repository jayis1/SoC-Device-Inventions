# USB CDC protocol

Line-oriented ASCII at 115200 baud; LF terminates commands; CR ignored. Maximum command length 95 printable characters.

- `sample,<label>` label: 1–39 ASCII letters/digits/underscore/hyphen. Emits elapsed milliseconds and raw clear/red/green/blue counts.
- `volume,<mL>` numeric reading in [0,100], decimal point permitted. This is manually read from the burette, not measured by the device.
- `mark,<label>` emits observer annotation, e.g. `blank`, `endpoint`, `sample`.
- Invalid lines emit `error,invalid_command`; overlong lines emit `error,line_too_long`.

Records are ephemeral USB serial output; firmware does not write persistent storage. Host software should save the CSV stream. Time is uptime since device start, not wall-clock time. Sensor read failure emits an error and no color sample record.
