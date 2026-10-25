# USB CDC JSONL API

The Pico enumerates as a USB serial device. Each line is one UTF-8 JSON object. Firmware emits `status`, `angle_deg`; after zero and span calibration it also emits `fraction` and `value` (fraction multiplied by `FULL_SCALE`, default 100). Angle is quantized to 22.5° sensor sectors by the current minimal firmware. No command parser or persistent calibration is implemented; calibration is volatile and is performed with the ZERO and SPAN switches. No pressure units are inferred by the device.

Design documentation by jayis1; hardware and measurement performance remain unverified.
