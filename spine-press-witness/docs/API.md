# USB CDC text protocol

Firmware revision: initial reference. Native USB CDC, one newline-terminated record per sample; no network interface or remote command path.

- `t` — tare from the next successfully read sample. The initial successful sample is the startup offset.
- Success record: `milliseconds_since_boot,raw_signed_counts,relative_counts`
- Failure record: `sensor_error` when HX711 DOUT does not become ready within 100 ms.
- Sampling interval: 100 ms nominal. USB baud setting is ignored by native CDC.

The relative count is raw ADC counts minus the tare offset; it is not calibrated mass or force. No protocol command controls mechanical equipment.
