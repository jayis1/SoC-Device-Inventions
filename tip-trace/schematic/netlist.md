# Tip Trace connectivity netlist

Reference wiring, not a native KiCad schematic. Use a Pico development board and verify its pin labels before powering.

| Ref | Pin | Net |
|---|---|---|
| J1 Pico | GP18 | SPI_SCK |
| J1 Pico | GP16 | SPI_RX |
| J1 Pico | GP17 | TC_CS_N |
| J1 Pico | 3V3(OUT) | +3V3 |
| J1 Pico | GND | GND |
| U1 MAX31855KASA+ | 1 (T−) | J2 K− |
| U1 MAX31855KASA+ | 2 (T+) | J2 K+ |
| U1 MAX31855KASA+ | 3 (GND) | GND |
| U1 MAX31855KASA+ | 4 (VCC) | +3V3 |
| U1 MAX31855KASA+ | 5 (SCK) | SPI_SCK |
| U1 MAX31855KASA+ | 6 (CS) | TC_CS_N |
| U1 MAX31855KASA+ | 7 (SO) | SPI_RX |
| C1 100 nF | 1 | +3V3 |
| C1 100 nF | 2 | GND |
| J2 thermocouple | K+ | U1 T+ |
| J2 thermocouple | K− | U1 T− |

Place C1 directly across U1 supply pins. Use a polarized keyed thermocouple connector and keep thermocouple leads away from noisy USB/power wiring. Do not add a pull-up to SO unless datasheet review establishes a need. Confirm pin numbering against the exact package datasheet before layout.
