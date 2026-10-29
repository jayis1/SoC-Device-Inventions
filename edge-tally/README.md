# Edge Tally

Firmware: host-buildable core (verified with GCC) · Schematic: conceptual netlist only (KiCad ERC not run) · BOM: common parts; sourcing not independently verified. Prototype reference design; not fabricated or electrically validated.

## Why this exists

Edge Tally gives repair technicians a safe, isolated way to count dry-contact operations on a disconnected switch or relay during endurance and bench tests. Unlike the repository's bounce sampler, it counts debounced actuations over long runs and exports a compact CSV record; it does not connect to mains or energize the device under test.

## Evening demo

Assemble the low-voltage input and Pico, connect a normally-open contact, plug in USB, and watch each stable closure increment the count. The host core test exercises debounce and rollover-safe elapsed-time handling.

## Architecture

```text
Dry contact -> 10k pull-up/input protection -> RP2040 GPIO
USB 5 V -> Raspberry Pi Pico regulator -> 3.3 V logic
RP2040 -> USB CDC -> host terminal / CSV
```

The target is a Raspberry Pi Pico (RP2040 module), avoiding a custom RF or power design. External device under test must be disconnected from all hazardous voltages; input is not galvanically isolated.

## Pin assignments

| Function | Pico pin | GPIO | Notes |
|---|---:|---:|---|
| Contact input | 15 | GP11 | Active low, internal pull-up, debounce |
| Status LED | 25 | GP25 | Pico onboard LED |
| USB CDC | USB | — | Native Pico USB interface |

## Power

USB 5 V powers the Pico; its onboard regulator supplies 3.3 V. Design load is <30 mA excluding the host, no battery. Contact input is 3.3 V logic only. Never connect an externally powered circuit.

## Files

- `schematic/netlist.md`: connectivity and safety notes (not a KiCad schematic).
- `hardware/BOM.csv`: reference parts list.
- `firmware/`: portable C debounce/counting core and host test.
- `docs/ASSEMBLY.md`, `docs/API.md`: build and protocol notes.

## Build and test

`cc -std=c11 -Wall -Wextra -Werror firmware/edge_tally.c firmware/test_edge_tally.c -o /tmp/edge-tally-test && /tmp/edge-tally-test`

The portable core is verified on host GCC. RP2040 USB/GPIO target integration and flashing are not included; firmware is a reusable logic core, not a complete Pico SDK image.

## Limitations and safety

This is an isolated-by-disconnection, low-voltage bench aid, not a safety instrument or production counter. No optical isolation or galvanic barrier is provided. Do not attach to mains, energized relay contacts, or unknown circuits. Contact bounce tolerance and count accuracy require hardware testing. No PCB, KiCad ERC, target build, or physical tests have been performed.
