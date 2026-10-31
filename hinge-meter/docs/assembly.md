# Assembly checklist

1. Confirm the Pico board revision and HX711 module pin labels; do not rely on wire colors for the load cell.
2. With USB disconnected, wire grounds and 3V3 first, then GPIO connections per `../schematic/netlist.md`.
3. Inspect for shorts between 3V3 and GND. Connect to a current-limited USB source; expected current is below 50 mA.
4. Build/flash using the Pico SDK instructions in the device README. Capture USB CDC output.
5. Mount load cell so the force is perpendicular to the 100 mm lever. Keep hands clear of the hinge pinch point.
6. Zero with no force, then apply two known weights to determine counts per newton. Verify increasing and decreasing load and compare angle against a protractor.

No assembly, calibration, or hardware test has been performed for this reference design.
