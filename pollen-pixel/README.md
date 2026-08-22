# Pollen Pixel — Pocket Airborne Pollen & Spore Imager

> A handheld allergy and crop-health instrument that pulls air across an adhesive impaction tape, images the captured particles under white + 405 nm illumination, and estimates pollen/spore concentration in grains/m³ with on-device classification.

## What It Is

**Pollen Pixel** is a field-portable aerobiology instrument for people who care about **what is in the air right now**, not just what a distant citywide station reported six hours ago. It combines a controlled airflow inlet, a moving adhesive tape, a close-focus camera, and simple morphology/fluorescence classification running on an **ESP32-S3-WROOM-1**.

The device is aimed at:

- **Allergy sufferers** who want local, immediate pollen risk
- **Farmers and greenhouse operators** tracking fungal spore pressure before visible disease outbreaks
- **Researchers and schools** teaching airborne particle sampling and image analysis
- **Makers and citizen scientists** building neighborhood pollen maps
- **Indoor air investigators** comparing HVAC filtration or room-to-room transfer

Unlike a generic particle counter, Pollen Pixel attempts to distinguish broad biological classes:

- **Tree pollen**
- **Grass pollen**
- **Weed/ragweed-like pollen**
- **Fungal spores**
- **Dust / non-biological debris**

It does this by combining:

1. **Impaction sampling** through a slit onto tacky tape
2. **Brightfield imaging** for shape/size
3. **405 nm fluorescence imaging** for sporopollenin / biological contrast
4. **Heuristic classification** using area, circularity, elongation, edge density, and UV/visible ratio

## Why This Device Matters

Public pollen services are sparse and regional. A user can be in a backyard, orchard, field edge, classroom, barn, or bedroom where exposure differs dramatically from the nearest station. Pollen Pixel gives a buildable path to local monitoring with components that are realistically available.

## Core Architecture

### Main SoC: ESP32-S3-WROOM-1-N8R8

The ESP32-S3 was selected because it brings together the exact features this design needs:

- Parallel camera interface support
- Enough RAM/PSRAM for frame buffers
- Wi-Fi + BLE for dashboard and phone sync
- USB serial for configuration and data export
- Sufficient CPU for real-time connected-component analysis at modest image sizes

## Functional Overview

1. A **5 V blower** pulls air through a **0.8 mm × 8 mm slit**.
2. Particles impact onto a **replaceable adhesive tape cassette** mounted on a stepper-driven drum.
3. At scheduled intervals, the drum advances one field of view.
4. The camera captures the current patch twice:
   - white LED brightfield image
   - 405 nm fluorescence image
5. The ESP32-S3 segments particles and extracts shape features.
6. The firmware classifies each particle into a coarse class.
7. Flow rate and imaged area convert counts into **grains/m³**.
8. Results are shown on the OLED, logged to microSD, and optionally streamed over BLE/Wi-Fi.

## Block Diagram

```text
           ┌────────────────────── Pollen Pixel ──────────────────────┐
           │                                                           │
Air inlet ─► slit nozzle ─► adhesive tape on drum ─► waste cassette    │
           │                 ▲                        │                 │
           │                 │                        │                 │
           │          bipolar stepper + DRV8834       │                 │
           │                                                           │
           │   white LEDs + 405 nm LEDs                               │
           │            │                                               │
           │            ▼                                               │
           │      HM0360 camera + M12 macro lens                        │
           │            │ DVP                                            │
           │            ▼                                                │
           │      ESP32-S3-WROOM-1                                      │
           │      - image segmentation                                  │
           │      - morphology classifier                               │
           │      - BLE/Wi-Fi export                                    │
           │      - SD logging                                          │
           │                                                            │
           │  SDP810 diff pressure ─┐                                   │
           │  BME280 ambient sensor ├─ I2C ─► ESP32-S3                  │
           │  PCF85063A RTC        ─┘                                   │
           │                                                            │
           │  SH1106 OLED ─ I2C                                          │
           │  microSD ─ SPI                                               │
           │  buttons / RGB LED / buzzer ─ GPIO                          │
           │                                                            │
           │  LiPo -> MCP73831 -> TPS63031 3V3                           │
           │                     -> MT3608 5V blower/LED rail            │
           │                     -> TPS7A2033 analog camera rail         │
           └────────────────────────────────────────────────────────────┘
```

## Full Schematic Design

### Major ICs and Subsystems

| Ref | Part | Function |
|-----|------|----------|
| U1 | ESP32-S3-WROOM-1-N8R8 | Main SoC, image processing, UI, comms |
| U2 | HM0360 | Monochrome camera sensor |
| U3 | DRV8834 | Microstepping driver for tape-advance stepper |
| U4 | SDP810-500PA | Differential pressure sensor across laminar restriction for airflow estimation |
| U5 | BME280 | Ambient T/RH/P compensation |
| U6 | PCF85063A | RTC for timestamping samples |
| U7 | SH1106 OLED module | Local UI |
| U8 | microSD socket | Data logging |
| U9 | MCP73831 | LiPo charger |
| U10 | TPS63031 | 3.3 V buck-boost main rail |
| U11 | MT3608 | 5 V boost for blower + illumination |
| U12 | TPS7A2033 | Low-noise analog/camera rail |
| Q1-Q4 | AO3400A | LED and blower switching |
| D1-D2 | 405 nm LEDs | Fluorescence excitation |
| D3-D4 | White LEDs | Brightfield imaging |
| M1 | 24BYJ bipolar stepper | Tape advance |
| B1 | 2000 mAh LiPo | Portable power |

### Signal and Power Nets

#### Camera bus

- **XCLK** from ESP32-S3 LEDC/timer output through 74LVC1T45 buffer
- **PCLK, VSYNC, HREF, D0..D7** from HM0360 to ESP32-S3 GPIO matrix
- Camera reset and standby controlled by GPIO
- Camera analog supply on low-noise 3.3 V rail

#### Flow measurement

- Air passes through a molded restriction downstream of the inlet slit
- SDP810 senses differential pressure across the restriction
- ESP32 converts ΔP to flow using calibration coefficients stored in NVS

#### Tape advance

- 4-wire bipolar can-stack stepper driven by DRV8834
- 1/8 microstepping for repeatable frame-to-frame motion
- Home optical flag position read by a GPIO interrupt

#### Illumination

- White LEDs switched by NMOS for brightfield capture
- 405 nm LEDs switched separately for fluorescence capture
- PWM current control allows auto-exposure and UV safety limiting

#### Storage and user I/O

- SH1106 OLED on I2C
- microSD on SPI
- three buttons: MENU, SAMPLE, ADVANCE
- RGB LED indicates idle / sampling / fault / UV active

## ESP32-S3 Pin Assignment

| ESP32-S3 Pin | Function |
|--------------|----------|
| GPIO4 | CAM_XCLK |
| GPIO5 | CAM_PCLK |
| GPIO6 | CAM_VSYNC |
| GPIO7 | CAM_HREF |
| GPIO8..GPIO15 | CAM_D0..CAM_D7 |
| GPIO16 | CAM_RESET |
| GPIO17 | CAM_STANDBY |
| GPIO18 | I2C_SDA |
| GPIO19 | I2C_SCL |
| GPIO20 | OLED_RESET |
| GPIO21 | microSD_CS |
| GPIO35 | SPI_MISO |
| GPIO36 | SPI_MOSI |
| GPIO37 | SPI_SCK |
| GPIO38 | STEPPER_STEP |
| GPIO39 | STEPPER_DIR |
| GPIO40 | STEPPER_EN |
| GPIO41 | HOME_SENSOR |
| GPIO42 | BLOWER_PWM |
| GPIO45 | WHITE_LED_EN |
| GPIO46 | UV_LED_EN |
| GPIO47 | BUTTON_MENU |
| GPIO48 | BUTTON_SAMPLE |
| GPIO1 | UART_TX debug |
| GPIO2 | UART_RX debug |

> Some ESP32-S3 modules reserve strapping pins depending on board design; verify final boot-mode compatibility during PCB layout.

## Particle Classification Strategy

For each segmented object the firmware computes:

- projected area in pixels
- equivalent diameter estimate
- bounding-box elongation
- circularity
- edge density / texture
- UV-to-brightfield intensity ratio

Heuristic rules then map particles to broad classes:

- **Tree pollen**: round, lower UV ratio
- **Grass pollen**: mid circularity, mid UV ratio
- **Weed pollen**: spikier texture, stronger UV ratio
- **Fungal spores**: elongated and highly fluorescent
- **Dust**: small, weak UV response

This is intentionally simple, transparent, and field-tunable.

## Concentration Calculation

The firmware estimates airborne concentration as:

```text
grains_per_m3 = (particle_count × tape_band_area / imaged_patch_area)
                / (sampled_air_volume × inlet_efficiency)
```

Where:

- sampled air volume = flow × time
- tape_band_area / patch_area extrapolates one image to the whole deposited strip width
- inlet efficiency is obtained from calibration against a reference sampler

## Power Architecture

```text
USB-C 5V ──► MCP73831 ──► 1-cell LiPo
                         ├─► TPS63031 3.3V main rail
                         │      ├─ ESP32-S3
                         │      ├─ OLED
                         │      ├─ RTC
                         │      └─ microSD
                         ├─► TPS7A2033 3.3V analog rail
                         │      ├─ HM0360
                         │      └─ SDP810
                         └─► MT3608 5V rail
                                ├─ blower
                                ├─ white LEDs
                                └─ 405 nm LEDs
```

### Estimated current draw

- idle UI only: ~85 mA
- blower sampling: ~210 mA
- blower + image capture + Wi-Fi burst: 280 to 360 mA
- expected battery life: 5 to 7 hours mixed field use

## Mechanical Design

- 3D-printed PETG clamshell with labyrinth optical baffles
- front inlet accepts stainless slit plate
- side-access cassette lets user replace adhesive tape cartridge without tools
- camera sits above the tape at fixed working distance with M12 focus lock ring
- blower exhausts through rear vents isolated from optics

## Firmware Layout

```text
firmware/
├── CMakeLists.txt
├── sdkconfig
├── include/
│   ├── device.h
│   ├── pipeline.h
│   ├── classifier.h
│   └── flow.h
└── src/
    ├── main.c
    ├── pipeline.c
    ├── classifier.c
    └── flow.c
```

The included code is a **host-buildable classifier core** that mirrors the embedded image-analysis logic and is useful for algorithm verification before binding it to ESP-IDF camera drivers.

## Build and Run the Firmware Simulator

```bash
cd firmware
cmake -S . -B build
cmake --build build
./build/pollen_pixel_fw_sim
```

Expected output: a demo pollen report with per-particle classification and a computed grains/m³ estimate.

## Logging Format

Example CSV schema:

```text
timestamp,flow_lpm,sample_seconds,particle_count,grains_m3,allergen_index,tree,grass,weed,spore,dust
2026-08-22T08:15:00Z,1.82,120,17,624.5,78.2,5,3,4,2,3
```

## Calibration Workflow

1. Set blower PWM to target 1.8 L/min using a reference flowmeter.
2. Store flow coefficients in NVS.
3. Focus on the supplied 10 µm dot slide.
4. Capture white and UV calibration frames.
5. Adjust LED duty until mean histogram is within target range.
6. Co-locate with a reference pollen trap for 3-7 days.
7. Fit the inlet-efficiency factor and per-class thresholds.

## Safety Notes

- 405 nm LEDs should be optically baffled; do not expose eyes at close range.
- Adhesive tapes may accumulate allergens; use gloves when swapping cassettes.
- UV LED duty is firmware-limited and should be hardware current-limited.
- Blower exhaust path should remain filtered to prevent contamination of optics.

## Example Use Cases

### Backyard allergy alert
A user leaves the instrument on a porch for 10-minute samples every hour. The device shows a morning rise in tree pollen followed by an afternoon increase in weed pollen, matching symptoms far better than citywide forecasts.

### Greenhouse disease prevention
A grower watches fungal spore counts near intake fans. A sudden jump after irrigation triggers a humidity-control adjustment before visible mildew damage spreads.

### Classroom aerobiology lab
Students compare indoor, outdoor, and HVAC-filtered air, then inspect how morphology and UV response differ between dust and biological particles.

## Repo Contents

- `README.md` — system overview and build instructions
- `schematic/` — KiCad project placeholders and net planning
- `firmware/` — C classifier/simulator core
- `hardware/BOM.csv` — buildable bill of materials
- `docs/assembly-guide.md` — assembly steps
- `docs/api-reference.md` — serial/BLE data interface
- `scripts/` — calibration and exposure-summary helpers

## Future Extensions

- tiny CNN classifier trained on local pollen species
- motorized z-stack focus sweep
- removable cyclone pre-separator for PM overload environments
- GPS-tagged community aerobiology maps
- hourly auto-advancing tape scheduler with solar dock
