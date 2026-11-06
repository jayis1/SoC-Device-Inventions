# Tea Steep

Tea Steep is a low-voltage, offline bench instrument for comparing infusion color over time. A fixed white-LED transmission path, TCS34725 color sensor, and DS18B20 probe record normalized red/green/blue/clear readings and liquid temperature to USB CDC CSV. It is intended for repeatable recipe experiments, not chemical concentration or food-safety measurement. It differs from this repository's environmental monitors and optical material analyzers by measuring the time evolution of a liquid infusion in a fixed cuvette.

Status: reference prototype. Firmware core host-tested; target build, calibration, optical performance, and hardware are unverified. No PCB layout is supplied.

## Design

- SoC: Raspberry Pi Pico (RP2040, RP2040 datasheet / Pico SDK 2.x)
- Sensor: TCS34725FN on a breakout with 3.3 V-compatible I2C; address 0x29
- Temperature: waterproof DS18B20 probe, 1-Wire with 4.7 kΩ pull-up to 3V3
- Illumination: white LED at fixed current through 1 kΩ, switched by 2N7002 NMOS; LED off between measurements
- Supply: USB 5 V into Pico, regulated 3V3 from Pico; sensor and pull-ups are 3.3 V only. Do not connect external 5 V I2C pull-ups.
- No battery, wireless radio, heater, or mains connection.

### Connections

| Pico GPIO | Signal | Connection |
|---|---|---|
| GP4 | I2C0 SDA | TCS34725 SDA; 4.7 kΩ pull-up to 3V3 |
| GP5 | I2C0 SCL | TCS34725 SCL; 4.7 kΩ pull-up to 3V3 |
| GP2 | 1-Wire | DS18B20 data; 4.7 kΩ pull-up to 3V3 |
| GP15 | LED_EN | 100 Ω to 2N7002 gate; 100 kΩ gate pulldown |
| GND | common return | Pico, sensor, probe, LED driver |
| 3V3 | sensor/probe logic | TCS34725 breakout and DS18B20 pull-ups |
| VSYS/USB | board power | Pico USB input only; do not inject external power here while USB attached |

### Optical/mechanical arrangement

Use a black, light-tight enclosure with the LED and sensor on opposing sides of a clear 10 mm cuvette. Keep geometry fixed and mask stray light. Record a blank-water reference and a dark reading before each series. The sensor's RGB channels are relative counts and vary with cuvette, alignment, LED spectrum, and tea type; compare within a controlled setup only.

## Build and operation

Install Raspberry Pi Pico SDK 2.x and its documented ARM GCC/CMake/Ninja toolchain. From `firmware/`: `cmake -S . -B build -DPICO_SDK_PATH=$PICO_SDK_PATH`, then `cmake --build build`. Flash `tea_steep.uf2` to a Pico in BOOTSEL mode. Connect USB CDC at 115200 baud (terminal settings are advisory for USB CDC). The current firmware samples the TCS34725 at approximately 1 Hz and emits CSV. The DS18B20 hardware is specified but its driver is not implemented yet; temperature is marked invalid and rows carry `ERR` until that driver is completed. Sensor I2C failures also produce `ERR`. Use the reference only for bring-up, not valid measurements.

## Files

- `schematic/netlist.md`: connection-level schematic specification (not a KiCad drawing)
- `hardware/BOM.csv`: parts and values
- `firmware/`: Pico SDK target and portable sampling/CSV core
- `docs/ASSEMBLY.md`, `docs/API.md`: build, wiring, and record format
- `scripts/plot_log.py`: optional CSV plotting helper

The design is not calibrated, food-contact certified, or intended for safety decisions. Verify all breakout-board regulator/pull-up details before wiring. A measured color trend is not a validated estimate of tea strength.
