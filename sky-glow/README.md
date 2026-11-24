# Sky Glow — portable night-sky brightness logger

Sky Glow is a low-voltage handheld reference instrument for comparing night-sky brightness at the same site and pointing direction. A Raspberry Pi Pico samples a TSL2591 ambient-light sensor and emits raw counts over USB CDC for offline comparison. It is distinct from the repository's cosmic-ray telescope (particle coincidence) and other optical instruments (spectral, imaging, transmission or reflectance measurements): it targets wide-field night-sky background comparison.

This is an observing aid, not an observatory-grade photometer. The baffled matte-black tube defines only an approximate 20-degree field. Sensor spectral response, stray light, gain/integration settings and calibration limit absolute accuracy. The prototype firmware reports raw counts and does not claim magnitudes/photometric accuracy. OLED is present in the BOM but not initialized by this firmware revision.

Block diagram: USB 5 V → Pico board 3.3 V regulator → I2C TSL2591 sensor (0x29); GP4/GP5 I2C; USB CDC → host CSV logger. Optional SSD1306 OLED (0x3C) shares I2C. Firmware emits sensor errors and saturation flags.

| Pico pin | Signal | Connection |
|---|---|---|
| 3V3(OUT), GND | supply | sensor and optional OLED |
| GP4 | I2C0 SDA | TSL2591 SDA; OLED SDA |
| GP5 | I2C0 SCL | TSL2591 SCL; OLED SCL |
| GP25 | onboard LED | fault indicator |
| USB CDC | data | CSV stream |

Use breakout boards explicitly rated for 3.3 V logic; pull-ups must terminate at 3.3 V. Power through the Pico USB connector only, never simultaneously inject 5 V. Budget allowance: Pico 50 mA, sensor 2 mA, OLED 25 mA; 100 mA USB allowance, not measured. Daylight can saturate the sensor. No battery, high voltage, radio or external load.

## Build

Raspberry Pi Pico SDK 2.x. Set `PICO_SDK_PATH`, then `cmake -S firmware -B build -DPICO_BOARD=pico` and `cmake --build build`; flash `build/sky_glow.uf2` via BOOTSEL. Target build not verified here because the SDK/toolchain is unavailable. Hardware is unbuilt and uncalibrated. This schematic-stage reference contains a wiring netlist, not native KiCad or fabrication files.

Files: `schematic/netlist.md`, `hardware/BOM.csv`, `firmware/`, `docs/ASSEMBLY.md`, `docs/API.md`, `scripts/skyglow_log.py`. MIT. Invented and maintained by jayis1.
