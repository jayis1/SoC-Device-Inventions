# Connection schedule (reference, not KiCad CAD)

| From | To | Net / condition |
|---|---|---|
| 3V3 | HX711 VCC, pot high end, MCU 3V3 | common regulated rail |
| GND | HX711 GND, load-cell E-, pot low end, buttons, MCU GND | common ground |
| Load cell E+/A+/A-/E- | HX711 E+/A+/A-/E- | Match cell datasheet colors; do not assume color code |
| HX711 DOUT/SCK | PC1/PC2 | 3.3 V only |
| Pot wiper | PA1 | 0–3.3 V ADC range |
| Button contacts | PC3/PC4 to GND | internal pull-ups |
| PD5 UART TX | USB-UART RX | 3.3 V adapter; common GND |
| USB 5 V | development board USB input | board regulator supplies 3.3 V |

Do not attach a 5 V UART adapter or drive MCU pins above 3.3 V. Select GPIO by board documentation; verify board routing and package mapping before assembly.
