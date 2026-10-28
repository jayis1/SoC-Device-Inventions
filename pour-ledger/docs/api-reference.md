# USB serial API

Transport: Pico USB CDC, 115200 baud setting (USB CDC ignores physical baud; terminal preference is conventional). Device emits one comma-separated record per acquired conversion; no header is currently emitted.

`ms,mass_g,flow_g_s,event`

- `ms`: milliseconds since firmware start, unsigned decimal.
- `mass_g`: EMA-filtered estimated mass in grams, two decimals.
- `flow_g_s`: difference of filtered mass samples divided by elapsed seconds; noisy and unvalidated.
- `event`: `SAMPLE`, `STAGE_n`, or `SENSOR_ERROR`.

Tare button: capture current raw value as zero. Mark button: increment stage index and emit `STAGE_n`. The reference firmware does not yet implement OLED rendering or recipe target prompts despite wiring the display; those are future work. HX711 stuck-not-ready timeout yields `SENSOR_ERROR` at most once per 100 ms timeout plus loop delay. No commands are accepted over USB.
