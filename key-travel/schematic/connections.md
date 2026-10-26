# Schematic-level interconnect reference

Reference design only; no KiCad capture or PCB is supplied. Signal names below are the source of truth pending schematic capture.

| Ref | Pin/signal | Connects to | Notes |
|---|---|---|---|
| U1 Pico | GP4 SDA | U3 AS5600 SDA | I2C0, 100 kHz |
| U1 Pico | GP5 SCL | U3 AS5600 SCL | I2C0, 100 kHz |
| U1 Pico | GP10 input | U2 DOUT | Active-low data-ready |
| U1 Pico | GP11 output | U2 SCK | Idle low; 25 pulses per sample |
| U1 Pico | 3V3/GND | U2 logic, U3 VCC/GND | Check module logic supply requirements |
| U2 | E+/E-/A+/A- | W1 bridge | Follow cell-specific color/pin sheet |
| U1 Pico | USB | Host | USB power and CDC |

Pico internal pull-ups are enabled in firmware as a convenience; external 4.7 kΩ pull-ups may be installed only when the breakout has none. Ensure the aggregate pull-up is suitable for 3.3 V. Do not power or probe unknown HX711 module pins until its supply and pin labels are confirmed.
