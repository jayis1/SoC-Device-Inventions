# USB CDC JSONL interface (planned host integration)

The target application layer is not yet implemented; this document defines the intended transport contract. Each output line is one UTF-8 JSON object followed by LF. No network service or credentials are used.

Example record:
```json
{"v":1,"ms":1200,"feed_mm":1.25,"spool_edges":1,"state":"ok"}
```

Fields: `v` protocol version (currently 1); `ms` uptime milliseconds; `feed_mm` signed feed movement since previous sample; `spool_edges` unsigned change in optical edge count; `state` is `ok`, `sensor_fault`, or `motion_disagreement`. A `sensor_fault` record must use null readings rather than stale values. Configure the host serial device at 115200 baud (USB CDC baud is advisory). One request per line: `HELLO` returns firmware/version; `ZERO` clears accumulated counters. Unknown or oversized requests are rejected; proposed max line length 64 bytes. These commands are specifications, not implemented behavior in the portable algorithm core.
