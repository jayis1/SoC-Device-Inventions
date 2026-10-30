# Connection netlist (not KiCad)

Reference design only; capture and ERC remain pending. The labeled interface wiring is in `../hardware/pinmap.csv`. Use a 3.3 V-compatible HX711 breakout, verify its terminal labels, and keep the load-cell bridge wiring short. Place 100 nF between module 3V3 and ground. No PCB layout is supplied.

```text
Pico USB -> onboard 3V3 -> HX711 VCC
Pico GND -------------> HX711 GND, switch return
Pico GP2 <------------- HX711 DOUT
Pico GP3 --------------> HX711 SCK
Pico GP15 <------------- normally-open switch -> GND
HX711 E+/E- <----------> load cell excitation pair
HX711 A+/A- <----------> load cell signal pair
```
