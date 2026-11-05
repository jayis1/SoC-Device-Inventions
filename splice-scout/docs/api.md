# CSV interface

USB CDC header: `uptime_ms,event,adc_raw`. Events are `mark` and `index`; one line is emitted per debounced press. `uptime_ms` is milliseconds since boot; `adc_raw` is RP2040 12-bit ADC output sampled at button recognition. No thresholding, calibration, persistence, or network API is implemented. Host software should treat records as observations, not a splice verdict.