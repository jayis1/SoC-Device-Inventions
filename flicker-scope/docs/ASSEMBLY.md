# Assembly and bring-up

1. Use a Raspberry Pi Pico (RP2040) module; do not build the RP2040 support circuit from this reference.
2. Assemble the photodiode/load network from `schematic/NETLIST.md`; orient BPW34 by its datasheet, not by assumption. Fit D1 inside an opaque tube.
3. Before inserting the Pico, check for solder bridges. With Pico powered from USB only, measure 3V3-to-GND and confirm no short.
4. Connect sensor header: 3V3, ADC_SENSE, GND. With no illumination, confirm ADC node remains between 0 and 3.3 V. Disconnect immediately if it goes outside range.
5. Build and flash per device README. Use a known LED source, then cover/uncover the probe; inspect streamed ADC samples.

Do not connect to mains wiring. Do not attach any external powered source to the ADC. Verify wiring and current limits before use. Optical readings are relative and uncalibrated.
