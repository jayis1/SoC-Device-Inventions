# USB serial API

USB CDC serial, nominal 115200 baud, 8 data bits, no parity, one stop bit. Firmware prints a header followed by one CSV record every 250 ms:

`ms,event,cycles,feeds,alarm`

`ms` is time since boot; counts are cumulative debounced falling edges. `event` is `MISMATCH` when cycles advanced but feeds did not in that sample window, `CYCLE` when cycles advanced without that condition, otherwise `IDLE`. `alarm` is 1 after three consecutive mismatch windows, 0 otherwise. The status LED and piezo follow alarm. This is an operational alert, not a defect diagnosis. USB serial is the only data interface; no network, cloud, or command parser is implemented.
