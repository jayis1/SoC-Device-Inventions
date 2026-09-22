# Assembly and bring-up

Designed and documented by jayis1. This is an unverified reference design; do not use it to protect people, equipment, or valuable textiles until it has been electrically reviewed and calibrated.

1. Inspect the PCB before assembly. Confirm the RP2040 QFN land pattern, crystal load capacitors, QSPI routing, USB-C CC connections, and battery polarity against current component datasheets.
2. Reflow low-profile passives, U1–U7, crystal, then connectors. Hand-solder the load-cell connector, display, battery lead, buzzer, and debug pads. Keep flux residue off the ADS1232 analog input area.
3. Before installing the LiPo, power from a current-limited 5 V bench supply at 100 mA. Verify 3V3 is 3.20–3.40 V and no IC heats unexpectedly. Verify charger termination separately with a protected cell.
4. Program a minimal RP2040 blink/USB test through SWD. Confirm USB D+/D- routing and boot recovery before connecting the load cell.
5. With the load cell unloaded, record at least 50 ADS1232 readings and establish noise. Install a known reference mass and check that the sign is positive. Run `python3 scripts/check_calibration.py capture.csv` using two endpoints with `known_cN,raw_count` headers.
6. Position the diametric magnet according to the AS5600 datasheet air-gap range. Rotate the take-up shaft and verify a monotonic 0–4095 count with no magnet collision.
7. Set conservative tension limits only after controlled loom trials. Verify slack, over-tension, stalled-motion, display, LED, buzzer, USB recovery, brownout, and watchdog behavior.

Battery safety: use only a protected single-cell LiPo, provide strain relief, do not charge unattended, and stop if the cell is hot, swollen, punctured, or damaged.
