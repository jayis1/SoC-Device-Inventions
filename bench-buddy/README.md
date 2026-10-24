# Bench Buddy

USB-powered, low-voltage continuity and connector mapper for makers repairing loose cable harnesses. It identifies which of up to eight contacts at one end connect to contacts at the other end, reports open/short/crossed wiring, and never drives an unknown cable with more than 3.3 V.

## User and use case

A repair volunteer or electronics student can map an unlabelled, unpowered cable before re-terminating it. A multimeter can do the same job, but requires repeated probe placement and manual note-taking; Bench Buddy automates the matrix scan and presents the result over USB serial. It is not for mains wiring, energized circuits, or safety-critical harness certification.

Non-goals: high-speed cable qualification, insulation resistance testing, shield bonding certification, and testing connected/energized equipment. Estimated prototype BOM is USD $15–25 excluding the Raspberry Pi Pico; hand assembly is moderate (fine-pitch TSSOP on a breakout or use a socketed 74HC4051 module).

## Architecture

```text
USB 5 V ── Pico (RP2040) ── GPIO scan controls ── 74HC4051 mux ── 1k current-limit ── test cable
                                          └── ADC sense inputs ◀── opposite cable contacts
```

Use a Raspberry Pi Pico board (RP2040, board includes QSPI flash and 3.3 V regulator). One mux selects the driven contact; a second selects the sensed contact. External 10 kΩ pull-downs ensure unselected inputs read low. Firmware scans each drive/sense pair at low current and reports a connectivity matrix. The cable must be disconnected at both ends from all equipment.

## Pin assignment

| Pico GPIO | Function | Notes |
|---|---|---|
| GP2, GP3, GP4 | drive mux S0–S2 | 3.3 V logic |
| GP5 | drive mux enable | active-low; external pull-up disables at reset |
| GP6, GP7, GP8 | sense mux S0–S2 | 3.3 V logic |
| GP26 / ADC0 | sense mux common | 0–3.3 V only |
| GP0 / GP1 | UART0 TX/RX | optional debug header |
| VSYS, GND | Pico power | USB powered; test fixtures are not electrically isolated |

RP2040 ADC inputs are on GPIO26–29; only GP26 is used. GP23–25 and QSPI pins are unused. The Pico USB connector is the flashing/debug interface.

## Power and limits

USB 5 V powers the Pico; its onboard regulator supplies 3.3 V. No battery or charger. Only one 3.3 V source passes through a 1 kΩ resistor at a time, limiting worst-case contact current to about 3.3 mA for a hard short. A 10 kΩ pulldown limits a floating receive node. Do not connect this unit to live, externally powered, or unknown voltage circuits. These are design estimates, not measured values.

## BOM

See `hardware/BOM.csv`. The external circuit is deliberately minimal. Cable fixtures are keyed 2×4 headers; wire each fixture contact to its matching mux channel and label pin 1.

## Firmware / build

Firmware is C targeting Raspberry Pi Pico SDK 2.1.1. Install the SDK and ARM GNU toolchain, set `PICO_SDK_PATH`, then run `cmake -S firmware -B firmware/build -DPICO_BOARD=pico` and `cmake --build firmware/build`. Copy `bench_buddy.uf2` to the Pico in BOOTSEL mode. The host-side matrix decoder can be tested with `gcc -std=c11 -Wall -Wextra -Werror tests/test_matrix.c firmware/src/matrix.c -o /tmp/test_matrix && /tmp/test_matrix`.

## Netlist / schematic status

`schematic/netlist.csv` is the authoritative wiring representation; it records the mux pin mapping and protection components. No native KiCad capture is included, and the board layout is not designed. Prototype wiring and electrical performance have not been validated. Inspect the netlist and perform continuity checks before connecting a cable.

## Protocol

USB CDC emits one JSON line per scan: `{"contacts":8,"matrix":[[0,1,...],...]}` where each matrix cell is 1 for continuity, 0 for open, and -1 for ambiguous/multiple paths. The host decoder is in `firmware/src/matrix.c`; no network or remote control is implemented.

## Safety and verification

For unpowered, low-voltage cables only. Not a cable-certification instrument; not for mains, medical, automotive safety, or other life-critical wiring. Never attach a cable still connected to equipment. Firmware host logic is build-tested; Pico SDK firmware is not build-verified on this machine. No KiCad/ERC or physical tests were run. All readings and current limits require prototype verification.