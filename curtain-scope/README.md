# Curtain Scope

Offline focal-plane shutter curtain travel tester for camera repairers. A repairer places a removable light-gate strip at the film plane, fires a camera at a safe low-energy setting, and compares leading/trailing curtain passage at five positions. Unlike `shutter-tick`, which measures one leaf-shutter pulse duration, this fixture measures spatial timing skew across a focal-plane frame to reveal uneven travel or curtain drag. It is a diagnostic aid, not a calibrated camera service instrument.

## Architecture

```text
5 x phototransistor gate -> 5 x MCP6002 comparator channels -> 5 GPIO edge inputs
                                                RP2040 / Pico -> USB CDC CSV
```

RP2040 (Raspberry Pi Pico) was chosen for five simultaneous edge timestamps and simple USB serial output; the Pico module supplies its own flash and 12 MHz crystal. Gate strip: five matched Vishay TEPT5700 phototransistors, each with a 10 kΩ pull-up and a comparator threshold trimmer. Use a diffuse constant LED source behind the gate strip; no laser or camera modification.

## Pin map

| Signal | Pico GPIO | Notes |
|---|---:|---|
| Gate 0..4 | GP2..GP6 | comparator outputs, active-low falling edge |
| Threshold status LED | GP15 | series resistor; firmware heartbeat |
| USB CDC | native | data output |
| SWD | SWCLK/SWDIO | debug, exposed pads |

All gate outputs are 3.3 V logic. GP2–GP6 and GP15 are not QSPI or boot pins. Firmware records edge times using the RP2040 hardware timer; reports only first edge per channel per capture, with overflow and missing-gate flags.

## Power

USB 5 V powers the Pico; its onboard regulator supplies 3.3 V. External sensor strip connects to Pico 3V3 and GND only. Estimated board current: 25 mA idle, 35 mA capture, <50 mA peak including LED; power budget must be measured on prototype. Do not connect camera flash/high voltage or camera circuitry. The camera is operated independently.

## Build / use

Requires Raspberry Pi Pico SDK 2.1.1 and CMake 3.13+. `cmake -S firmware -B build -DPICO_SDK_PATH=/path/to/pico-sdk && cmake --build build`. Flash `curtain_scope.uf2` by BOOTSEL. Open USB CDC at 115200 (CDC ignores baud); send `START\n`, expose gate strip, fire camera, then read one CSV row per capture. Output format and parser are in `docs/api.md`. Source is a reference implementation and has not been cross-built or hardware-tested in this run.

## Schematic status

`schematic/netlist.md` is the authoritative connection specification; no fabricated KiCad file is provided. Comparator reference must be trimmed against illumination before use. See assembly notes for alignment and limitations.

## BOM

See `hardware/BOM.csv` (estimated listed-parts subtotal about $8.17 excluding enclosure, fixture, diffuse LED source, and shipping). Prices are planning estimates; verify sourcing before build.

## Status and limitations

Reference design; hardware unverified. Gate alignment, comparator hysteresis, phototransistor response matching, timer calibration, and shutter-cycle repeatability require bench validation. Optical geometry can bias readings; results do not establish shutter speed accuracy or diagnose cause. Use only with camera's ordinary low-energy shutter cycle; never open or electrically probe a camera. No regulatory or calibration claims.

## License

MIT, consistent with repository license. Design and documentation authored by jayis1.
