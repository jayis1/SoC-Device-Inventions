# Assembly and verification checklist

1. Confirm all module and cell datasheets. The BOM's load-cell line intentionally has no fabricated manufacturer part number: select a documented 100 N, four-wire S-beam cell compatible with the excitation voltage and fixture.
2. Mount cell with the vendor's specified fasteners and orientation. A rigid frame, guided crosshead, travel stop, transparent guard, and line-end clamps are required but are not part of the electronics BOM.
3. Assemble the Pico/HX711 circuit per `../schematic/connection-netlist.md`; confirm Pico GP2/GP3 signals and HX711 package pinout before wiring. Never apply 5 V to Pico GPIO.
4. With no line load and a stable mounted fixture, connect USB and allow the firmware tare to complete. An HX711 timeout must be resolved before use.
5. Apply at least three known forces across the intended range, using calibrated masses or a calibrated force reference. Repeat loading/unloading to check hysteresis. Record a regression-derived scale in counts/newton, configure firmware, rebuild, and repeat the check. The current build leaves scale zero and thus emits `NA` for force.
6. Run several matched sacrificial line samples with identical line, knot dressing, clamp geometry, and pull rate. Keep people out of the line path; stop if the fixture shifts or any component deforms.

No measured performance claims are made. This checklist is a proposed validation procedure, not a report that testing has happened.
