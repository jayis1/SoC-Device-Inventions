# CSV interface

Firmware emits a header `ms,angle_deg,raw_adc,force_n`; `force_n` is intentionally blank until calibrated. `ms` is monotonic milliseconds since boot. `angle_deg` is encoder ticks multiplied by a placeholder 0.25°/tick; calibrate this against the actual encoder and lever zero before use. `raw_adc` is the signed 24-bit HX711 channel-A reading. Timeout rows begin with `#` and are ignored by the converter. Host helper: `python3 scripts/convert.py capture.csv --counts-per-newton <calibrated-value>`.
