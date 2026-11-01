# Latch Ledger assembly and interface

## Sensor wiring

Use the purchased DRV5032FA datasheet to verify pin numbering; do not infer pin order from the package drawing. Connect its supply to Pico 3V3, ground to Pico ground, and open-drain output to GP2. Add R1 10 kΩ from GP2 to 3V3 and C1 100 nF at sensor supply. The netlist uses logical pin names and intentionally flags physical sensor pin numbers for confirmation.

Mount the magnet so the selected Hall switch changes once across the closed/open transition. Secure the assembly mechanically; this reference design is not a door safety interlock. Use USB only, never connect external 5 V to GP2 or sensor VDD.

## Bring-up

With sensor disconnected, check 3V3 and ground continuity. Connect the Pico to a USB host, flash UF2, inspect startup JSON, then connect the sensor. Confirm idle and actuated levels before cycle counting. Record sensor gap, magnet orientation, and fixture geometry with each experiment.

## Event format

UTF-8 JSON Lines over native USB CDC. `startup` announces format version 1. `open` and `close` include monotonic milliseconds since boot; `close.dwell_ms` is the measured open duration. Reboot resets time to zero. Host software must not treat this counter as a globally synchronized timestamp.
