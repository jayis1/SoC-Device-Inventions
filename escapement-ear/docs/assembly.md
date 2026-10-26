# Assembly and first power

This is a breadboard/breakout reference, not a fabrication package. Use a Pico module and 3.3 V-compatible OLED. Check exact comparator and amplifier package pinouts from vendor datasheets. Connect the piezo only through the series resistor/clamp/filter network; keep its wire short and the analog ground return separate from USB noise until the board ground point. Avoid shorting 3V3 to VBUS.

Before connecting USB, inspect for shorts between 3V3/GND and VBUS/GND. Power from a current-limited USB source; verify Pico 3V3 is within the module specification, then check comparator output idles low and produces one rising edge per mechanical impulse. Adjust RV1 only after checking the comparator input stays within its rails. If the output sticks high/low or multiple edges appear, disconnect and troubleshoot. Do not attach to powered equipment.

No custom PCB layout, enclosure, measured current, or physical test is included. The present firmware reports over USB CDC; OLED support is not implemented in this prototype.
