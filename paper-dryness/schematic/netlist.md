# Reference connections (not a KiCad capture)

Pico 3V3(OUT) -> MCP3008 VDD, VREF; Pico GND -> MCP3008 AGND, DGND.
Pico GP16/GP17/GP18/GP19 -> MCP3008 DOUT/CSn/CLK/DIN respectively.
Place 100 nF from MCP3008 VDD to GND adjacent to U2.
Pico GP4/GP5 -> OLED SDA/SCL; OLED supply 3V3 and GND. Add 4.7 kΩ pull-ups to 3V3 if not present on the display module.
Pico GP25 is onboard LED. GP14 is reserved for a future comparator-based probe timing revision; leave unconnected in baseline.

No KiCad symbols, electrical rule check, or PCB layout are provided. Build module-level wiring only and verify all module pin labels before powering.
