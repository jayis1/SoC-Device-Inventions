# USB CDC format

Startup lines: `status,sky_glow_v1` and `ms,ch0,ch1,status`. Rows contain elapsed milliseconds, TSL2591 channel 0/1 raw unsigned counts and `ok`, `sensor_error`, or `saturated`. No photometric conversion or command parser is implemented; gain/integration metadata and empirical calibration would be required for absolute values.
