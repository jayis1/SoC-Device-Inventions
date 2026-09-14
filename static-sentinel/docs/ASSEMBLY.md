# Static Sentinel assembly and bring-up

## Prerequisites

- KiCad 7+ for completing symbol/footprint linkage and ERC.
- A current-limited 5 V USB supply set to 0.50 A maximum.
- Two isolated resistance standards (for example 1 Mohm, 1%, rated for the low test voltage).
- A calibrated DMM. Do not use an energized ESD mat as the initial test article.

## Assembly order

1. Assemble F1, D1, U4, C6, and C7. With U1 absent, apply 5 V and verify 3.3 V at the U1 supply pad; stop if current exceeds 20 mA.
2. Fit U2, U3, R5, R6, and local 100 nF capacitors. Confirm SDA/SCL idle high at 3.3 V.
3. Fit R1–R4 and the MAT/STRAP terminals. Inspect for flux residue and solder bridges. Keep the high-impedance sense traces short, guarded, and away from USB/antenna copper.
4. Fit U1, button, LED, and buzzer driver. Program with the module vendor’s documented ESP32-C6 UART/USB procedure; preserve the GPIO9 boot strap requirements.
5. Before attaching any bench accessory, connect 1 Mohm standards to the two inputs. Run the host-equivalent calibration calculation and record offsets.

## Bring-up acceptance checks

| Check | Expected result | Failure action |
|---|---|---|
| USB input at 5 V | 3V3 rail 3.20–3.40 V | remove power; inspect F1/D1/U4 |
| I2C scan | 0x44 and 0x48 present | inspect pull-ups and address straps |
| GPIO2 reset state | low / high impedance | do not attach terminals until corrected |
| 1 Mohm standard | configured `OK` band | calibrate divider and inspect R1/R2 |
| open terminal | invalid/out-of-policy | inspect connector and ADC source impedance |

## Enclosure

Use a nonconductive enclosure with strain relief. Put explicit labels on J3/J4: `SELV 3.3 V SENSE ONLY — DO NOT CONNECT TO MAINS`. Provide an access opening for the programming interface without exposing the measurement terminals.

## Production notes

This is not a fabrication-ready PCB package. Complete ERC, creepage/clearance review, RF/ESD layout, fixture testing, and calibration before any production use.
