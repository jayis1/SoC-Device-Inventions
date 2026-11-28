# Assembly

1. Populate the Pico headers and mount the board in an insulating enclosure with USB access.
2. Fit the slotted disk to a manually turned spindle. Guard the disk and keep it low-speed.
3. Wire the keyed J1 sensor connector per `../schematic/netlist.md`. Verify sensor supply/output ratings before connection; output must never exceed 3.3 V.
4. Connect the capture button between GP14 and GND. Place C1 beside J1.
5. With USB disconnected, check for shorts between 3V3 and GND. Power through USB only; verify 3V3 at J1 before connecting the sensor.
6. Build and flash firmware as in the device README. Open the USB CDC serial port, enter length, start, rotate the spindle once by hand, and confirm one count. Then validate over 100 turns against a manually counted reference.

No observed bench readings are claimed. This prototype has no designed PCB or enclosure. No device component is connected to a heater, mains circuit, or powered machinery.