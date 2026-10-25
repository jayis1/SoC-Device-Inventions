# Assembly guide

This is a low-voltage prototype requiring soldering and a Pico carrier board. Before applying power, verify no short between 3V3 and GND, confirm both shift registers' orientation, check each LED polarity, and confirm the 74HC595 output pins from the manufacturer's package drawing. Then connect USB and inspect the 3V3 rail. Load firmware only after implementing the Pico SDK adapter; the repository currently supplies only a portable logic core.
