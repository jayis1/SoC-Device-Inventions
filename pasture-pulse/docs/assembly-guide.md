# Pasture Pulse Assembly Guide

## 1. Overview

This guide describes a practical first prototype for the Pasture Pulse electric-fence monitor.

## 2. PCB Partitioning

Use three physical zones on the PCB:

1. **Radio + MCU zone** near antenna keep-out
2. **Low-noise analog zone** around ADS131M02 and OPA2192
3. **High-impedance pickup / surge zone** near the capacitive plate connector and HV divider string

Add milled isolation slots between the pickup divider and the logic domain.

## 3. Recommended Stack-Up

- 4-layer FR-4
- L1: signals and placement
- L2: solid ground plane
- L3: split power plus guarded analog traces
- L4: signals / low-speed routing

## 4. Assembly Order

1. Populate BQ25570 power harvester section.
2. Bring up 3V3 buck output with bench supply before connecting the battery.
3. Populate STM32WL55JC, decouplers, and SWD header.
4. Populate I2C devices: TMP117, SHT45, FRAM, OLED connector.
5. Populate ADS131M02 and OPA2192 pulse-current path.
6. Populate comparator and wake path.
7. Populate pickup divider string and surge clamps last.
8. Mount the capacitive pickup plate and dielectric window.

## 5. First Power-Up Checks

- Verify `3V3_SYS = 3.30 V ±3%`
- Confirm no excessive current draw in sleep mode
- Check I2C scan response from TMP117, SHT45, MB85RC256V, and OLED
- Validate FRAM read/write persistence
- Confirm LoRa radio can enter standby and TX test mode

## 6. Bench Pulse Injection

Do **not** begin with a live fence.

Use an isolated function generator to inject a low-voltage pulse equivalent into the conditioned ADC input:

- amplitude: 0.5-1.5 Vpp
- rise: 5-30 µs
- decay: 50-250 µs
- repetition: 1 Hz

Verify:

- comparator interrupt triggers reliably
- ADS131M02 captures both channels
- computed rise/decay metrics track generator settings

## 7. Field Calibration

1. Install Pasture Pulse on a known-good fence segment.
2. Place a commercial fence tester nearby.
3. Record 100 pulses across dry conditions.
4. Fit a linear or 2-point calibration between pickup ADC peak and tester kV.
5. Repeat during wet vegetation conditions to refine the leakage thresholds.

## 8. Enclosure Guidance

- UV-stable polycarbonate body
- silicone gasketed lid
- external strap ears for T-post or timber-post mounting
- ePTFE vent over humidity sensor
- shield wall between pickup plate and digital section
- clear labeling: **NON-CONTACT PICKUP ONLY**

## 9. Safety Notes

- never make a direct low-impedance electrical connection to the live fence terminal for this design intent
- maintain creepage and insulation around the pickup network
- remove service USB before storm operation
- comply with regional electric-fence energy and grounding requirements
