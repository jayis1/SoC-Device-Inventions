# Serial API

USB CDC at 115200 baud. On boot: `millis,raw_force,angle_raw`. Pressing GP6 toggles capture and emits `#START` or `#END`. During capture, each line is CSV: unsigned milliseconds since boot, signed HX711 raw count, 12-bit AS5600 angle (0–4095). I2C failure emits `#SENSOR_ERROR`; HX711-not-ready currently emits raw zero, so this is ambiguous and should be excluded from calibrated data. Values are uncalibrated; firmware does not produce force or displacement units.
