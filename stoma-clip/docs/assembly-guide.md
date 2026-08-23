# Stoma Clip Assembly Guide

## Overview

This guide describes how to assemble the **Stoma Clip** handheld leaf porometer prototype.

## Mechanical Stack

1. Print the main enclosure and clip jaws in PETG or nylon.
2. Install brass inserts in the body shell.
3. Fit the lower jaw silicone gasket.
4. Bond the copper temperature tongue into the lower jaw recess.
5. Mount the TMP117 board or flex onto the tongue root.
6. Route the flex cable back to the main PCB.
7. Install the upper jaw chamber insert and flap valve.
8. Mount the servo so the flap fully opens and closes without binding.
9. Fit the blower and route purge tubing into the chamber manifold.
10. Install the ambient vent labyrinth foam so blower exhaust cannot recirculate.

## PCB Population Order

### Pass 1: fine-pitch ICs
- ESP32-S3 module
- TPS63031
- TPS61023
- MCP73831
- AS7341
- INA219
- DRV2605L
- PCF85063A
- optional TCA9546A

### Pass 2: passives and connectors
- all 0603 decouplers and pull-ups
- USB-C connector
- microSD socket
- OLED header
- battery connector
- flex connector for leaf tongue

### Pass 3: electromechanical parts
- buttons
- buzzer
- RGB LED light pipe
- blower leads
- servo header
- leaf-present and clip-closed switches

## Sensor Placement Notes

- Keep the **ambient SHT45** near exterior vents and away from warm regulators.
- Place the **chamber SHT45** in a sheltered cavity inside the chamber roof, not in the purge jet.
- The **TMP117 tongue** should make gentle thermal contact with the leaf but not puncture it.
- The **AS7341** needs a matte diffuser window to avoid angular bias.

## Bring-Up Checklist

1. Power from bench supply at 3.8 V current-limited to 0.6 A.
2. Verify 3.3 V rail.
3. Enable 5 V actuator rail and verify no-load current.
4. Check USB enumeration.
5. Probe I2C bus for all devices.
6. Test blower PWM.
7. Sweep servo and confirm full flap travel.
8. Verify OLED and SD logging.
9. Run ambient-equalization test with clip open.
10. Perform wetted-membrane calibration.

## Chamber Leak Test

1. Place a moist calibration membrane inside the clip.
2. Close the chamber.
3. Observe RH rise over 15 s.
4. Repeat while intentionally loosening the gasket.
5. Compare fit R² and slope drop.
6. Reject builds with persistent poor seal linearity.

## Field Calibration

- zero sensors in shade for 2 minutes
- cross-check both humidity sensors in open air
- confirm leaf tongue reads within 0.3 °C of a reference film thermistor on a metal shim
- store chamber-volume and gasket-factor coefficients in NVS

## Cleaning

- wipe gaskets with DI water after sticky or dusty leaves
- never flood the chamber SHT45
- replace porous gaskets when compressed or cracked

## Safety

- do not clamp toxic or irritant species bare-handed
- keep LiPo away from irrigation water
- avoid measuring leaves hotter than 55 °C until gasket material is validated
