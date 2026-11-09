Reference wiring (not a KiCad schematic; symbol-level CAD and ERC are not supplied).

- J1: Pico board; USB supplies 5 V. Pico 3V3(OUT) powers sensor breakout.
- J2: TCS34725 breakout, VIN=3V3, GND=GND, SDA=GP4, SCL=GP5. I2C address 0x29; 100 kHz.
- R1/R2: 4.7 kΩ SDA/SCL pull-ups to 3V3 only if breakout lacks them. Never pull up to 5 V.
- C1: 100 nF ceramic across breakout 3V3/GND near connector if module lacks local bypass.
- J3: USB host via Pico connector, power and USB CDC.

All grounds common. Keep sensor physically outside liquid; no electrical connection to burette, sample, or electrodes. Confirm module pin labels, voltage tolerance, pull-up rail, and sensor identity before wiring. The design provides no isolation barrier, chemical shielding, or PCB layout.
