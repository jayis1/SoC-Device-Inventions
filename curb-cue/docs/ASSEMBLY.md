# Curb Cue assembly and bring-up

Designed and documented by jayis1.

## Required tools

Hot-air or reflow capability suitable for QFN/LGA packages, microscope, fine solder paste stencil, current-limited bench supply, SWD probe, USB-C power meter, and a nonmetallic ranging target. Do not assemble a swollen, damaged, unprotected, or unknown LiPo cell.

## Assembly order

1. Inspect the bare PCB against the KiCad reference sheet. Resolve all symbols and footprints; do not order from the reference sheet alone.
2. Place U7, U6, F1, D1, passives, and the USB-C connector. Verify no VBUS-to-GND short.
3. Add U1 and confirm 3V3 at the SoC with the bench supply limited to 80 mA. Do not attach a battery yet.
4. Add U2/U3, pull-ups, U4/U5, then motors. Keep both XSHUT nets low until firmware controls them.
5. Add the protected LiPo only after charger polarity and 4.2 V regulation are verified. Fit strain relief and a physical switch in the enclosure.

## First power-up

| Check | Expected result | Stop condition |
|---|---|---|
| USB input at J1 | 5 V nominal after F1 | heat, visible damage, or >80 mA before firmware |
| 3V3 rail | 3.3 V ±5% | outside range or regulator heat |
| SWD attach | target detected | repeated reset or unknown target |
| I2C scan sequence | only one ToF at a time before readdress | both at 0x29 simultaneously |
| Haptic test | brief, distinct left/right pulse | continuous drive, excessive heat, or loose motor |

## Mechanical and calibration notes

Orient the low-zone sensor forward and the high-zone sensor about 18 degrees upward. The ToF windows must not be covered by glossy, tinted, wet, or recessed enclosure material. Start with 1500 mm caution and 750 mm near thresholds, then test with stationary matte targets at different heights. Record the final mechanical angle and threshold choices on the enclosure revision label.

## Troubleshooting

- Both ToF sensors at `0x29`: hold both XSHUT low, release only the low sensor, readdress it to `0x2A`, then release the high sensor.
- Haptic motor always on: verify P0.13/P0.14 are low before DRV2605L initialization and inspect I2C addresses.
- Range flickers outdoors: lower confidence, increase filtering, and treat the cue as unavailable; do not compensate by relying on it more heavily.
- Charging is hot or unstable: disconnect USB and battery immediately; inspect polarity, charge-program resistor, and cell condition.
