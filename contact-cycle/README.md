# Contact Cycle

An offline RP2040 reference design for measuring dry-contact switch bounce and cycle-to-cycle closure timing using a protected, low-voltage input. Unlike relay analyzers, this is intended for manually actuated panel switches, limit switches, and foot switches; it neither drives nor energizes the device under test. It reports raw edge timing so the operator can compare specimens without asserting a pass/fail standard.

Status: schematic/netlist reference and portable C event core; no KiCad-native schematic, Pico target adapter, calibration, or physical validation supplied. Do not treat it as a certified tester.

## Design

- Controller: Raspberry Pi Pico (RP2040 module, 3.3 V GPIO).
- Input: two screw-terminal leads across an isolated-from-mains dry contact only. On-board 3.3 V pull-up is 10 kΩ; a 1 kΩ series resistor limits accidental input current. 100 nF to ground plus a BAT54 clamp pair provide basic transient conditioning. The clamp is not galvanic isolation or a substitute for ESD qualification.
- Trigger: tactile button on GP15 starts/stops one capture; GP14 status LED via 1 kΩ.
- Contact input: GP16, active-low; target sampling proposal is 1 kHz, with 5 ms debounce for cycle classification. The core stores millisecond transition timestamps; shorter bounce events are not measurable.
- Supply: USB 5 V into Pico's onboard regulator; DUT stimulus is limited to the board's 3.3 V pull-up and about 0.3 mA nominal contact current. Do not connect to powered, mains, or unknown circuits.
- Output: USB CDC CSV records; no radio, storage, or network service.

## Build and use

The firmware core in `firmware/src/contact_cycle.c` is portable C11 and host-buildable with CMake. It demonstrates bounded event capture and CSV output; Pico GPIO/USB integration remains a target-specific follow-up. Configure/build: `cmake -S firmware -B build && cmake --build build`. Run host check: `cc -std=c11 -Wall -Wextra -Werror -pedantic -DCONTACT_CYCLE_TEST firmware/src/contact_cycle.c -o /tmp/contact-cycle-test && /tmp/contact-cycle-test`.

Connect only a passive dry contact with the unit unpowered. Attach leads, connect USB, open serial at 115200, start capture, actuate manually, stop capture, and save CSV. Each record is monotonically timestamped in milliseconds with debounced state. The example core does not implement production USB serial transport.

## Novelty and limitations

The repository includes relay/solenoid signature analysis and several timing fixtures, but this design isolates a simple operator-driven dry-contact bounce/cycle dataset workflow, with no coil drive, actuator sensing, or automated diagnosis. This is a low-voltage measurement concept, not a unique measurement principle. Timing accuracy, EMC immunity, clamp behavior, and Pico integration are unverified. Use a current-limited USB source; never connect to energized wiring.

## Contents

- `schematic/netlist.md`: pin-level connection reference
- `hardware/BOM.csv`: prototype module and protection parts
- `firmware/`: portable event-recording core and host test
- `docs/ASSEMBLY.md`: safe setup and bring-up checklist
- `docs/API.md`: CSV contract
- `scripts/check_bom.py`: BOM structure validator
