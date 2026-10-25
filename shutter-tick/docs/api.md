# USB CDC output

One compact JSON object is written per recognized light pulse, terminated by LF:

`{"duration_us":12345,"valid":true}`

`duration_us` is the time between the first polled illuminated sample and the first polled dark sample. It is not a calibrated exposure measurement; polling latency and optical threshold shift are uncharacterized. A one-second timeout reports `{"valid":false,"error":"pulse_timeout"}`. Host capture tools should ignore records with `valid:false` and preserve raw integer microseconds.

No commands are accepted from the host. USB CDC is a local output channel; there is no network interface.
