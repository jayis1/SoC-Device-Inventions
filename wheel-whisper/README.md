# Wheel Whisper — Bicycle Spoke Tension and Trueing Assistant

Wheel Whisper is a clamp-on reference design for a home mechanic or small bicycle workshop that wants repeatable evidence while tensioning and truing a wire-spoked wheel. A piezo contact sensor captures the decaying note of a tapped spoke, while a Hall sensor reads a small magnet on the rim stand. The nRF52840 calculates fundamental frequency and spoke-to-spoke spread, then gives local OLED/haptic guidance and optionally streams a local BLE measurement record.

## Design status and safe use

| Evidence | Status |
|---|---|
| Architecture, pin map, BOM, and netlist reference | Complete reference design |
| Portable C measurement core | Host-buildable and tested on the design host |
| nRF52840 SDK/Zephyr integration | Not built in this environment |
| KiCad ERC/DRC, PCB layout, calibration, and wheel trials | Not performed |

This is an unbuilt reference design, not a calibrated tension meter, wheel-strength certification tool, or riding-safety system. Frequency is only a useful tension proxy after choosing the correct spoke length, material, gauge, lacing pattern, and manufacturer tension range. A wheel may be unsafe despite apparently uniform readings; inspect for damage and use manufacturer guidance or a qualified wheel builder.

## Problem, user, and novelty

Spoke tension is often checked by feel or by comparing a plucked note. A dedicated device beats a dev board because its spring clamp, contact piezo, tap-triggered capture, rim-index cue, and one-handed haptic feedback make a repeatable workshop interaction rather than a loose microphone experiment.

Wheel Whisper is distinct from this collection’s vibration sentinels, torque auditor, acoustic monitors, and machinery diagnostics: it estimates the resonant frequency of one tensioned wire spoke in a known wheel geometry, then makes *relative spoke uniformity* visible during manual trueing. It does not infer absolute tension without a wheel-specific calibration table.

Estimated prototype BOM: **US$31.36** excluding printed clamp, battery, assembly, and test fixture. Target PCB: 52 x 34 mm; recommended enclosure is a PETG or nylon clamp with a compliant TPU piezo tip.

## Architecture

```text
USB-C 5 V -> PTC + TVS -> MCP73831 -> protected LiPo -> TPS62743 3V3
                                                          |
Piezo contact tip -> OPA320 charge amplifier -> nRF52840 SAADC (AIN0)
                                                          |
Rim magnet -> DRV5032 Hall switch ------------------- GPIO P0.11
OLED SSD1306 (0x3C) ---- I2C P0.26/P0.27 ----------- nRF52840 -> BLE
Buttons P0.06/P0.07, vibration motor P0.20 -> MOSFET, LED P0.13
SWD pads P0.18/P0.19; USB is charging only in this reference design
```

The clamp’s silicone-coupled piezo disc senses local spoke vibration. Firmware waits for a tap threshold, samples a 120 ms window at 8 ksample/s, subtracts DC, and searches the 80–1200 Hz band using a Goertzel sweep. The Hall input creates a rim-reference event for the optional 32-position measurement route; it does not measure wheel speed or control braking.

## Pin and bus contract

| nRF52840 pin | Net | Connection | Notes |
|---:|---|---|---|
| P0.02 / AIN0 | PIEZO_ADC | OPA320 output | 0–3.0 V biased analog signal; 1 kOhm series resistor |
| P0.26 | I2C_SDA | SSD1306 SDA | 4.7 kOhm pull-up to 3V3 |
| P0.27 | I2C_SCL | SSD1306 SCL | 4.7 kOhm pull-up to 3V3 |
| P0.11 | RIM_INDEX | DRV5032 OUT | input pull-up; active low magnet event |
| P0.06 | BTN_NEXT_N | pushbutton to GND | internal pull-up |
| P0.07 | BTN_SAVE_N | pushbutton to GND | internal pull-up |
| P0.13 | STATUS_LED | LED via 1 kOhm | active high |
| P0.20 | HAPTIC_EN | Q1 gate | logic-level NMOS low-side motor switch |
| P0.18 / P0.19 | SWDIO / SWCLK | programming pads | dedicated debug pads |
| P0.31 / AIN7 | BAT_SENSE | divided VBAT | divider must keep input below VDD |

I2C is 400 kHz at 3.3 V; the SSD1306 address is `0x3C`. The DRV5032 is a digital Hall switch, not an I2C device. Keep the piezo/amplifier traces short, shielded from the motor return, and physically isolated from USB-C and radio antenna keep-out.

## Measurement method and limits

For a sampled signal `x[n]`, the portable core calculates a mean-removed energy at each candidate frequency using Goertzel recurrence. It chooses the strongest candidate only if energy and prominence clear configured thresholds. The displayed metric is `frequency_hz`; a wheel profile may optionally map frequency to estimated kilogram-force using an empirically measured lookup curve.

1. Enter spoke material, free length, nominal tension, and target spread from the wheel maker or a validated tension gauge comparison.
2. Position the piezo tip near the middle third of a single spoke, tap it gently, and accept only a stable capture.
3. Record all spokes in the same side/group. Compare median, range, and percent spread—not a universal frequency number.
4. Make small manual adjustments, recheck lateral/radial true, dish, and stress-relieve per standard wheel-building practice.

Crossed spokes, bladed spokes, butted profiles, nipples, tire pressure, rim damping, nearby fingers, contact force, and ambient vibration affect the reading. The design rejects saturated, weak, and broad-peak captures, but rejection is not proof of a correct measurement.

## Power architecture and budget

```text
USB-C VBUS -> F1 0.5 A PTC -> D1 USB TVS -> U4 MCP73831 -> B1 protected LiPo
B1 -> U5 TPS62743 3V3 -> nRF52840, OLED, DRV5032, OPA320
B1 -> Q1 -> M1 vibration motor (separate pulsed return to battery star)
```

| Mode | Estimated current | Notes |
|---|---:|---|
| Shipping/off | < 5 uA | protection leakage; not measured |
| System-on idle, display dim | 1.8 mA | BLE advertising disabled |
| Capture + OLED | 9 mA | 120 ms windows at user cadence |
| BLE connection | 4–12 mA average | connection interval dependent |
| Haptic pulse | 70 mA peak | 100 ms maximum recommended |

A 300 mAh protected LiPo is estimated for 15–35 hours of intermittent workshop use; verify on hardware. Fit 100 nF at each IC, 1 uF at the nRF52840 regulator pins per reference design, and 10 uF bulk capacitance near 3V3. The USB-C connector needs 5.1 kOhm Rd resistors, ESD protection, and an antenna keep-out defined by the selected module datasheet.

## Schematic, BOM, and mechanics

- [Conceptual netlist and placement notes](schematic/wheel-whisper-reference.md)
- [Suggested BOM](hardware/BOM.csv)
- [Assembly and calibration guide](docs/ASSEMBLY.md)
- [BLE and measurement API](docs/API.md)
- [Calibration worksheet generator](scripts/profile_check.py)

The schematic folder intentionally contains a text reference rather than an unverified KiCad-native file. Transfer it into the exact library/version used for fabrication, validate symbol pin mapping and footprints, then run ERC/DRC and review antenna, analog, battery, and mechanical clearances before ordering.

## Firmware

```text
firmware/
├── wheel_whisper.c        portable capture-quality and Goertzel analysis core
├── wheel_whisper.h        pin-independent public contract
├── main.c                 host demonstration adapter
├── test_wheel_whisper.c   portable core tests
├── CMakeLists.txt         C11 host build and CTest target
└── sdkconfig.defaults     target-integration notes
```

Run the portable checks:

```text
cmake -S firmware -B firmware/build
cmake --build firmware/build
ctest --test-dir firmware/build --output-on-failure
python3 -m py_compile scripts/profile_check.py
```

The target adapter must configure SAADC with bounded DMA buffers, bias/scale the analog front end safely, use a watchdog after initialization, debounce controls, use the SoftDevice/Zephyr BLE stack approved for the selected module, and never label an invalid capture as a tension result.

## BLE/API summary

The optional BLE GATT service has 128-bit base UUID `b91a0000-7d3c-4f20-8a1c-4f8d4b0a6001`.

| Characteristic | Suffix | Properties | Payload |
|---|---|---|---|
| Measurement | `...6002` | notify/read | sequence, rim index, frequency centi-Hz, quality, flags |
| Profile | `...6003` | read/write | profile id, target frequency, tolerance permille |
| Command | `...6004` | write | start capture, clear route, sleep |

No cloud account is part of this design. Pair only with a trusted local host; BLE telemetry can reveal workshop activity but not wheel ownership data by itself.

## Directory structure

```text
wheel-whisper/
├── README.md
├── schematic/wheel-whisper-reference.md
├── firmware/                 portable C core, test, CMake configuration
├── hardware/BOM.csv
├── docs/ASSEMBLY.md
├── docs/API.md
└── scripts/profile_check.py
```

## Credit

MIT. Designed and documented by [jayis1](https://github.com/jayis1).