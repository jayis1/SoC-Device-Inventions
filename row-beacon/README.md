# Row Beacon

Status: reference design; firmware core host-syntax-checked, Pico SDK build and hardware not verified. Author: jayis1.

## Overview
A clip-on knitting row counter for knitters working flat or in the round. A small magnet on a removable needle marker passes a Hall sensor at the end of each row/round; the device increments, displays the count, and lets the user undo an accidental count. Unlike a phone counter, it detects the event without requiring the knitter to put down the work or tap a screen.

Demo: clip the sensor at the fabric edge, attach the magnet marker to the working needle, and knit. Each marker pass increments the large OLED count; press the encoder to undo, rotate to set a target, and hold to reset after confirmation. Estimated BOM $18–25; module-based assembly is beginner-friendly soldering, no custom PCB required.

## Problem and differentiation
Paper tallies and phone apps lose context or interrupt two-handed work. Existing repository devices measure loom warp tension/motion or sewing-machine cycles; Row Beacon is a personal knitting accessory that counts a deliberate marker pass, not machine operation, fabric feed, or tension. It is a low-risk craft tool, not a production counter or garment-quality instrument.

## Block diagram
```text
Magnet marker -> DRV5032 Hall switch -> RP2040 Pico GPIO -> count/debounce core
                                              | I2C
                                              +-> SSD1306 128x64 OLED
                                              +-> rotary encoder + push switch
USB 5 V -> Pico onboard regulator -> 3V3 logic and OLED
```

## SoC choice
Raspberry Pi Pico module (RP2040, 40-pin header version) keeps assembly accessible and provides enough GPIO, I2C and USB for local display/configuration. The design does not need radio or cloud connectivity. Use a genuine Pico board, not a pin-compatible clone without checking its pinout. GPIO operates at 3.3 V and is not 5 V tolerant.

## Pin map
| Function | Pico GPIO | Notes |
|---|---:|---|
| Hall switch output | GP2 | DRV5032 digital output, active low; 10 kΩ pull-up to 3V3 |
| I2C0 SDA | GP4 | OLED address 0x3C; 4.7 kΩ pull-up to 3V3 |
| I2C0 SCL | GP5 | OLED address 0x3C; 4.7 kΩ pull-up to 3V3 |
| Encoder A | GP10 | Switch to GND; internal pull-up |
| Encoder B | GP11 | Switch to GND; internal pull-up |
| Encoder push | GP12 | Switch to GND; internal pull-up |
| USB | Pico USB | Power and optional USB CDC diagnostics |
| Ground | GND | Common return |

GPIO values refer to the RP2040 GPIO identifiers, not physical header numbers. GP2/4/5/10/11/12 are exposed general-purpose pins; keep SWD and BOOTSEL accessible. Hall sensor supply/output is 3.3 V.

## Power architecture and budget
USB-powered only. USB 5 V feeds the Pico's onboard regulator; regulated 3V3 powers the OLED, Hall switch, and encoder. No battery or charger is included. Estimate: Pico 20–30 mA active, OLED 8–20 mA depending on pixels/brightness, Hall sensor <1 mA, total typical 30–50 mA at 5 V input; allow 100 mA. These are planning estimates, not measured values. Use 100 nF local bypass near the sensor and 10 µF bulk across 3V3/GND near the OLED. Avoid powering from an unknown USB-C breakout unless it has correct CC pull-downs; the Pico's own connector is preferred.

## BOM
See `hardware/BOM.csv`. Prices are indicative estimates, not quotes; confirm package and module revisions before purchase. No custom PCB is required: use a Pico, small solderable perfboard, and short wires.

## Firmware architecture
`firmware/row_core.c` contains portable edge-debounce and count/undo logic; `firmware/main.c` is the Pico SDK adapter skeleton. It accepts one active-low Hall event after a 35 ms quiet period, saturates at UINT32_MAX, and supports undo. The adapter indicates where to integrate an SSD1306 driver and encoder scan; UI/display integration is not implemented in this prototype. Build target: Raspberry Pi Pico SDK 2.x, CMake, GCC ARM Embedded. This environment lacks Pico SDK and arm-none-eabi-gcc; target compilation is unverified. Host check: `gcc -std=c11 -Wall -Wextra -Werror -fsyntax-only firmware/row_core.c`.

## Build and flash
Install Raspberry Pi Pico SDK 2.x and its documented ARM toolchain. Set `PICO_SDK_PATH`, then run `cmake -S firmware -B firmware/build -DPICO_SDK_PATH="$PICO_SDK_PATH"` and `cmake --build firmware/build`. The current adapter is intentionally a scaffold and requires an SSD1306/encoder implementation before a usable UF2 can be produced. Do not flash this prototype expecting a complete user interface.

## Assembly
Follow `docs/assembly.md`. Keep the Hall sensor face toward the marker magnet and adjust the gap until a pass produces one transition. Magnet orientation and sensor sensitivity depend on the selected component; verify on the bench. Do not attach the sensor where it can snag yarn or obstruct needle movement.

## API
USB CDC diagnostic line format and interaction behavior are described in `docs/api.md`. The portable core does not persist counts across power loss; record the count before unplugging.

## Testing and calibration
Run `gcc -std=c11 -Wall -Wextra -Werror -fsyntax-only firmware/row_core.c` and `python3 scripts/check_bom.py`. Bench-test at least 100 deliberate marker passes, 100 stationary/no-magnet trials, quick repeated motion, undo, and UINT32_MAX saturation before relying on counts. No hardware tests have been performed.

## Safety and limitations
Low-voltage USB device only. This is a craft aid, not a medical, safety, or industrial instrument. The magnet is a small swallow hazard; keep it away from children and implanted devices, and secure it so it cannot detach. No ESD, EMC, enclosure, or regulatory assessment is claimed. It does not detect missed stitches and cannot guarantee one count per row. Counts are volatile and display/encoder integration remains incomplete. The `schematic/` folder contains a connection-level netlist, not a native KiCad schematic; no PCB layout is supplied.

## License
Repository MIT license. Author and project attribution: jayis1.
