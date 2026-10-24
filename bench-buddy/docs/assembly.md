# Assembly and first use

1. Populate two 74HC4051 multiplexers, local 100 nF bypass capacitors, current-limit resistors, and receive pulldowns on a low-voltage prototype board.
2. Connect U2 channel outputs through one 1 kΩ resistor each to the drive fixture; connect U3 channels to the sense fixture. Verify all eight channel numbers and connector pin-1 marks with a continuity meter before inserting a cable.
3. Connect mux VCC to Pico 3V3(OUT), grounds together, and all unused analog inputs to a defined level; do not connect either fixture to external equipment.
4. Flash the UF2 and open the USB serial port at 115200 baud. With no cable, expect an all-zero matrix. Verify a known straight-through cable before testing unknown cables.

This is a reference prototype, not a released PCB. Confirm the selected mux's exact package/pinout and voltage limits before assembly. Firmware thresholds, pull-down loading, and multi-path behavior require bench validation.