# API specification (planned; not implemented)

No USB CDC transport exists in this prototype yet. Intended line commands: `HELP`, `STATUS`, `ZERO`, `SET_LENGTH_MM,n`, `START`, `STOP`. Planned CSV result: `sample_id,length_mm,mass_mg,turns,tex_milli,turns_per_milli`.

A future command parser must reject malformed/out-of-range fields, bound input lines, never start the motor on unknown commands, and keep outputs low after reset or any sensor/driver error. This file describes an interface proposal, not a functioning endpoint.
