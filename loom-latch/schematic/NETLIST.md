# Connection netlist (reference; not KiCad)

| Ref | Pin | Net | Connects to |
|---|---|---|---|
| U1 Pico | VSYS | USB_5V | onboard USB supply only |
| U1 Pico | 3V3(OUT) | SENSOR_3V3 | U2 VCC, U3 VCC, R1.1, R2.1 |
| U1 Pico | GND | GND | U2 GND, U3 GND, C1.2, C2.2 |
| U1 Pico | GP14 | GATE_A | U2 open-collector output, R1.2 |
| U1 Pico | GP15 | GATE_B | U3 open-collector output, R2.2 |
| C1.1 | — | SENSOR_3V3 | U2 VCC local decoupling |
| C2.1 | — | SENSOR_3V3 | U3 VCC local decoupling |

Each output is pulled up to 3V3 through 4.7 kΩ. Select sensors whose output is genuinely open-collector/open-drain and whose supply/output ratings match 3.3 V. Check physical pinout from the selected sensor datasheet; generic BOM entries are not orderable part numbers. Route sensor wiring away from moving loom parts. USB 5 V has no connection to GPIO.
