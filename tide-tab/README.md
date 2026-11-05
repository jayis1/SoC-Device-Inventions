# Tide Tab

Offline aquarium evaporation and top-off interval logger. A magnet-mounted float switch records when the water surface crosses a chosen low-level threshold; it does not pump water or control mains equipment. The novelty relative to the catalog is event-duration logging for passive water-level change, rather than aquarium chemistry control (Aqua Guard) or sump pump fault detection (Sump Shepherd).

## Prototype status
Reference design, not fabricated or electrically validated. Firmware core compiles as portable C when built with the Pico SDK; no sensor accuracy or field performance is claimed. This is a USB-powered development-board assembly, not a custom routed PCB. The switch and USB remain isolated from any powered pump.

## Architecture
- SoC: Raspberry Pi Pico board (RP2040, 3.3 V GPIO); USB supplies VSYS.
- Sensor: normally-open low-voltage magnetic float switch to GPIO 15/GND. Internal pull-up; closed means threshold reached.
- Status: LED on GPIO 16 with 330 ohm series resistor to ground.
- Output: USB CDC JSONL. One record per level transition, including monotonic milliseconds and state. No network, battery, actuator, or cloud dependency.
- Current: Pico board from USB; GPIO LED adds at most about 4 mA at 3.3 V. Sensor input is pulled up internally; switch current is negligible. Use a reputable current-limited USB supply.

## Pin assignments
| Pico signal | Connection | Notes |
|---|---|---|
| GND | Switch return, LED return | Common low-voltage ground |
| GPIO15 | Float switch | Switch between pin and GND; internal pull-up |
| GPIO16 | LED through 330 ohm | LED anode toward GPIO, cathode resistor to GND |
| USB | Host / 5 V supply | CDC console and power |

Use only a dry-contact switch rated for low-voltage logic. Keep electronics above water, provide drip loops, and do not use the device as an overflow safety interlock. Debounce is 50 ms; records are generated only on stable transitions.

## Build and run
Install Raspberry Pi Pico SDK (v2.1.1 recommended), set `PICO_SDK_PATH`, then:

```sh
cd firmware
cmake -S . -B build -DPICO_BOARD=pico
cmake --build build
```

Flash `build/tide_tab.uf2` using the Pico BOOTSEL drive. Open its USB CDC serial port at 115200 baud. JSONL example: `{"ms":12345,"level_low":true}`. The monotonic timestamp resets at each power-up; host software should timestamp sessions if wall-clock time is needed.

## Validation and limitations
`firmware/src/tide_tab.c` contains the portable state machine and can be host-tested; `main.c` adapts GPIO/USB to Pico SDK. No hardware test, water-resistance test, or calibration has been performed. Float installation height determines the threshold; validate it with a measured water-level rise/fall before relying on logs. Power loss is not logged and does not preserve timestamps. No PCB fabrication files are supplied.

## Contents
- `schematic/tide-tab.kicad_sch`: KiCad-format conceptual sheet documenting external interconnect; board symbols are not a fabrication schematic.
- `schematic/netlist.md`: readable wiring reference.
- `hardware/BOM.csv`: parts and ratings.
- `firmware/`: Pico SDK C project.
- `docs/ASSEMBLY.md`: bench assembly and acceptance checks.
- `scripts/check_events.py`: offline JSONL validator.

Invented and maintained by jayis1.
