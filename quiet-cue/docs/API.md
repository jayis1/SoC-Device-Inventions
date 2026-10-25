# Firmware core API

`qc_clamp_minutes(minutes)` returns a supported countdown duration in the inclusive range 1–120. `qc_led_count(elapsed_seconds, duration_minutes)` returns remaining illuminated segments (0–10), reaching zero at or after expiry. The functions are platform-independent; GPIO, encoder debounce, USB CDC protocol, and persistent settings are not implemented in this core.
