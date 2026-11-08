# Serial API

USB CDC emits one UTF-8 line per nominal one-second sample. The initial `#` lines are comments and describe columns. CSV fields are `t_ms,angle_raw,position_mm,temp_C,rh_pct,status`; timestamps are milliseconds since MCU boot. `angle_raw` is the 12-bit AS5600 angle (0–4095). `position_mm` is relative to the current zero and uses configured pulley circumference. Temperature/RH are raw SHT31 conversions without calibration; blank fields indicate a failed environment read. `status` is `OK`, `ERR_SHT31`, or `ERR_AS5600`.

Single-character commands: `z` sets the current angle to zero when sensor access succeeds; `h` prints help. Commands are read opportunistically between samples. No network service or stored credentials exist. The angle-to-position mapping assumes carriage travel stays within ±half a sensor revolution from zero; larger travel needs a multi-turn encoder or validated unwrap algorithm.
