# Assembly and operating notes

1. Mount the Pico and sensor module on a small carrier or solderless prototype board. Verify the selected TSL2591 breakout accepts 3.3 V before connecting it.
2. Wire to `../schematic/connection-list.txt`; inspect for shorts before USB connection.
3. Mount LED and sensor face-to-face behind a matte-black tunnel with a repeatable film gate and diffuser. Ambient leakage invalidates measurements.
4. Power over USB. Firmware reports sensor detection status but does not yet acquire lux readings; do not interpret this scaffold as a working densitometer.
5. Once acquisition is implemented, apply dark-offset correction, record open-gate reference, then use a calibrated step wedge to characterize response.

Never connect the sensor bus to 5 V logic. This is a prototype wiring guide; no PCB layout or fabrication release is supplied.
