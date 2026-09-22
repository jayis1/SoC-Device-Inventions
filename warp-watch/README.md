# Warp Watch — Loom Warp-Tension and Take-up Sentinel

Warp Watch is a compact reference design for a handweaver, small textile studio, or conservation lab that needs a visible record of warp tension and take-up motion without instrumenting an entire loom. A flexure-mounted load cell measures tension while a magnetic encoder observes take-up rotation; the RP2040 presents a local OLED state and USB-serial log.

## Design status and safety

| Evidence | Status |
|---|---|
| Architecture, pin map, BOM, and KiCad reference sheet | Complete reference design |
| Firmware policy core | Host-buildable and unit-tested |
| RP2040/Pico SDK integration | Not built in this environment |
| KiCad ERC / PCB layout | Not run / PCB not supplied |
| Mechanical calibration and loom trials | Not performed |

This is not a safety device, tension limiter, automatic brake, or production-quality-control certification system. Never use it as the sole basis for worker safety, machinery protection, conservation treatment, or commercial acceptance.

## Problem, user, and novelty

A weaver usually discovers a drifting warp after uneven picks, broken ends, or visible fabric distortion. Warp Watch is for a maker or small studio who wants a quantitative cue that tension is slack, excessive, or unchanged despite expected take-up movement. Its falsifiable design target is to report 0.05–10 N warp tension with <=0.1 N repeatability after a two-point calibration on the final mechanical fixture.

It is distinct from the repository’s heated textiles, magnetic material instruments, fiber-production, torque, vibration, and process analyzers: the domain is loom operation and the primary interaction is force-plus-rotation feedback for warp management. A generic hanging scale cannot associate tension with take-up motion, while industrial electronic looms generally require proprietary multi-end tension hardware. Warp Watch deliberately makes one transparent, calibration-aware sensing channel practical for a retrofit clamp.

Estimated prototype BOM: **US$28.20** excluding enclosure and assembly. Target controller PCB: 60 x 38 mm; the load-cell flexure and loom clamp must be designed for the specific loom.

## Architecture

```text
USB-C 5 V -> PTC + TVS -> MCP73831 -> protected 1S LiPo -> TPS62743 3V3
                                                         |
        load cell -> ADS1232 24-bit bridge ADC --- GPIO2/3
                                                         |
AS5600 magnet encoder (0x36) -- I2C0 GPIO0/1 --- RP2040 --- USB CDC log
SSD1306 OLED (0x3C) ----------- I2C0 GPIO0/1      |-- GPIO6 status LED
buttons GPIO10/11 -------------------------------  |-- GPIO7 piezo driver
battery divider ----------------------------------- GPIO26 ADC
SWD pads GPIO24/25/RUN; optional UART GPIO14/15
```

The sensor system has no radio and no cloud dependency. Force samples are converted only after a local two-point calibration. The AS5600 observes a diametric magnet on the take-up shaft or an isolated idler; its count is used for motion detection, not as an absolute length measurement.

## Pin and bus contract

| RP2040 pin | Net | Connection | Notes |
|---:|---|---|---|
| GPIO0 | I2C0_SDA | AS5600, SSD1306 | 4.7 kOhm pull-up to 3V3 |
| GPIO1 | I2C0_SCL | AS5600, SSD1306 | 4.7 kOhm pull-up to 3V3 |
| GPIO2 | ADS_DOUT | ADS1232 DOUT | 3.3 V digital input |
| GPIO3 | ADS_SCLK | ADS1232 SCLK | keep away from analog input |
| GPIO6 | STATUS_LED | LED1 via 1 kOhm | active high |
| GPIO7 | BUZZER_PWM | Q1 base resistor | Q1 low-side switch for BZ1 |
| GPIO10 | BTN_MODE_N | switch to GND | internal pull-up, active low |
| GPIO11 | BTN_MARK_N | switch to GND | internal pull-up, active low |
| GPIO14/15 | UART_TX/RX | 3.3 V header | optional diagnostics only |
| SWDIO/SWCLK/RUN | SWDIO/SWCLK/RUN | test pads | dedicated debug/recovery pins; not GPIO24/25 |
| GPIO26 | BAT_SENSE | divided VBAT | ADC; scale and clamp in firmware |

I2C is 400 kHz, 3.3 V only. AS5600 is `0x36`; SSD1306 is `0x3C`. ADS1232 uses a dedicated two-wire synchronous interface, not I2C. Do not connect 5 V USB/UART/I2C accessories directly to the RP2040 GPIO.

## Power architecture and budget

```text
USB-C VBUS -> F1 0.5 A PTC -> D2 TVS -> U6 MCP73831 -> B1 protected LiPo
B1 -> U7 TPS62743 3.3 V -> RP2040, ADS1232, AS5600, OLED logic
B1 -> Q1/BZ1 return at battery star point
```

| Mode | Estimated current | Notes |
|---|---:|---|
| Shipping/off | < 5 uA | protection leakage only; validate on hardware |
| Display off, sample 10 Hz | 7 mA | RP2040 clocked conservatively; estimated |
| OLED active, USB detached | 20 mA | depends on display brightness |
| USB CDC active | 28 mA | host-powered enumeration and logging |
| Buzzer pulse | 45 mA peak | short, local cue |

A 500 mAh protected LiPo is estimated at 15–40 hours depending on screen and buzzer duty cycle; this is not a measured runtime. Place 100 nF at each IC, 1 uF at RP2040 VREG pins, and 10 uF bulk capacitors near 3V3 and battery rails. USB-C requires two 5.1 kOhm Rd resistors, ESD protection in the physical layout, and 90-ohm differential D+/D- routing. Keep the ADS1232/load-cell analog region separated from USB, buzzer, and switching loops.

## Schematic, BOM, and mechanics

- [KiCad reference schematic](schematic/warp-watch.kicad_sch)
- [KiCad project settings](schematic/warp-watch.kicad_pro)
- [Normalized BOM](hardware/BOM.csv)
- [Assembly and bring-up guide](docs/ASSEMBLY.md)
- [USB protocol](docs/API.md)
- [Validation record](docs/VALIDATION.md)

The KiCad 7 source is a human-readable reference sheet, not an ERC-cleared fabrication release. Resolve library symbols, verify footprints and USB layout, design the load path, then run ERC/DRC and a mechanical review before ordering.

The clamp must load the specified one-kilogram bar load cell in its intended axis without rubbing or side-loading it. Use a guarded, rounded yarn guide and ensure a broken yarn cannot be trapped against a sharp edge. Calibration is mechanical: zero with the path unloaded, apply at least two traceable force points in the final routing geometry, and recheck after temperature changes or moving the clamp.

## Firmware

`firmware/` contains a strict C11 policy core plus host tests; it has no hidden credentials or wireless configuration.

```text
firmware/
├── include/warp_watch.h       state and pin-independent policy contract
├── src/warp_watch.c           slack/over-tension/stall/fault decisions
├── src/main.c                 host demo adapter
├── tests/test_warp_watch.c    policy assertions
├── CMakeLists.txt             host C11 build
└── pico-sdk-notes.txt         RP2040 target adapter contract
```

Build and test the portable core:

```text
cmake -S firmware -B firmware/build
cmake --build firmware/build
ctest --test-dir firmware/build --output-on-failure
./firmware/build/warp_watch_demo
python3 -m py_compile scripts/check_calibration.py
```

The RP2040 target adapter must sample ADS1232 with bounded waits, reject impossible ADC readings, debounce buttons, detect AS5600 motion with wrap-aware deltas, save calibration atomically, mute buzzer output during boot/recovery, and enable a watchdog only after peripherals are initialized. If force or motion data are stale, it must report `SENSOR_FAULT` and avoid representing a measurement as valid.

## Behavior and configuration

Default example limits are 150 cN minimum, 800 cN maximum, and 30 seconds without observed take-up motion. These are illustrative only; actual limits depend on yarn, loom geometry, shed, and weaving method.

- `STABLE`: in-range tension and recent motion.
- `SLACK`: below the configured minimum; amber indicator and short cue.
- `OVER_TENSION`: above configured maximum; red indicator and longer cue.
- `TAKEUP_STALLED`: in-range force but no detected take-up change for the timeout; both indicators and cue.
- `SENSOR_FAULT`: invalid/stale force or motion data; both indicators, output inhibited until recovery.

See [API.md](docs/API.md) for the versioned USB framing and [check_calibration.py](scripts/check_calibration.py) for two-point calibration coefficient calculation.

## Limits, privacy, and credit

Load-cell creep, yarn friction, angle changes, temperature, vibration, magnet alignment, display noise, and clamp mechanics can dominate accuracy. The device cannot identify every broken end or fabric defect. It is not certified for machinery safety, worker protection, battery safety, EMC, or regulatory compliance.

USB data remains local to the connected host; no radio, account, or cloud service is included. Use a protected LiPo and observe the battery guidance in the assembly guide.

MIT. Designed and documented by [jayis1](https://github.com/jayis1).
