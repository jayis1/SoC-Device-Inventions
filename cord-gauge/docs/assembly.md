# Assembly and calibration

1. Mount the load cell rigidly as a series force element between frame anchor and specimen clamp. Follow the cell datasheet mounting orientation and do not exceed the lower of cell rating and fixture rating.
2. Couple the carriage to the linear-potentiometer slider without side loading. Add mechanical stops so the slider cannot exceed its travel.
3. Wire the module per `../schematic/netlist.md`; verify 3.3 V logic before connecting MCU pins.
4. Power through the development board USB connector. Check 3.3 V rail and UART at 115200 8N1.
5. With no specimen, tare. Calibrate force with traceable known masses at several points; compare potentiometer readings to a ruler over the usable stroke.
6. Install a specimen at a recorded gauge length; apply only gentle, slow hand force and record force/displacement pairs. Repeat at least three times for a comparative result.

No calibrated force or extension accuracy is guaranteed by this reference design.
