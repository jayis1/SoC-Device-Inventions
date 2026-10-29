# Tank Tally

Offline film-developing agitation logger

Tank Tally clips to a 35 mm/120 developing tank and records inversion/agitation timing so a photographer can reproduce development cadence between rolls. It is a logger and practice aid, not a chemistry timer or process-control instrument. Its use case (manual inversion cadence) differs from the repo's enlarger exposure integrator and other optical/film tools.

## Design summary

A Raspberry Pi Pico (RP2040) samples an LIS3DH accelerometer over I2C at 100 Hz. Firmware detects rotation-like motion using a simple magnitude-variation threshold and a refractory interval, accumulates event intervals, and exports CSV as lines over USB CDC. The user starts a run by pressing the BOOTSEL-adjacent user switch (GPIO14), then agitates the tank; a second press ends the run. No radio, cloud, battery, chemistry contact, or autonomous actuator is present.

This is a low-voltage reference design. It is not a certified measurement instrument; event thresholds depend on clip placement and tank geometry. Validate the classifier with dry runs before relying on records. Do not attach it where it can fall into chemicals or obstruct tank sealing.

## Block diagram

```text
USB host ── Pico RP2040 ── I2C ── LIS3DH accelerometer
                  │
                  ├── GPIO14 run button
                  └── USB CDC CSV log
```

## Power and wiring

USB 5 V powers the Pico; its 3.3 V rail powers the LIS3DH breakout. Use a breakout explicitly rated for 3.3 V logic and with onboard regulator/level shifting only if its documentation permits 3.3 V supply; simplest recommendation is a bare 3.3 V LIS3DH breakout. Peak current budget: Pico up to 100 mA (design allowance), LIS3DH 1 mA, button negligible: 101 mA. Pico USB input budget is sufficient; estimated design margin against 500 mA host port is 79.8%. No battery or charger.

| Signal | Pico GPIO | Connection |
|---|---:|---|
| I2C0 SDA | GP4 | LIS3DH SDA, 4.7 kΩ pull-up to 3V3 |
| I2C0 SCL | GP5 | LIS3DH SCL, 4.7 kΩ pull-up to 3V3 |
| Button | GP14 | Momentary button to GND, internal pull-up |
| Power | 3V3(OUT) | Sensor VDD; common GND |
| USB | USB D+/D− | Pico native USB CDC |

LIS3DH address is 0x18 (SDO/SA0 low); verify breakout strap. Pico provides SWD pads and USB bootloader access. The Pico module performs its own reset, clocking, flash, and regulation; do not connect external voltages to GPIO.

## Build and use

Requires Raspberry Pi Pico SDK 2.x, CMake, and ARM GNU toolchain. From `firmware/` run `cmake -S . -B build -DPICO_SDK_PATH=/path/to/pico-sdk -DPICO_BOARD=pico`, then `cmake --build build`. Copy `tank_tally.uf2` to the Pico in BOOTSEL mode. Firmware build was not run in this design session; physical hardware has not been assembled or tested.

Open the USB CDC serial port at 115200 (CDC ignores baud on USB). Press the button to start/stop. Records: `event,index,elapsed_ms,interval_ms`. Button starts/stops only; timestamps use monotonic uptime. The event detector and CSV formatting are in `firmware/main.c`.

## BOM (reference build)

See `hardware/BOM.csv`. Pico is a module, not a custom bare-chip design. Sensor breakout choice is flexible but its exact board-level circuitry varies; the schematic is a connectivity/netlist reference, not a KiCad native schematic or PCB layout. A clip enclosure and mounting method remain to be fabricated.

## Assembly and validation

1. Inspect Pico and LIS3DH breakout for shorts; power Pico from USB without sensor and confirm enumeration.
2. Wire 3V3/GND and I2C; verify 3.3 V at sensor before connection. Fit the two 4.7 kΩ pull-ups if absent on the breakout.
3. Connect GP4/GP5; verify address 0x18 with an I2C scanner before running firmware.
4. Wire GP14 to a normally-open button to GND. Confirm the input idles high.
5. Flash UF2, run a stationary baseline, then make controlled inversions. Tune threshold in firmware if the chosen clip/tank differs.

No KiCad ERC, SDK compile, or bench test is claimed. `schematic/connectivity.txt` provides the pin-level connection list. BOM/script validation and host-side syntax checks are the checks performed for this reference package.

## Limitations

The magnitude-variation detector is intentionally simple and may count bumps or miss slow motion; output is evidence for repeatability, not a validated agitation metric. No chemical/process recommendations are embedded. Authored by jayis1.