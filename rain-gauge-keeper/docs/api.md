# USB CDC stream

At startup firmware prints:

```text
rain-gauge-keeper,format=TIP,<uptime_ms>
```

For each debounced falling edge it prints `TIP,<uptime_ms>` where uptime is milliseconds since boot, unsigned decimal. The UART is disabled; connect by USB CDC at any nominal host baud setting. This prototype does not store events to microSD; reset loses its time origin and no absolute date is recorded. A future persistence implementation should append monotonic event records and supply a host-set UTC epoch if calendar timestamps are required.

The stream is informational only. Missing records can result from USB disconnect, power loss, or firmware faults. Treat the serial log as volatile until the storage implementation is added.
