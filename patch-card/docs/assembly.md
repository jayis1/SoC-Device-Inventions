# Assembly and verification

1. Fit the Pico and sensor modules on an insulated carrier; secure wheel axle and magnet so it cannot rub the sensor board.
2. Connect only the listed 3.3 V-compatible module pins. Verify each module's pin labels and output type before wiring; breakout boards vary.
3. Align the optical gates at the selected stitch feature and shield them from room light. Adjust the module comparator threshold.
4. With USB disconnected, check continuity from 3V3 to GND. Power Pico alone over USB, then attach sensors one by one.
5. Run `cmake -S firmware -B build && cmake --build build && ctest --test-dir build --output-on-failure` for the portable measurement core.
6. The delivered firmware does not yet include Pico SDK GPIO, I2C, or USB adapters. Do not flash this host test as target firmware.

A schematic connectivity reference is in `schematic/netlist.md`. No PCB is supplied. Validate breakout pinouts against their own documentation; do not rely on physical connector orientation shown in marketing images.
