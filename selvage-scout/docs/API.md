# Core API

`scout_init(&state)` resets a session. `scout_start(&state, now_ms)` starts a numbered run and clears its event count. `scout_sample(&state, adc, threshold, now_ms)` returns 1 for each rising threshold event after a 20 ms refractory period; it returns 0 otherwise. ADC is unsigned 12-bit. Threshold is caller-selected 0–4095. Unsigned millisecond subtraction handles timer wrap. A low sample rearms the detector. The core does not perform ADC, button handling, persistence, or JSON output.

A future USB adapter should emit one JSONL record per accepted event with `test`, `events`, and `elapsed_ms`; output is not implemented in this host-core release.
