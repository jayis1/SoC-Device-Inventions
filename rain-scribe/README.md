# Rain Scribe

Offline manual rain-garden infiltration logger built around a Raspberry Pi Pico (RP2040). It timestamps water-level crossings in a clear standpipe and exports USB CDC JSONL, letting a gardener compare relative infiltration between plots without cloud service or an electronic probe in soil. It is an observational aid, not a calibrated hydrology instrument.

## Status and scope

Reference design; portable C event core and host tests are provided. No target firmware build, fabricated PCB, calibration, or physical test is claimed. The schematic is an editable wiring/netlist reference, not a KiCad fabrication design. Measure the standpipe geometry and validate float travel before field use.

## Architecture and power

A USB-powered genuine Raspberry Pi Pico supplies 3.3 V to an external vertical reed-switch ladder. Five normally-open reed switches at 10 mm spacing share ground and connect to GP10–GP14 with internal pull-ups; a small magnet in a buoyant float closes each switch as water rises. GP15 is a normally-open manual START/MARK button to ground. USB provides power and CDC records. No battery, radio, mains, or actuator. Limit ladder wiring to 3.3 V GPIO, keep the assembly dry, and do not immerse the Pico or connectors.

Block: standpipe water → floating magnet → reed ladder → GP10..14 → event core → USB CDC JSONL. Button → GP15.

## Pin assignments

- GP10..GP14: active-low reed ladder, levels 0..4, each switch to GND; 3.3 V internal pull-up.
- GP15: active-low momentary START/MARK button to GND; internal pull-up.
- USB: Pico power, programming, and CDC output; SWD pads for recovery.

Debounce all inputs in the target adapter. Level readings indicate threshold crossings only; the host-testable core accepts monotonic millisecond timestamps and level values.

## BOM and build

See `hardware/BOM.csv`; `schematic/rain_scribe.net` describes all connections. Mount reed switches externally beside a transparent standpipe at measured, equal intervals; the float must move freely and its magnet must actuate each switch. This is a hand-wired prototype reference; no PCB footprints or connector orientation are specified. Test each switch with a meter before connecting the Pico.

## Firmware

`firmware/rain_scribe.c` is a portable C event core; `firmware/test_rain_scribe.c` exercises session start, level events, and mark events. A Pico SDK GPIO/USB adapter is intentionally not included. Host test:

`cc -std=c11 -Wall -Wextra -Werror -Ifirmware firmware/rain_scribe.c firmware/test_rain_scribe.c -o /tmp/rain_scribe_test && /tmp/rain_scribe_test`

The core emits JSON lines for BEGIN, LEVEL, MARK, and END. Compare plots only with the same standpipe diameter, level spacing, placement, and water application. Convert height change to volume using the measured standpipe cross-section; no conversion is asserted by firmware.

## Safe first use and limitations

Keep electronics above splash level. Inspect wiring for shorts, power from a current-limited USB source, and verify 3.3 V before connecting sensors. Do not use near mains equipment. Reed switches can chatter or miss a crossing if the float binds or the magnet is misaligned. This design does not measure soil moisture, rainfall, absolute infiltration rate, or regulatory drainage compliance. No target build, KiCad ERC, or hardware validation is claimed.
