USB CDC emits `elapsed_s,temp_c` at boot. During a run, rows are emitted approximately once per second; sensor faults produce `#sensor_error,<elapsed_s>` and are not converted into numeric temperatures. `#run,start` and `#run,stop` mark button toggles. Temperatures are Celsius. Host scripts should discard comment records and validate field counts.

The OLED connector is reserved in this design reference; current firmware does not initialize or drive an OLED. No network interface or persistent device-side storage is implemented.
