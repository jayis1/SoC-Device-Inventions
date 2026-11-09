# Ink Witness

Offline fountain-pen ink flow comparison fixture. A Raspberry Pi Pico records timed optical interruptions as a pen is drawn across a replaceable test strip, producing repeatable relative line-continuity and wetting-event records over USB CDC. It is a comparative workshop aid, not a viscosity instrument or a measure of archival permanence.

## Design status

Reference design; firmware core host-buildable, but no target build or physical test is claimed. Schematic is a wiring reference, not a KiCad fabrication design. Validate sensor alignment and thresholds on the chosen strip material.

## Architecture and power

USB 5 V powers a genuine Raspberry Pi Pico (RP2040); its onboard regulator supplies 3.3 V logic. Budget 50–100 mA USB pending measurement. A TCRT5000 reflective sensor module powered at 3.3 V faces the strip at 3–5 mm; its digital comparator output feeds GP14. A normally-open RUN button from GP15 to ground starts/stops a trace. Internal pull-up on GP15. No battery, radio, heater, or actuator. Do not power the sensor from 5 V or allow a 5 V output into GPIO.

Block: strip under reflective sensor → GP14 edge capture → RP2040 event core → USB CDC JSONL. RUN button → GP15.

## Pin assignments

- GP14: sensor digital output, active low; module powered 3.3 V; shared ground.
- GP15: RUN switch to GND, internal pull-up.
- USB: power, programming, CDC output. SWD pads available for recovery.

## BOM

See `hardware/BOM.csv`. Use a Pico module with onboard USB/regulator. Sensor modules vary: verify 3.3 V operation and output polarity before connection. The schematic/netlist is authoritative for this reference design.

## Firmware

Portable C event core in `firmware/ink_witness.c`; host test uses the system C compiler. Input adapter and Pico SDK integration remain future work. The core debounces transitions, records monotonic millisecond offsets and emits JSONL event records. Build/test:

`cc -std=c11 -Wall -Wextra -Werror -Ifirmware firmware/ink_witness.c firmware/test_ink_witness.c -o /tmp/ink_witness_test && /tmp/ink_witness_test`

A session emits `BEGIN`, `EDGE` and `END` JSON objects. Compare counts and intervals using identical pen, paper, pressure, speed, sensor gap, and strip width; optical readings are strongly affected by ink color and substrate.

## Assembly and safe first power

Mount the sensor perpendicular to an unprinted calibration strip; secure the paper so the pen cannot strike the sensor. Before USB connection inspect for shorts. Power from current-limited 5 V USB and verify 3.3 V at the Pico and sensor. Test sensor output with a meter before connecting GP14. Stop if the board, cable, or sensor heats. Keep ink away from electronics and clean the removable strip holder separately.

## Limitations

This records threshold crossings, not deposited ink mass, chemical identity, or standardized writing quality. Ambient light, paper texture, ink color, and geometry affect results. No KiCad CAD, target firmware build, calibration, or physical validation is provided; do not treat as a production or calibrated instrument.