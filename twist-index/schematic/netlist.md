# Connector-level netlist reference (not KiCad)

U1 is a Raspberry Pi Pico development board. J1 is a 3-pin photogate connector, J2 is a normally-open capture switch. All logic is 3.3 V.

| Ref | Pin | Net | Connects |
|---|---|---|---|
| U1 | pin 20 / GP15 | GATE_OUT | J1 pin 3, sensor open-collector/push-pull output |
| U1 | pin 19 / GP14 | CAPTURE_N | J2 pin 1 |
| U1 | pin 36 / 3V3_OUT | 3V3 | J1 pin 1, C1 pin 1 |
| U1 | pin 38 / GND | GND | J1 pin 2, J2 pin 2, C1 pin 2 |
| C1 | 100 nF | 3V3/GND | local decoupling at J1 |

J1 pin order viewed into the keyed connector: 1=3V3, 2=GND, 3=OUT. Fit keyed shroud and label orientation. The selected sensor must tolerate 3.3 V supply and output logic; do not connect a 5 V output. Pico pin numbers refer to Raspberry Pi Pico, not Pico W.

Power: USB 5 V powers Pico; onboard regulator supplies sensor from 3V3. No external power input. No reverse-polarity or surge protection is needed for the keyed low-voltage sensor connector; disconnect USB before wiring.