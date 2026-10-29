# USB serial API

The RP2040 USB CDC interface is a one-way stream at the Pico USB CDC default. It does not accept commands or require authentication because it exposes no control functions.

- Header: `time_ms,raw,force_n,peak_n`
- Valid sample: monotonically increasing milliseconds since boot, signed ADC count, force in newtons, and session peak.
- Uncalibrated sample: `time_ms,raw,NA,NA`.
- Fault: `ERROR,hx711_timeout` when data-ready does not assert within 500 ms.

Capture the session as text/CSV. `raw` is an uncorrected 24-bit HX711 code; force is valid only after calibration. No framing checksum is included; detect truncation or malformed records in host tooling.
