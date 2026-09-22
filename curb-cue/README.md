# Curb Cue — Haptic Cane-Handle Obstacle Cue

Curb Cue is a reference design for a compact, clip-on mobility-cane handle module. It uses two short-range time-of-flight ranging sensors and directional haptic feedback to give a wearer a simple near-field cue for low obstacles and overhanging hazards while keeping their cane technique and hearing available.

## Design status

| Evidence | Status |
|---|---|
| Architecture, pin map, BOM, and KiCad reference sheet | Complete reference design |
| Firmware policy core | Host-built and unit-tested |
| Zephyr/nRF52840 target integration | Not built in this environment |
| KiCad ERC / PCB layout | Not run / PCB not supplied |
| Physical prototype and field validation | Not performed |

This is an unverified conceptual reference design, not a navigation aid, medical device, collision-avoidance system, or substitute for mobility training. Never rely on it to detect traffic, stairs, drop-offs, moving vehicles, people, or every obstacle.

## Problem, user, and difference

A cane user can detect ground-level obstacles through trained cane technique, but a bag, branch, sign edge, or protruding furniture can still occupy the torso/head zone before the cane contacts it. Curb Cue is intended for a cane user who wants an optional, private near-field warning layer without a phone, cloud account, camera, or continuous audio.

Why not buy a generic ultrasonic key finder or smart cane? Those products often provide one undirected beep, use a single broad sensor, or require a phone. Curb Cue deliberately separates a forward low sensor from an upward-angled sensor and maps the result to left/right haptics; it is a cue, not autonomous navigation.

It differs from the repository's wearable environment, acoustic, optical, agricultural, building, and instrument designs on at least two axes: its accessibility domain and its paired ToF-to-haptic near-obstacle interface. It uses a low-power BLE SoC only for optional configuration, not reporting or cloud telemetry.

Estimated prototype BOM: **US$30.40** excluding enclosure and assembly. Difficulty: **intermediate SMD assembly**. Target form factor: 58 x 34 mm PCB in a clip-on, splash-resistant handle enclosure.

## Architecture

```text
USB-C 5 V -> TVS + charger/protection -> LiPo cell -> 3V3 buck/LDO
                                                |
                                         nRF52840-QIAA
                       I2C GPIO26/27 ---+--- VL53L5CX low-zone ToF (0x2A after XSHUT sequence)
                                        +--- VL53L5CX high-zone ToF (0x29)
                     GPIO13/14 -> DRV2605L haptic drivers -> left / right LRA motors
                     GPIO11 <- mode button          GPIO12 -> status LED
                     SWDIO/SWDCLK -> programming pads; BLE -> optional configuration only
```

The sensors share I2C but start with their `XSHUT` pins low. Firmware enables the low-zone unit, changes it from default `0x29` to `0x2A`, then enables the high-zone unit at `0x29`. The high-zone sensor is mechanically aimed approximately 18 degrees above horizontal; validate the angle in the final enclosure.

## Pin assignment

| nRF52840 pin | Net | Function | Notes |
|---:|---|---|---|
| P0.26 | I2C_SDA | shared ToF / haptic I2C data | 4.7 kOhm pull-up to 3V3 |
| P0.27 | I2C_SCL | shared ToF / haptic I2C clock | 4.7 kOhm pull-up to 3V3 |
| P0.13 | HAPTIC_L_EN | DRV2605L left enable | active high |
| P0.14 | HAPTIC_R_EN | DRV2605L right enable | active high |
| P0.15 | TOF_LOW_XSHUT | low-zone sensor reset | high after address setup |
| P0.16 | TOF_HIGH_XSHUT | high-zone sensor reset | high last |
| P0.11 | MODE_BTN_N | mode button | internal pull-up, active low |
| P0.12 | STATUS_LED | tri-state/status LED drive | 1 kOhm series |
| P0.18 | SWDIO | debug/program | test pads only |
| P0.20 | SWDCLK | debug/program | test pads only |
| RESET | nRESET | reset | 10 kOhm pull-up, reset pad |

I2C addresses after startup: low-zone VL53L5CX `0x2A`; high-zone VL53L5CX `0x29`; DRV2605L left `0x5A`; DRV2605L right `0x5B` (ADDR strapped high). The bus operates at 400 kHz. Do not attach external 5 V I2C equipment.

## Power architecture and budget

```text
USB-C VBUS -> F1 (0.5 A PTC) -> D1 (5 V TVS) -> MCP73831 charger -> protected 1S LiPo
LiPo -> TPS62743 3.3 V buck -> nRF52840, sensors, haptic logic
LiPo -> DRV2605L motor supply -> individual LRA motors
```

| Mode | Estimated current | Notes |
|---|---:|---|
| System off / shipping | < 5 uA | battery protection leakage only; validate on hardware |
| Idle scan at 10 Hz | 18 mA | radios off, two ToF devices duty-cycled |
| Near-obstacle haptic burst | 95 mA peak | two LRAs briefly active |
| BLE configuration | 12 mA average | optional, time-limited |

A 500 mAh protected LiPo is estimated at 12–18 hours of intermittent scan use; that is an estimate, not a measured runtime. The charger is set to 100 mA. Each IC has a 100 nF local capacitor; 10 uF bulk capacitors are placed near the buck, radio, and motor supply. USB-C has two 5.1 kOhm Rd resistors. Add antenna keep-out, ESD protection at USB-C, and a physical power switch in the final PCB/enclosure.

## Schematic and BOM

- [KiCad reference schematic](schematic/curb-cue.kicad_sch)
- [KiCad project settings](schematic/curb-cue.kicad_pro)
- [Normalized BOM](hardware/BOM.csv)
- [Assembly and bring-up guide](docs/ASSEMBLY.md)
- [Protocol and configuration reference](docs/API.md)
- [Validation record](docs/VALIDATION.md)

The schematic is a KiCad 7 text reference sheet that documents the required nets, components, electrical constraints, and test points. It is not an ERC-cleared fabrication release; replace the reference-sheet symbols with library-resolved symbols, set footprints, create a PCB, and run ERC/DRC before ordering hardware.

## Firmware

`firmware/` contains portable C11 policy logic and tests. The target integration is planned for **nRF Connect SDK v2.6.0** with board `nrf52840dk/nrf52840`; it is deliberately separated from policy logic so threshold behavior can be tested on a host.

```text
firmware/
├── include/curb_cue.h
├── src/curb_cue.c
├── src/main.c
├── tests/test_curb_cue.c
├── CMakeLists.txt
└── zephyr-prj.conf
```

Host verification:

```text
cmake -S firmware -B firmware/build
cmake --build firmware/build
ctest --test-dir firmware/build --output-on-failure
./firmware/build/curb_cue_demo
```

The target adapter must initialize both XSHUT lines low, assign the low-zone address first, leave motors disabled until a valid range frame arrives, and feed a watchdog during sensor transactions. BLE configuration must be opt-in and never required for basic use.

## Operating behavior

- `CLEAR`: no active haptic cue.
- `CAUTION`: an object enters 1.5 m; a slow pulse indicates the side with the closer return.
- `NEAR`: an object enters 0.75 m; a faster pulse indicates the closer return.
- `BOTH_NEAR`: both zones are near; both motors pulse together.
- `SENSOR_FAULT`: stale or invalid measurements; three short pulses, then cues are inhibited until valid data resumes.

The default thresholds are starting points only. Configure them only after controlled, stationary testing with the final sensor angle and enclosure.

## Limitations and safety

- Keep ordinary cane technique, environmental awareness, and mobility training as the primary safety method.
- ToF ranging can fail on dark, transparent, reflective, narrow, angled, rainy, sunlit, or moving targets.
- Do not use while crossing streets, riding, driving, near railway platforms, near stairs/drop-offs, or as the sole means of obstacle detection.
- Use a protected, certified LiPo; inspect it for swelling/damage; charge only in a nonflammable location and do not leave charging unattended.
- This design makes no accessibility, battery-runtime, RF, IP rating, impact, regulatory, or safety certification claim.

## License and credit

MIT. Designed and documented by [jayis1](https://github.com/jayis1).