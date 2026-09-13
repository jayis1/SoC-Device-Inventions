# Assembly guide

## Mechanical build

1. Machine or procure a 250 mL vacuum-rated chamber. Use polycarbonate at least 6 mm thick (not acrylic), a replaceable silicone gasket, an outward-opening lid, and a clear external shield.
2. Fit a 45 kPa vacuum relief valve, check valve, filtered vent, and 4 mm tubing. Put the pump outside the chamber.
3. Bond PZ1 to the center of the lower platen with thin cyanoacrylate; bond PZ2 to the opposite wall. Route their twisted pairs away from the pressure tubes.
4. Mount the TMP117 against the inside wall under Kapton tape. Confirm the reed interlock reads closed only with the lid fully latched.

## Electrical checkout

1. Before connecting a battery, inspect for shorts and power the board from a current-limited 5 V supply.
2. Confirm 3V3_DIG and 3V3_A are 3.3 V. Verify I2C addresses and that pressure rises toward ambient when vented.
3. With the chamber empty and lid open, verify pump and vent polarity. With lid closed, command -10 kPa first; ensure the mechanical relief opens before -45 kPa.
4. Run a 2.8 kHz burst and confirm the pickup envelope is centered near 1.65 V with no ADC clipping.

## Profile setup

For every package format, collect at least 20 known-good and 10 deliberately defective (where permitted) units under the same chamber geometry. Store median pressure-decay/ring-down values and robust standard deviations. Tighten limits only after confirming repeatability across operators, temperature, and lot variation.
