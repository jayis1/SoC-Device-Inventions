# Serial record format

UART: 115200 baud, 8 data bits, no parity, 1 stop bit, 3.3 V TTL. Each acquisition emits one JSON object terminated by CRLF:

`{"n":12,"force_raw":-1234,"displacement_adc":2048}`

`n` is the monotonically increasing sample counter, `force_raw` is tare-relative HX711 counts, and `displacement_adc` is the raw 12-bit potentiometer ADC code. Values are not converted to SI units until calibration is supplied by the operator. Save one object per line. The current firmware core invokes board adapter hooks; it does not itself implement UART or sensor drivers.
