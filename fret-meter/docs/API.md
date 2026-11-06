# USB CDC API

Each debounced sample emits one JSON line: `{"raw":2048,"relative_mm":5.002}`. `raw` is the arithmetic mean of 16 12-bit ADC samples. `relative_mm` uses firmware placeholder endpoints 0 and 10 mm; replace these with measured calibration endpoints before treating the value as a length. No command parser or persistent calibration store is implemented. Disconnecting USB stops logging; readings are not retained.
