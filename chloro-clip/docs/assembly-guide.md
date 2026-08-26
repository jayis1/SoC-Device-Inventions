# Chloro Clip Assembly Guide

## 1. PCB Bring-Up Order

1. Assemble the power path first: USB-C, charger, buck-boost, battery connector.
2. Verify 3.3 V regulation under idle and flash-pulse load.
3. Assemble the ESP32-S3 module, USB/UART path, and boot circuitry.
4. Add the I2C peripherals: OLED, RTC, TMP117, VEML7700, DAC, ADC.
5. Populate the analog front end last: photodiode, OPA320, filter network, guard ring clean-up.
6. Add LED drivers and current-limited bench flash tests.

## 2. Optical Module

- Mount the blue measuring LED and photodiode at opposing 45° angles to reduce direct feedthrough.
- Place the long-pass filter directly in front of the photodiode.
- Line the optical cavity with matte black tape or black resin.
- Keep the flash LED thermally isolated from the TMP117 leaf temperature pad.

## 3. Clip Mechanics

- Use a torsion spring that gives repeatable pressure without damaging thin leaves.
- The hall switch should toggle only when the clip is fully closed.
- Add replaceable silicone pads on both jaws.
- For dark-adapted workflows, include a snap-on opaque cap.

## 4. TIA Layout Rules

- Keep photodiode-to-op-amp trace length under 8 mm.
- No digital traces under the TIA input node.
- Surround the inverting input region with a grounded guard ring.
- Separate flash LED return current from analog ground return.

## 5. First Optical Calibration

1. Close the clip with no leaf inserted.
2. Record dark offset for 5 seconds.
3. Insert a healthy dark-adapted leaf.
4. Sweep measuring LED DAC from low to moderate current.
5. Pick the lowest current that gives stable Fo while keeping ADC headroom for Fm.
6. Increase flash current until Fm no longer increases materially.

## 6. Validation Targets

Typical expectations for a healthy unstressed leaf:

- Fo: stable and repeatable within ±3%
- Fv/Fm: ~0.79 to 0.84
- ΦPSII under moderate light: ~0.35 to 0.65
- NPQ: lower under mild light, higher under excess light or stress

## 7. Field Use Notes

- Wipe moisture from the clip before measurements.
- Avoid direct sun entering the optical tunnel during open-clip handling.
- Allow the flash LED to cool during repeated teaching demos.
- Re-zero dark current when ambient temperature changes sharply.
