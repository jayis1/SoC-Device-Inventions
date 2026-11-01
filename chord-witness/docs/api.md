# Firmware core API

`cw_estimate_frequency(samples, count, sample_rate_hz, midpoint, min_peak_to_peak)` returns a fundamental estimate in Hz or 0 when the buffer is invalid, too quiet, or outside the 40–1500 Hz accepted range. `samples` are 12-bit ADC codes. The function performs bounded loops and no allocation. It is a simple rising-crossing period estimator, not a robust polyphonic pitch detector.

Host build: `cmake -S firmware -B build && cmake --build build`. This produces a portable static library; Pico ADC sampling, USB CDC, note-name conversion, and hardware integration are intentionally left to a Pico SDK application.
