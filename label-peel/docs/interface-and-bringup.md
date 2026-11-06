# Interface and bring-up notes

This is a module-level reference, not a PCB schematic. Net connections are explicit below; no fabrication files are supplied.

| Ref | Pin | Net | Destination |
|---|---|---|---|
| Pico U1 | GP2 | HX_DOUT | HX711 U2 DOUT |
| Pico U1 | GP3 | HX_SCK | HX711 U2 SCK |
| Pico U1 | GP14 | FOOT_N | SW1 to GND; firmware internal pull-up |
| Pico U1 | 3V3(OUT) | +3V3 | U2 VCC only if verified compatible |
| Pico U1 | GND | GND | U2 GND, load-cell return, SW1 return |
| U2 | E+/E-/A+/A- | bridge | LS1, exact mapping from cell data sheet |
| U1 | USB | USB_VBUS/D+/D- | host, via Pico board connector |

Bring-up: initially leave the cell disconnected and check U2 logic supply and GPIO levels. Power the completed low-voltage assembly from a current-limited USB source; inspect for heating, verify GP3 idles low, and confirm HX_DOUT eventually falls low when conversion completes. A timeout record indicates disconnected, unpowered, or incompatible converter wiring. Attach the cell only after its bridge pinout is identified. Apply a small known load by hand, verify polarity, remove load, and establish zero/span calibration before any force conversion. Stop if the fixture binds or overloads the cell.
