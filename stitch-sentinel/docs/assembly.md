# Assembly guide

This reference design needs a Pico-compatible USB cable, two 3.3 V reflective optical sensor modules with safe open-collector outputs, a simple bracket, and the BOM parts. Use a breadboard/prototype wiring first; no PCB is supplied.

1. With the sewing machine unplugged, fit sensor 1 to view a small reflective marker fixed to a safely accessible reciprocating needle-bar surface. Do not attach anything where it can enter the needle path.
2. Fit sensor 2 to view a marked fabric edge or fabric-driven feed roller. The sensor must not impede material movement.
3. Wire the nets in `../schematic/netlist.md`. Inspect the sensor outputs unpowered and powered; never connect a 5 V push-pull output to Pico GPIO.
4. Before plugging in USB, check continuity, polarity, and absence of shorts. Power from isolated USB and check 3V3 and idle current (expected below 100 mA).
5. Build and flash per firmware instructions. Open the Pico USB serial port at 115200 baud, 8-N-1.
6. Run slowly on scrap material. Adjust optical alignment/thresholds until cycle/feed edges are reliable, then test an intentional stop. Stop and unplug the sewing machine before repositioning anything.

No mains, motor, pedal, or foot-control wiring is permitted. Prototype not physically tested; bracket design and optical performance are machine-dependent.
