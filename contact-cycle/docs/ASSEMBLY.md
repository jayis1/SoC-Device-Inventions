# Assembly and bring-up

1. Populate a prototype only after checking Pico pin labeling and BAT54 SOT-23 pinout against the manufacturer data sheet. The netlist is a functional reference, not a board layout.
2. Keep the device under test disconnected while inspecting continuity between 3V3, GND, and GP16. Confirm there is no direct 3V3-GND short.
3. Power the Pico by USB from a current-limited source. Measure 3V3 before connecting the input fixture.
4. Attach only a passive, unpowered, isolated dry-contact switch. No mains, automotive, powered logic, or unknown wiring.
5. Firmware concept samples GP16, debounces for 5 ms, and timestamps accepted state transitions at millisecond resolution. Verify with a logic analyzer before relying on any result.
6. Export CSV and check increasing timestamps. Full-scale firmware integration and hardware characterization remain future work.

Stop if the board, cable, or DUT warms, behaves unexpectedly, or measures voltage while disconnected from this unit.
