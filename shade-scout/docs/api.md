# USB CDC API

Intended records are CSV, one header and then one row per second:

```
angle_deg,lux
123.4,456.7
```

Angle is AS5600 single-turn absolute position in degrees (0 to less than 360); lux is the TSL2591-reported illuminance. Unavailable readings are represented as `nan`; consumers should reject those rows. The current Pico adapter is a scaffold and does not implement sensor acquisition, so it emits `nan,nan`. Baud rate is irrelevant to native USB CDC.
