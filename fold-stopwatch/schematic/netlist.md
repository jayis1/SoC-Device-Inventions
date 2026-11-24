Ref design wiring reference (not a KiCad schematic)

U1: Raspberry Pi Pico SC0915 module. USB is the only power source.
J2/J3: TCST2103 slot sensor ports, pins 1=3V3, 2=OUT, 3=GND. Verify physical pinout against selected component datasheet before wiring.
R1: 10 kΩ from J2.OUT to 3V3.
R2: 10 kΩ from J3.OUT to 3V3.
C1/C2: 100 nF across each sensor's 3V3/GND near connector.
J2.OUT -> U1 GP2; J3.OUT -> U1 GP3.

TCST2103 phototransistor collector outputs are pulled up to 3V3. Emitter returns to GND. IR LED anode supply requires series resistor sized from the exact LED forward voltage/current; include it within each sensor assembly and do not omit it. Confirm module output never exceeds 3.3 V. USB VBUS is not connected to sensor outputs or GPIO.

Prototype wiring only; no PCB layout, ERC, connector footprint, or fabrication validation supplied.
