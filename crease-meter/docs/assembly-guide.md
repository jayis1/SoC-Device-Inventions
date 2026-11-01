# Assembly notes

1. Mount Pico and sensor facing a blackened optical slot; shield from room light.
2. Fit LED and sensor opposite each other with a repeatable coupon stop.
3. Wire only to Pico 3V3/GND/GPIO pins per schematic/NETLIST.md. Inspect transistor pinouts from the selected datasheet.
4. Run host core tests; firmware does not yet implement target ADC sampling, so do not expect a standalone instrument.
5. For future target bring-up, measure dark and illuminated ADC ranges and establish a calibration against matched uncreased paper controls.

This reference is unbuilt and electrically unreviewed. Disconnect USB before changing wiring.
