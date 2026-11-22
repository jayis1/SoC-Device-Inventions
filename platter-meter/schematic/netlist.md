# Platter Meter connection-level schematic (Pico module)

This is an explicit wiring netlist, not a KiCad CAD schematic or PCB layout. The Pico is a module; use its published board pinout. Reference IDs correspond to BOM.

| Ref | Pin | Net | Connection |
|---|---|---|---|
| J1 Raspberry Pi Pico | VSYS/USB | +5V_USB | USB host supplies the Pico; no external supply |
| J1 | 3V3(OUT) | +3V3 | R1 top, J2 VCC |
| J1 | GND | GND | J2 GND |
| J1 | GP2 | SENSOR_N | J2 OUT and R1 bottom |
| J1 | GP25 | LED_STATUS | onboard LED, firmware controlled |
| J2 TCRT5000 module | VCC | +3V3 | Powered at 3.3 V only |
| J2 | GND | GND | Common ground |
| J2 | OUT | SENSOR_N | Open-collector output; verify module behavior before use |
| R1 10 kΩ | 1 | +3V3 | Sensor pull-up |
| R1 | 2 | SENSOR_N | GP2 input |

Fit a 100 nF bypass capacitor directly across J2 VCC/GND if the selected module lacks local decoupling. Do not use a 5 V-powered module output directly on GP2. Before connection, confirm output high is at or below 3.3 V with a meter. Configure input with falling-edge interrupt; debounce/ignore edges less than 500 ms apart. No mains or audio connections.