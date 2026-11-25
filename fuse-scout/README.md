# Fuse Scout

Offline, USB-powered low-voltage fuse continuity and comparative voltage-drop checker for hobby electronics benches. It tests individual, unpowered blade or cartridge fuses using a current-limited 5 V stimulus, reports OPEN / CHECK / PASS on a small display (display integration is optional), and emits a compact USB CDC result. It is not a fuse-rating tester and cannot establish whether a fuse will clear safely at its rated current.

The repository already includes a cable continuity mapper and a coin-cell pulse tester; Fuse Scout differs by measuring the resistance of a removable, unpowered fuse under a known low test current rather than mapping multiwire connectivity or loading a cell.

## Design limits and safety

- Test only one disconnected, unpowered fuse at a time. Remove from its circuit first.
- The fixture is powered only from USB 5 V. No external supply or battery connects to the test terminals.
- A 47 ohm series resistor limits worst-case short-circuit stimulus to about 106 mA at 5 V (nominal; USB and resistor tolerances apply). The fixture is not for mains fuses in energized equipment.
- Intended for small fuses with expected resistance under 1 ohm. Contact resistance dominates; readings are comparative screening only.
- No claim of fuse interrupt rating, time-current curve, internal damage detection, or calibration accuracy.

## Architecture

```text
USB 5 V -> Raspberry Pi Pico (RP2040) -> 3V3
 GPIO2 -> TEST_EN -> 2N3904 low-side switch
 GPIO26/ADC0 <- scaled test-node voltage
 GPIO3/4 I2C -> optional SSD1306 OLED, address 0x3C
 USB CDC -> textual status and raw ADC
```

Fixture circuit: USB 5 V feeds RTEST=47 ohm, 1%, 0.25 W, then the positive test clip. The negative clip returns through an NPN transistor (2N3904) to ground, enabled only during a measurement. A 10 kΩ base resistor limits GPIO drive; 100 kΩ base-emitter pulldown keeps the stimulus off during reset. ADC0 samples the voltage at the positive clip through a 100 kΩ / 100 kΩ divider; add 100 nF from ADC input to ground. With a sound fuse the clip node is near 5 V; an open fuse is near zero. The maximum ADC input is under 2.6 V including USB 5.25 V tolerance. Ground clip, Pico ground, and USB ground are common.

The ADC circuit indicates node voltage, not calibrated fuse resistance. Firmware classifies a stable node above 2.0 V as PASS, 0.4–2.0 V as CHECK, and below 0.4 V as OPEN. These provisional thresholds require bench validation with known resistors and are not production specifications.

## Pin assignments

| Pico GPIO | Function | Notes |
|---|---|---|
| GP2 | TEST_EN | Base resistor to Q1; active high; default low |
| GP26 / ADC0 | SENSE | 100k:100k divider, 100 nF to GND |
| GP3 | OLED SDA | Optional SSD1306, 3.3 V only |
| GP4 | OLED SCL | Optional SSD1306, 3.3 V only |
| SWD pads | Debug | SWDIO/SWCLK as on Pico board |

Do not connect 5 V to RP2040 GPIO. The Pico board's onboard regulator supplies 3.3 V. The ADC divider, stimulus, and display share ground.

## BOM

See `hardware/BOM.csv`. Use a genuine Raspberry Pi Pico or electrically equivalent RP2040 board, and keep the fuse fixture isolated from powered circuits. The schematic directory contains the wiring/netlist source, not a fabrication-ready KiCad drawing or PCB.

## Firmware

Firmware is the portable C decision core in `firmware/src/fuse_scout.c`; it is independent of the Pico SDK. Build the core with:

```sh
gcc -std=c11 -Wall -Wextra -Werror -pedantic -c firmware/src/fuse_scout.c -o /tmp/fuse_scout.o
gcc -std=c11 -Wall -Wextra -Werror -pedantic firmware/tests/test_fuse_scout.c -o /tmp/fuse_scout_test && /tmp/fuse_scout_test
cmake -S firmware -B /tmp/fuse-scout-build && cmake --build /tmp/fuse-scout-build
```

The `scripts/classify_adc.py` helper mirrors the provisional status thresholds. Pico ADC acquisition, USB CDC glue, and OLED rendering are integration work, not implemented in this reference version. Firmware defaults TEST_EN low; a host must explicitly call the sampling/classification API.

## Build and bring-up

1. Inspect the fixture with no fuse inserted; ensure no exposed clip can contact USB 5 V directly.
2. Power from a current-limited USB port. Confirm test output is inactive at reset.
3. With no fuse, confirm the sensed node is below 0.4 V and status is OPEN.
4. Use a 47 ohm test resistor, then a short, then an open circuit to verify the three reporting bands before inserting a fuse.
5. Test only a removed, unpowered fuse. Clean clips and repeat measurements; treat borderline results as CHECK.

The first prototype should be current-limited and checked with a DMM. No physical testing, calibration, KiCad ERC, or target firmware build is claimed. Not a safety instrument; never use its result to certify protection in life-safety or mains equipment.
