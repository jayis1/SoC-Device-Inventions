# USB CDC data protocol

Firmware prints a header then one record every 100 ms:

```text
sample_ms,angle_raw,force_raw,direction
1234,2010,812300,1
1334,2012,812440,1
1434,INVALID,INVALID,1
```

- `sample_ms`: milliseconds since RP2040 boot.
- `angle_raw`: AS5600 12-bit RAW ANGLE register (0–4095); `INVALID` means I2C transaction failed.
- `force_raw`: signed 24-bit HX711 ADC count; `INVALID` means conversion was not ready within 100 ms.
- `direction`: 1 increasing angle, -1 decreasing angle, 0 not yet established.

The protocol deliberately provides raw values only. Host software should unwrap angle modulo 4096, apply the measured screw pitch and calibration constant, and reject INVALID records. Do not treat the count as newtons until calibrated against traceable masses. USB CDC is local wired output; there is no network service or remote-control command channel.
