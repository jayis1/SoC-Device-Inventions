# Assembly guide

## PCB and layout

Use a four-layer 80 x 55 mm board: top signals/components, inner 1 solid ground, inner 2 split 3V3_DIG/3V3_A, and bottom signals. Put the ESP32-S3 antenna at a short board edge with the module maker's copper and component keep-out. Keep the AD5933/RFB/cup connector inside a guard ring driven from the excitation buffer; do not route the high-impedance sense input under the radio, microSD, or switching inductor.

Place the OPA320 and ADS122C04 at the piezo connector. Route load-cell pairs as differential Kelvin traces and place the 100 ohm/100 nF input filter adjacent to HX711. Join AGND and DGND at the TPS63031 output return only.

## Build order

1. Reflow power ICs, then confirm 3V3_DIG and 3V3_A are within 3.20–3.40 V with no cup installed.
2. Program the ESP32-S3 bootloader and verify I²C sees AD5933, TMP117, SHT45, MAX17048, and ADS122C04.
3. Bond TMP117 to the outside of the metal cup using thin electrically insulating epoxy; allow it to cure fully.
4. Install the load cell beneath the cup shelf: one end fixed to the enclosure, the other end to the shelf. Do not clamp both ends of the sensing beam.
5. Install the piezo disc beneath the shelf with a small dot of flexible adhesive. It must not bear sample weight.
6. Fit the cup liner, electrode connector, OLED, buttons, and battery. Verify the metal cup cannot contact the PCB ground or battery tabs.

## Initial calibration

With a clean, empty, dry cup, invoke `EMPTY CUP`. For each crop, prepare six or more sealed sample lots across the expected moisture range. Determine reference wet-basis moisture with the recognized oven method for that crop, log impedance magnitude and grain temperature, and run `scripts/calibrate_profile.py`. Put the produced coefficients in the profile table, then validate on independent samples.

## Acceptance checks

- Empty-cup magnitude repeats within 0.5% over ten runs.
- A 100.0 g check mass reports 99.8–100.2 g after tare.
- TMP117 is within 0.3 °C of a traceable thermometer after five minutes.
- A fingertip tap on the cup produces a piezo event but no sustained count with no tapping.
- Confirm microSD CSV and Wi-Fi JSON field values agree with the display.
