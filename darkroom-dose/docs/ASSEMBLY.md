# Assembly and bring-up

Status: reference design only. Obtain the exact Pico board, and prototype the unresolved photodiode amplifier before assembling a complete unit. The netlist intentionally marks the TIA feedback network as a hold point; do not wire the photodiode directly to GP26.

1. Confirm all parts are 3.3 V logic compatible; keep the external enlarger disconnected.
2. Inspect the Pico board and solder only its header/connectors and low-voltage user-interface parts.
3. Validate the OPA333 TIA experimentally with a current-limited bench setup and a known light source. Select feedback R/C values based on dynamic range, stability, and output swing; measure the ADC node against ground and ensure it remains between 0 and 3.3 V.
4. Connect GP26 to the validated TIA output, buttons GP6/GP7, and optocoupler input GP8 through its resistor. Verify default-off with a meter before connecting any receiver.
5. Connect USB. Check 3V3 rail (expected 3.2–3.4 V), flash the UF2, and capture serial output at 115200 baud.
6. Verify shade/illuminate changes raw ADC samples; test stop and 120 s timeout with the isolated output unloaded.
7. Validate repeatability using controlled test strips and fixed enlarger geometry. Do not treat ADC counts as calibrated photographic exposure.

No PCB, mains interface, enclosure rating, or measured current budget is provided. Any external SELV interface requires its own design review. Never wire mains to this circuit or its prototype enclosure.