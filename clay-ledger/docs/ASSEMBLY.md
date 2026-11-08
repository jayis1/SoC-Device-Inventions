# Assembly and first power

1. Build the low-voltage electronics on a dry bench. Mount the Pico and sensor breakouts so they cannot contact clay, water, or cleaning liquids. Fit the carriage, pulley, and magnet; mechanically guard pinch points.
2. With USB unplugged, inspect wiring against `../schematic/netlist.md`. Confirm 3V3-to-VCC and common ground. Confirm neither breakout sources 5 V pull-ups onto SDA/SCL. Fit R1/R2 only when no module already supplies pull-ups.
3. Check for shorts between 3V3 and GND. Connect to a current-limited 5 V USB supply (limit 250 mA for initial bring-up). Stop if current exceeds the expected low tens of mA, parts heat, or the board resets.
4. Flash the UF2 using BOOTSEL. Open a USB CDC terminal at 115200 baud (USB CDC ignores baud setting on most hosts). Verify header, sensor readings, and error behavior with each sensor disconnected in turn.
5. Calibrate fixture travel: mark a known carriage displacement using a ruler or gauge block, adjust `PULLEY_CIRCUMFERENCE_MM` in `firmware/src/config.h` for the installed pulley, rebuild, then repeat in both directions to expose backlash. This is a comparative calibration, not traceable metrology.
6. Mount a ceramic test bar with repeatable, gentle end contact. Send `z` to zero. Keep electronics out of the wet zone. Export terminal output to a CSV file.

Troubleshooting: `ERR_AS5600` means position sensor not responding at 0x36; check magnet/sensor alignment and I2C wiring. `ERR_SHT31` means environmental sensor not responding at 0x44. An angle jump near wraparound should remain continuous modulo one turn; a full pulley revolution wraps displacement and requires a carriage geometry that prevents exceeding sensor range. If readings jitter, shorten wiring, check pull-ups, and reduce vibration. No enclosure IP rating, humidity-chamber qualification, or physical test is claimed.
