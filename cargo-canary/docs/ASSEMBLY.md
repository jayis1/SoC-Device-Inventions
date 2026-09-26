# Cargo Canary assembly and bring-up guide

## Before ordering

1. Recreate/verify all symbols and footprints against current manufacturer land patterns; this repository is a reference design.
2. Run KiCad ERC and DRC on the exact library release used for fabrication. Obtain the BG95 hardware design, RF, GNSS, and carrier-certification guidance directly from Quectel.
3. Have an RF engineer review the 50 ohm LTE/GNSS feeds, antenna keep-outs, ground via fence, battery pulse reservoir, and four-layer stackup.
4. Select a permitted cell and shipping configuration. Primary lithium transport, cellular radio operation, and customer data retention each require their own compliance review.

## Assembly order

1. Place passives, TPS62745, TCA9548A, W25Q128, sensors, and nRF52840; inspect polarized parts and QFN wetting by X-ray if available.
2. Place the BG95-M3 only after verifying solder-mask and paste aperture requirements. Keep its RF region free of copper where the modem reference layout requires it.
3. Add display connector, reed lead pads, SWD fixture pads, and battery connector. Do not fit the radio antennas until post-reflow wash/dry is complete.
4. At current-limited bench supply (3.6 V, 100 mA), verify the 3V3 rail. Confirm no rail shorts before inserting the primary cell.
5. Program a signed development image through SWD. Verify I2C channel isolation, temperature/RH response, accelerometer interrupt, flash erase/program, display refresh, and battery ADC calibration.
6. With an approved SIM and test account, validate modem PWRKEY timing, TLS certificate checking, no-fix timeout, queue retention after forced network loss, and acknowledgement replay handling.

## Acceptance checks

- A 10-minute temperature excursion opens only after the configured consecutive-sample threshold and closes only after the configured recovery threshold.
- Disconnecting the reed loop creates a tamper event; covering/uncovering light alone remains `SUSPECT_OPEN`.
- Corrupting a flash record produces a CRC failure and never a fabricated decoded event.
- Removing cellular service retains queued records, enforces the retry budget, and preserves local display/logging.
- Measure modem TX rail dip under worst signal conditions. If rail minimum or cell pulse rating is violated, stop and redesign the energy store.

## Mechanical notes

Use a gasketed enclosure with sensor vents that do not permit splash ingress. Place SHT45 behind a hydrophobic membrane and thermally isolate it from modem, regulator, display, and cell. Mount the reed/magnet so normal vibration cannot open the loop. Label the tag with battery chemistry, disposal route, radio identifiers, and its uncalibrated-reference status.

Designed and documented by jayis1.
