# Connection-level netlist (concept only)

Not a KiCad schematic; package pad assignments and circuit are not validated for fabrication.

| Net | Connections |
|---|---|
| USB_5V | J1 VBUS, U2 VIN, C2. J1 is USB-C receptacle with 5.1 kΩ CC1/CC2 pulldowns to GND. |
| +3V3 | U2 VOUT, U1 VDD, C1, C3; U2 is TLV70033DBVR. |
| GND | J1 GND, U1 VSS, U2 GND, C1/C2/C3 ground, UART header GND, J2 contact return. |
| UART_TX | U1 PD1 to J3 TX pad (connect to adapter RX). |
| UART_RX | J3 RX pad (connect to adapter TX) to U1 PD6. |
| TEST | J2 signal to R1 1 kΩ to U1 PC4; D1 low-capacitance 3.3 V TVS from PC4-side of R1 to GND. |
| CC1 | J1 CC1 through R2 5.1 kΩ to GND. |
| CC2 | J1 CC2 through R3 5.1 kΩ to GND. |

U1: CH32V003F4P6 SOP-8. Confirm pins with WCH package drawing before layout. External UART adapter must be 3.3 V logic and separately host-powered. C1 100 nF at MCU, C3 4.7 µF on 3.3 V; C2 1 µF at LDO input. D1 part is TBD; select a low-capacitance ESD clamp with standoff appropriate to 3.3 V before fabrication.
