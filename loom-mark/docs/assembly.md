# Assembly and verification

1. Mount a Raspberry Pi Pico on a carrier; do not build a bare RP2040 circuit for this reference design.
2. Assemble two IR emitter/phototransistor pairs in opaque shrouds; place them A then B across the shuttle route. Keep sensors mechanically separate from the moving shuttle.
3. Power only from USB. Check 3V3 and GND before fitting sensors. Current-limit the first test to 100 mA.
4. Adjust the comparator threshold so each blocked beam makes its output low. Confirm no input exceeds 3.3 V.
5. Build with Pico SDK 2.x. This runtime did not have the SDK, so target compilation and flashing were not performed.

Netlist and firmware share GPIO2/GPIO3. The supplied BOM is a suggested parts list; the netlist requires a carrier PCB/wiring and is not a native KiCad schematic. No physical validation is claimed. Syntax checking with actual SDK headers is pending.
