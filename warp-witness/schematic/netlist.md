# Connectivity specification (not a KiCad schematic)

| Ref | Pin | Net | Connects |
|---|---|---|---|
| U0 Pico | GP2 | ENC_A | J2-A, R1 to 3V3 |
| U0 Pico | GP3 | ENC_B | J2-B, R2 to 3V3 |
| U0 Pico | GP4 | ENC_SW | J2-SW, R3 to 3V3 |
| U0 Pico | GP6 | LED_SER | U1-SER |
| U0 Pico | GP7 | LED_CLK | U1-SRCLK |
| U0 Pico | GP8 | LED_LATCH | U1-RCLK |
| U0 Pico | GP10 | HX_DOUT | U2-DOUT |
| U0 Pico | GP11 | HX_SCK | U2-SCK |
| U0 Pico | GP14 | MOTOR_IN1 | U3-IN1 |
| U0 Pico | GP15 | MOTOR_IN2 | U3-IN2 |
| U0 Pico | 3V3 | VDD_3V3 | U1/U2 VCC; encoder pullups |
| U0 Pico | GND | GND | logic returns, U2 bridge return |
| U3 | VM | MOTOR_5V | J1 regulated 5 V supply |
| U3 | OUT1/OUT2 | MOTOR | M1 terminals |
| LS1 | E+/E−/A+/A− | LOAD_CELL | U2 bridge excitation/input |

Common ground required. Fit 100 nF decoupling at each IC and 47 µF at U3 VM. Add a motor guard and mechanical stop. Confirm package pin numbers against component datasheets before layout. This netlist is not fabrication-ready.
