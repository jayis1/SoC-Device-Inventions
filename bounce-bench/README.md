# Bounce Bench

Design status: unverified reference concept; not fabricated or physically tested.

A low-voltage bench instrument for keyboard, arcade-button, and switch makers: characterize a contact's bounce duration and edge count instead of guessing a debounce interval. Unlike the repository's cable continuity mapper, this measures time-domain behavior of a single switch; unlike a keyboard force/travel rig, it needs no actuator or mechanical stage. The useful differentiator is a timestamped contact trace from a tiny RISC-V MCU, with a serial CSV export.

Target: hobbyist bench use; USB 5 V input, 3.3 V logic; 50 × 25 mm board; target parts cost under $12 at quantity 10 (estimate, not quoted). No battery, radio, mains, or medical/safety use. Input accepts dry contacts only; do not connect external powered circuits.

## Operation

Connect a normally-open dry contact between TEST and GND. On command `T` over UART at 115200 8N1, firmware arms for 100 ms; the first falling edge starts capture. It samples the contact at 10 kHz for 20 ms, then emits `t_ms,level` CSV rows for each transition plus a summary with total transitions and last transition time. Pull-up is internal; a 1 kΩ series resistor and 3.3 V clamp limit accidental ESD/current. It is a sampled logger, not a precision oscilloscope: pulse widths below 100 µs can be missed.

## Block diagram

USB 5 V -> 3.3 V LDO -> CH32V003F4P6 -> UART USB-serial adapter
                                      <- TEST jack / 1 kΩ / clamp / pull-up

## Pin assignments

| CH32V003 pin | Function | Notes |
|---|---|---|
| PD1 | UART TX | 115200 baud to external 3.3 V USB-UART adapter |
| PD6 | UART RX | Command input; adapter TX must be 3.3 V |
| PC4 | TEST input | Internal pull-up; external dry contact to GND |
| GND | Common | USB-UART and test contact return |
| VDD | 3.3 V | 100 nF local bypass plus 4.7 µF rail capacitor |

Package is SOP-8, CH32V003F4P6. Confirm exact package pin numbering against the current WCH datasheet before PCB layout; do not treat this text netlist as a fabrication drawing.

## Power architecture and budget

| Rail | Source | Load / design limit |
|---|---|---|
| USB 5 V | USB-C receptacle, CC pull-downs | 100 mA design limit; no USB-PD negotiation |
| 3.3 V | TLV70033 LDO | MCU <20 mA; UART adapter powered separately from host |

Estimated active current: 5 mA at 5 V. No sleep/battery claim. Use a current-limited USB source for first power-up.

## BOM

See [hardware/BOM.csv](hardware/BOM.csv). Verify stock, ratings, and package details before purchase.

## Firmware

C source targets CH32V003 with the WCH EVT peripheral library. This repository does not vendor that SDK. Build requires its `ch32v00x` headers/library and RISC-V embedded GCC; exact build status is recorded below. Capture logic uses a bounded 200-sample buffer, no dynamic allocation, and a safe idle-high input.

## Build and flash

Use WCH EVT firmware library for CH32V003 and a compatible RISC-V GCC toolchain. Integrate `firmware/main/main.c` and `pins.h` into the vendor's CH32V003 project, then flash via a WCH-LinkE SWIO probe. The project has not been compiled in this environment unless stated in verification below.

## Assembly

Follow [docs/assembly.md](docs/assembly.md). The connection-level schematic and pin audit are [schematic/NETLIST.md](schematic/NETLIST.md); no native KiCad schematic is claimed.

## API

See [docs/api.md](docs/api.md); command and CSV field names match firmware.

## Limitations and safety

No calibrated timebase claim; timing accuracy depends on the MCU clock configuration. Sampling is 10 kHz. Input is for an unpowered dry contact only, not a powered signal, mains, or automotive circuit. No EMC/ESD or physical tests performed. Design is not PCB-ready pending datasheet pin verification, schematic capture, and hardware validation.

## Verification

Reference files only. Firmware build and hardware behavior unverified; no KiCad ERC run because the deliverable is a textual connection netlist.