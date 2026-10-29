# Assembly and bring-up

1. Inspect the purchased SS49E data sheet for lead order; do not assume the generic TO-92 order.
2. Mount one small diametric magnet on the jig so one north/south cycle occurs per shaft revolution. Fix magnet securely; keep it away from loose wire and tools.
3. Wire the Pico via USB only. Connect sensor J2 in order 3V3, GND, OUT and piezo between GPIO15 (through 1 kΩ) and GND.
4. Before attaching the Hall sensor, power the Pico and verify 3.3 V at its 3V3(OUT) pin. Disconnect USB before wiring.
5. Flash the built UF2 using BOOTSEL. Open USB CDC and confirm periodic status JSON.
6. Rotate slowly; verify a single `turn` event per full revolution. Change the gap/alignment if idle ADC values do not cross below 1800 and above 2295.

Stop if the Pico or sensor warms, supply is shorted, or ADC remains at a rail. This reference has no reverse-polarity protection beyond the Pico's USB input; never apply external power to the sensor connector. Keep sensor wiring short. No physical prototype or measured result is claimed.
