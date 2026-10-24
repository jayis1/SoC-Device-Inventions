# Firmware/core interface (prototype)

The portable core currently defines a debounced Hall event and count/undo operations. It has no storage, display, or encoder driver; those integrations are future work and are not represented as implemented.

Intended local UI mapping for a future adapter:
- one accepted Hall falling edge increments count;
- encoder rotation selects target row count;
- short encoder press undoes the most recent count;
- long press resets only after a confirmation screen.

Planned USB CDC diagnostic format (not implemented): `ROW,<uint32_count>,<uptime_ms>\n`. A host parser must reject malformed or out-of-range fields. No network service or remote control is provided. Counts are volatile in this prototype.
