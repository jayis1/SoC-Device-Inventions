# Clay Ledger connection-level schematic

Reference wiring only; this is not a native KiCad schematic or fabrication design. Use Raspberry Pi Pico (RP2040) module, not a bare RP2040.

```text
J1 Pico micro-USB -> USB_VBUS (5 V) and USB data
Pico 3V3(OUT) pin 36 -> U2 AS5600L breakout VCC
Pico 3V3(OUT) pin 36 -> U3 SHT31-D breakout VCC
Pico GND pin 3 -> U2 GND, U3 GND, common ground
Pico GP4 pin 6 (I2C0 SDA) -> U2 SDA, U3 SDA
Pico GP5 pin 7 (I2C0 SCL) -> U2 SCL, U3 SCL
R1 4.7k 3V3->SDA; R2 4.7k 3V3->SCL (fit only if modules lack pullups)
C1 100nF 3V3-GND adjacent to U2; C2 100nF 3V3-GND adjacent to U3
Pico GP25 onboard LED -> firmware heartbeat
```

Addresses: U2 AS5600L = 0x36; U3 SHT31-D = 0x44. Both share I2C0 without conflict. Confirm breakout pull-ups and voltage translation before connecting; no signal may exceed 3.3 V. Cable runs should be short. There is no external connector, protection rating, PCB layout, ERC, or DRC in this reference design.

Mechanical sensor path: carriage magnet -> AS5600L angle; carriage belt/pulley links to one end of test bar; the opposite end references the fixture. The mechanism and magnet alignment are not electrically represented.
