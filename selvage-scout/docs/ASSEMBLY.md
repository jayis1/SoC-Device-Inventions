# Assembly and setup

1. Mount a Raspberry Pi Pico and a 3.3 V-compatible TCRT5000 analog module facing a short rounded nylon comb. Fix sensor height and ambient-light shielding before comparisons.
2. Wire exactly as in `../schematic/netlist.md`. Confirm module VCC is 3.3 V and measure analog OUT stays between 0 and 3.3 V before connecting GP26.
3. Install Pico SDK 2.1.1 only when a future hardware adapter is available. Current checked-in firmware is a host-testable core, not a flashable image.
4. Host verification: configure/build CMake in firmware and run CTest. To develop a Pico adapter, sample ADC0 and pass readings/time to `scout_sample`; debounce SW1/SW2 and format JSONL in the adapter.
5. Fix sensor, comb, material conditioning, stroke length, and draw speed. Record these conditions with every comparison. Test a blank run and known loose-fiber sample; calibrate threshold experimentally.

Do not use for fabric certification, product safety, or material acceptance decisions. Hardware and measurement repeatability are unverified.
