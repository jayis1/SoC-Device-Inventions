Reference wiring (not a KiCad schematic or fabrication document)

J1: Raspberry Pi Pico module; USB connector is the sole power input.
R1: 10 kΩ linear potentiometer, end terminals to 3V3(OUT) and GND.
R2: 10 kΩ series resistor from wiper to GP26/ADC0.
C1: 100 nF from GP26/ADC0 to GND (place near Pico).
SW1: normally-open momentary switch from GP15 to GND; firmware enables internal pull-up.

Net summary:
3V3 -> R1.1
GND -> R1.3, C1.2, SW1.2
R1.wiper -> R2.1
R2.2 -> ADC0 / GP26 / C1.1
GP15 -> SW1.1

Use a Pico module with USB supply; do not connect external voltage to 3V3 while USB-powered. ADC input is restricted to 0–3.3 V. This reference omits PCB layout, ESD qualification, and enclosure details; it is suitable only for a low-voltage bench prototype.
