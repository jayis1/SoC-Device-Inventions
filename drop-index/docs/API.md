# USB serial output

USB CDC enumerates as a serial port. At boot the device prints a banner. Pressing the active-low GP15 button starts one 60-second capture. Header: `elapsed_s,mass_g,rate_g_per_min`; each following row contains elapsed seconds, accumulated mass relative to the startup tare, and interval-derived rate. An HX711 conversion timeout emits `ERROR,HX711 timeout` and ends that capture. Values use a compile-time counts-per-gram factor and are not trustworthy until calibrated.

There is no command parser and no inbound host protocol. The host should reject malformed/non-numeric rows and treat the banner/error as non-data. Do not use the serial output as a billing or safety instrument.
