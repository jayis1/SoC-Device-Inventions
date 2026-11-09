# Leaf Press — offline herbarium pressing record

Leaf Press is a USB-powered field accessory for botanical educators and small herbaria. It records the elapsed time and press-opening events for a specimen packet, alongside operator-entered specimen codes, as newline-delimited JSON over USB CDC. It does not identify plants, measure moisture, or replace collection permits and metadata practice. Unlike the repo's leaf-area digitizer, this device tracks a specimen's pressing workflow, not geometry.

## Design status

Reference design; firmware core host-buildable. No physical prototype, calibration, or target flash has been verified. Schematic is an explicit wiring/netlist reference, not a KiCad fabrication design. Confirm board pinout and USB behavior against the chosen Raspberry Pi Pico revision before assembly.

## Hardware

- SoC: Raspberry Pi Pico module (RP2040, 3.3 V GPIO), USB-powered; use the Pico's onboard regulator and USB interface.
- Inputs: two normally-open momentary buttons to GPIO with internal pull-ups: MARK=GP14 and NEXT=GP15. Button closure is to GND; debounce in firmware.
- Status: onboard Pico LED, controlled by SDK abstraction (pin varies by Pico board revision; not required for operation).
- Connector: USB on Pico; no external supply, battery, radio, or exposed high voltage.
- Power: USB 5 V feeds Pico board; 3.3 V local regulator powers RP2040. Typical current is board-dependent; budget 50–100 mA USB, verify on actual board.

Block: USB host ↔ Pico USB CDC ↔ RP2040 firmware ↔ MARK/NEXT buttons.

## BOM

See `hardware/BOM.csv`. Use a genuine Pico or compatible board with RP2040 and onboard USB/regulator. Buttons are low-voltage momentary switches.

## Firmware and operation

`firmware/leaf_press.c` contains a portable event core. The target adapter is intentionally not included: build and flash integration with Pico SDK remains outstanding. Host test: `cc -std=c11 -Wall -Wextra -Werror -I firmware firmware/leaf_press.c firmware/test_leaf_press.c -o /tmp/leaf_press_test && /tmp/leaf_press_test`.

1. Connect the Pico by USB; host sees a CDC serial port.
2. Send `START <specimen-code>` followed by newline (ASCII, max 31 printable characters).
3. Press MARK for a timestamped event; press NEXT to close the session.
4. The core emits JSONL records with monotonic elapsed milliseconds. Keep duplicate IDs out of your collection log.

## Pin and schematic reference

`schematic/netlist.md` lists connections. Do not connect buttons to 5 V. GP14/GP15 are ordinary GPIO on the Pico module; verify board-specific pin labels. USB programming/debug is through the board's native USB and SWD pads; use a debug probe for recovery if needed.

## Limitations and safety

No specimen identification, legal collection advice, or preservation guarantee. Device is a logging aid; maintain independent field notes. No hardware validation performed. First power-up: inspect for shorts, use a current-limited 5 V USB supply, verify 3.3 V rail, then connect one button at a time. Stop if the board or cable heats, smells, or draws abnormal current.
