# Assembly and calibration

1. Build a rigid base with a freely moving lever, parallel pressure bar, and hard stop. Mount load cell so force is applied axially; avoid side load. Attach diametric magnet concentrically to pivot and AS5600 module across its specified air gap.
2. Wire using schematic/netlist.md and BOM. Confirm module logic voltage and pullups. With sensors disconnected, inspect for shorts, current-limit USB to 100 mA, confirm 5 V and 3.3 V rails; connect each module individually and stop for heating or rail collapse.
3. Connect USB CDC; inspect raw serial output. HX711 not ready within 100 ms yields zero in this prototype and must be treated as invalid (firmware refinement needed). Check sensor errors and button transitions.
4. Calibrate force using traceable known masses in the fixture orientation, record raw ADC readings, tare unloaded, and fit counts per newton. Calibrate travel using ruler-measured positions across the motion; fit angle-to-distance for the actual linkage. Repeat across range and retain calibration notes.
5. Compare identical paper samples with fixed orientation and loading speed. No absolute paper grade or acceptance threshold is implied. Keep fingers clear of the lever; use a mechanical overload stop.

Unverified: mechanics, accuracy, linearity, calibration, target firmware build, schematic ERC, and physical operation.
