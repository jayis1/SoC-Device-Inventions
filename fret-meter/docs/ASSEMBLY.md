# Assembly and bring-up

1. Fit the Pico to a carrier without powering it. Inspect for shorts between 3V3 and ground.
2. Wire RV1 as a divider, R1 between wiper and GP26, C1 from GP26 to AGND, and SW1 from GP14 to AGND. Verify the pin numbers against the Pico board revision.
3. Power from a current-limited USB source (initial limit 100 mA). Confirm 3V3 is 3.2–3.4 V before connecting the pot. Stop on heating, odor, or unexpected current.
4. Install Pico SDK 2.1.1 from its official upstream release, build as in the top-level README, flash UF2 over BOOTSEL, and open the USB CDC port at 115200 (CDC baud is nominal).
5. Confirm button records yield raw values within 0–4095 and changing carriage position changes the value monotonically. Confirm it returns when released and does not bind.
6. Calibrate on two measured carriage positions using gauge blocks. Compare several intermediate positions against a feeler gauge; characterize hysteresis and force-induced string displacement before using measurements.

The carrier and mechanical gauge require physical review. No device has been fabricated, calibrated, or bench-tested. Never place a sharp tip against a finished instrument.
