# Firmware core API

`crease_stop_record(pressed, adc_raw, event)` returns true exactly once per observed rising press state, filling an incrementing sequence number and the raw 12-bit ADC count. A false `pressed` sample rearms the edge detector. Invalid pointers and counts above 4095 return false without producing an event.

The function is intentionally single-threaded and stores edge/sequence state internally. Debouncing, ADC conversion, USB CDC serialization, persistence, calibration, and Pico GPIO initialization belong to the target adapter and are not implemented here.

CSV transport convention proposed by the device README: `event,sequence,position_raw,button_state`, followed by rows whose `event` is `press` and `button_state` is `1`.
