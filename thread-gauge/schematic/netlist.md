# Electrical netlist reference (not a KiCad schematic)

Use a Raspberry Pi Pico / Pico H module with onboard USB, flash, and regulator. Component reference designators are in `hardware/BOM.csv`.

| Ref | Pin | Net | Connection |
|---|---|---|---|
| U1 | GP2 | HX_DOUT | U2 DOUT |
| U1 | GP3 | HX_SCK | U2 SCK |
| U1 | GP14 | BTN_TARE_N | SW1 to GND, internal pull-up |
| U1 | GP15 | BTN_RECORD_N | SW2 to GND, internal pull-up |
| U1 | 3V3(OUT) | 3V3 | U2 VCC; local 100 nF bypass |
| U1 | GND | GND | U2 GND, SW1/SW2 ground |
| U2 | E+, E-, A+, A- | BRIDGE | 4-wire 5 g load cell, match its datasheet colors |
| U1 | USB | USB_5V | USB connector / host supply |

The HX711 is powered at 3.3 V to keep its digital output within Pico GPIO limits. Do not power the module from 5 V. Keep the load-cell bridge wiring short and twisted; add shielding only at the instrument end. There is no KiCad file or PCB in this concept release.
