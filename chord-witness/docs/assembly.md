# Assembly and bring-up

This reference design is not fabricated. Assemble the Pico and low-voltage signal board only after reviewing the netlist and confirming BAT54S polarity and all footprints. Use a current-limited USB source. With pickup disconnected, confirm VBIAS is near 1.65 V and ADC input stays within 0–3.3 V. Verify continuity from J2 ground to Pico ground, then attach a passive piezo pickup. Never attach an amplifier output or unknown powered source.

Flash a Pico SDK adapter that samples GP26/ADC0 at 8 ksample/s and sends 256-sample windows to the portable estimator. Confirm readings against a function generator before relying on reported pitch. The adapter, native KiCad schematic, PCB routing, physical assembly, and calibration are not supplied or tested.
