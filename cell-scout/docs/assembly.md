# Assembly and bring-up

1. Populate the Pico headers, divider, load resistor, gate resistor and pulldown; verify Q1 pinout from its datasheet.
2. With no coin cell installed, power only the Pico over USB and confirm GP15 is low at reset. Do not connect the load until this is confirmed.
3. With firmware flashed, issue `t` without a cell: readings should be near zero; do not interpret output as valid.
4. Insert one correctly oriented CR2032. Open-circuit voltage should be within the cell manufacturer's stated range. Confirm the measured voltage using a multimeter.
5. Issue `t`; check that loaded voltage is below open voltage and recovery is near open voltage. If readings disagree materially, disconnect and inspect wiring/divider calibration.

The prototype has no hardware reverse-polarity protection, fuse, or calibrated current measurement. Do not leave it unattended or use damaged cells.
