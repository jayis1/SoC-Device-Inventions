# Serial API

USB CDC, 115200 baud. Each boot emits `sample,adc_raw`, followed every 250 ms by a decimal sample counter and MCP3008 channel-0 reading (0–1023). No commands are accepted. Values are raw counts and are not calibrated humidity measurements.
