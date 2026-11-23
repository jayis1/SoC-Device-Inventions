# Assembly

1. Use a Pico module on a nonconductive base; power only from USB.
2. Wire J1, R1 and C1 as in `../schematic/netlist.md`. Put C1 at the GP2/GND connection. Connect buttons GP14 and GP15 to ground when pressed.
3. Before connecting the switch, inspect for shorts between 3V3 and GND. Do not apply an external voltage to GP2.
4. Build firmware with Pico SDK 2.1.1, flash UF2 by holding BOOTSEL, and open USB CDC serial at 115200 baud.
5. Verify idle gives no event; manually actuate once and check one `closure` JSON record. Confirm timing against a known interval before comparisons.

The reed capsule is fragile; secure the leads and strain-relieve wiring. This reference design has no enclosure, PCB, or verified EMC performance.
