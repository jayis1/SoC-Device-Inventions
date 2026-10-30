# Cell Scout

Offline coin-cell pulse-response tester for repair benches and battery drawers. It compares a cell's loaded voltage sag and recovery against the same cell's open-circuit voltage; it is a screening aid, not a capacity tester or chemistry identifier.

## Design brief

- User/scenario: a repairer sorting loose CR2032 cells before using one in a low-current sensor.
- Why not a multimeter: the controlled 10 mA pulse reveals short-term source resistance that a no-load voltage misses.
- Constraints: USB-powered, no battery charging, no cloud, low-voltage only.
- Signature mechanism: Pico firmware uses a timed MOSFET load pulse and records both immediate sag and post-pulse recovery.
- Distinction: unlike the repo's optical seed counter, this is a wired electronics-bench instrument measuring electrical source response, not counting physical objects.

## Design status

Reference design; no PCB layout or KiCad capture; unbuilt and not calibrated. Firmware compiles for the Raspberry Pi Pico SDK 2.x when that SDK is installed. The load-current target is calculated from the actual fitted resistor and cell voltage, not asserted as a precision measurement. This is not a battery capacity, safety, or remaining-life guarantee. Test only single-cell coin cells in a holder; never connect rechargeable lithium cells, multi-cell packs, or external power to the cell terminals.

## Architecture

```text
USB 5 V -> Pico board regulator -> RP2040 (ADC + timed GPIO)
CR2032 + -> 100 ohm load resistor -> N-MOSFET -> CR2032 -
cell voltage -> 100k/100k divider -> GP26 / ADC0
USB CDC -> CSV-like serial result
```

The divider presents half the cell voltage to the 3.3 V ADC. The load is switched low-side; firmware leaves the MOSFET off at reset. Use a logic-level N-channel MOSFET specified at VGS=2.5 V. At 3.2 V the nominal pulse is about 32 mA (3.2/100); resistor dissipation is about 0.10 W, so use a 0.25 W part. Pulse duration is 100 ms, once per command.

## Pin assignments

| Pico signal | Connection | Notes |
|---|---|---|
| GP26 / ADC0 | midpoint of 100k/100k divider across cell | ADC range 0–3.3 V; firmware doubles reading |
| GP15 | MOSFET gate through 100 ohm | 100k gate pulldown; active high |
| GND | cell negative and USB ground | common reference |
| VSYS/USB | Pico supply only | do not connect cell positive here |

Pico GPIO numbers are RP2040 GPIO numbers. Keep the cell measurement divider and cell holder wiring short. No wireless interface or stored personal data.

## Build and use

Requires Raspberry Pi Pico SDK 2.x and its documented ARM toolchain. From `firmware/`: `cmake -S . -B build -DPICO_SDK_PATH=/path/to/pico-sdk -DPICO_BOARD=pico && cmake --build build`. Flash `cell_scout.uf2`. Open USB CDC at 115200 baud; send `t` then Enter. The firmware reports open-circuit voltage, loaded voltage, recovery voltage and estimated pulse resistance. The calculation assumes the load resistor value and ignores MOSFET and wiring resistance; it is approximate.

## Power and safety

USB powers the Pico. The cell is electrically isolated from the Pico supply except for measurement and load ground. Do not charge or reverse-connect cells. Stop if the cell warms, leaks, or is damaged. This design has no fuse or reverse-polarity protection: observe holder polarity and test one cell at a time. No mains connections.

## Validation

Firmware source and host-side model test are included. Pico SDK build and physical measurements have not been run in this environment. `schematic/netlist.md` is the authoritative connection list; no editable KiCad schematic is supplied. Treat this as a prototype reference, not a calibrated instrument.
