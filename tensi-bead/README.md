# Tensi Bead — Pocket Pendant-Drop Tensiometer and Contact-Angle Analyzer

> A handheld liquid-characterization instrument that dispenses a repeatable droplet, images its silhouette with a calibrated grayscale camera, and computes **surface tension** and **contact angle** on-device for field formulation work in coatings, detergents, agriculture, water treatment, food, and education.

## What It Is

**Tensi Bead** is a practical lab-in-your-hand SoC device built around the **ESP32-S3-WROOM-1-N8**. It combines a camera, controlled backlight, syringe micro-pump, load-cell feedback, environmental sensing, OLED UI, microSD logging, and BLE/Wi-Fi connectivity to perform two common wetting measurements without a bench instrument:

1. **Pendant-drop mode** for liquid **surface tension**.
2. **Sessile-drop mode** for solid-surface **contact angle** and wetting behavior.

That lets a technician or researcher answer questions such as:

- Is a detergent bath still active enough to wet parts properly?
- Did a coating or plasma treatment really raise the surface energy of a polymer?
- Is rinse water contaminated with surfactant carryover?
- Are ink or agrochemical formulations drifting between batches?

## Why This Device Matters

Surface tension and contact angle are often measured with benchtop instruments that are expensive, fragile, and awkward outside a lab. In many real workflows, the operator needs a quick field answer rather than a glass-enclosed metrology station.

Tensi Bead brings the essentials into a pocket tool:

- repeatable droplet generation via motorized syringe stage
- silhouette imaging with a controlled optical path
- on-device geometry extraction and classification
- battery operation for line-side or field work
- open firmware and open hardware rather than opaque vendor software

## Main SoC Choice

### ESP32-S3-WROOM-1-N8

The ESP32-S3 is a good fit because it provides:

- enough RAM and CPU for image pre-processing and shape fitting
- native Wi-Fi for instant CSV and dashboard export
- BLE for phone commissioning and quick capture control
- good camera-interface support for low-resolution monochrome sensors
- low cost and mature open-source tooling

## Functional Overview

1. A **NEMA8 syringe stage** dispenses a controlled droplet through a blunt needle.
2. A **diffuse LED backlight** creates a clean silhouette.
3. An **HM0360 grayscale camera** images the droplet.
4. The **ESP32-S3** thresholds the silhouette, finds the contour, and extracts neck, equator, height, base width, and contact-angle geometry.
5. In pendant mode, the firmware estimates **surface tension** from drop shape and density assumptions or stored presets.
6. In sessile mode, the firmware estimates **left/right contact angle**, spread index, and inferred wetting class.
7. The result is shown locally on OLED, logged to microSD, and streamed over BLE/Wi-Fi.

## Block Diagram

```text
               ┌──────────────────── Tensi Bead ────────────────────┐
Liquid syringe │                                                      │
 + needle ────►│ NEMA8 + DRV8834 syringe stage ─┐                    │
               │                                │                    │
Test droplet ─►│ Optical chamber / platen       │                    │
               │                                ▼                    │
White LED ────►│ Diffuse backlight      HM0360 camera ─────────┐     │
               │                                                │     │
Coupon / glass►│ Sessile platen + TMP117 + load cell/HX711 ────┤     │
Ambient air ──►│ SHT45 temp/RH ────────────────────────────────┤     │
RTC / OLED ───►│ DS3231 + SSD1306 ─────────────────────────────┤     │
microSD ──────►│ SPI storage ───────────────────────────────────┤     │
               │                                                ▼     │
               │              ESP32-S3-WROOM-1-N8                    │
               │      - contour extraction                           │
               │      - shape metrics                                │
               │      - surface tension / contact angle solve        │
               │      - UI, logging, BLE, Wi-Fi                      │
               │                                                      │
USB-C / 18650 ─► power mux / charger / 3V3 buck / LED current driver │
               └──────────────────────────────────────────────────────┘
```

## Full Schematic Design

### Major ICs and Subsystems

| Ref | Part | Function |
|-----|------|----------|
| U1 | ESP32-S3-WROOM-1-N8 | Main SoC, image processing, UI, telemetry |
| U2 | HM0360 | Monochrome camera for silhouette capture |
| U3 | SSD1306 128×64 OLED | Local UI and result display |
| U4 | SHT45 | Ambient temperature and humidity compensation |
| U5 | TMP117 | Sample/platen temperature sensing |
| U6 | DS3231SN | RTC for time-stamped captures |
| U7 | CP2102N | USB-UART bridge for console and provisioning |
| U8 | DRV8834 | Stepper driver for syringe plunger stage |
| U9 | HX711 | 24-bit load-cell ADC for tare / mass checks |
| U10 | 5 kg bar load cell | Fixture tare and coupon mass feedback |
| U11 | TPS62162 | 3.3 V buck regulator |
| U12 | MCP73831 | 18650 Li-ion charger |
| U13 | TPS2113A | Automatic USB/battery power mux |
| U14 | LED backlight array | Diffuse silhouette illumination |
| U15 | AL8860 | Constant-current LED driver with PWM dimming |
| J1 | microSD socket | CSV + raw frame logging |
| J2 | USB-C receptacle | Power and serial debug |
| M1 | NEMA8 stepper | Syringe actuation |

### Camera and Optics

The **HM0360** is positioned normal to the sample window and views the droplet against a diffuse backlight.

- grayscale imaging simplifies thresholding
- fixed-focus lens avoids autofocus drift
- capture window is calibrated with a 5 mm reference pin
- black optical chamber reduces reflections and edge ghosts

The ESP32 processes the frame in this order:

1. dark-frame subtraction
2. adaptive threshold
3. largest-blob contour isolation
4. sub-pixel edge interpolation
5. geometry extraction from contour

### Syringe and Sample Handling

A disposable syringe with blunt needle is mounted vertically above the window.

- **DRV8834** drives the NEMA8 stage in microsteps
- limit switch or current-stall detect defines home
- firmware supports drop sizes from roughly 1-20 µL
- flush and retract routines reduce stringing between captures

### Sessile Platen and Load Feedback

For contact-angle work, droplets are dispensed onto a removable coupon platen.

- **TMP117** measures platen temperature for temperature-sensitive liquids
- **HX711 + bar load cell** gives tare and optional droplet mass cross-check
- platen insert can be swapped between glass, PTFE, stainless, or custom coupons

### Communications and Storage

- **I2C**: SHT45, TMP117, DS3231, OLED
- **SPI**: microSD
- **DVP camera bus / SCCB**: HM0360
- **UART0**: CP2102N debug / manufacturing test
- **Wi-Fi**: HTTP JSON API, CSV export, OTA updates
- **BLE**: phone commissioning and capture trigger

## ESP32-S3 Pin Assignment

| ESP32-S3 Pin | Function |
|--------------|----------|
| GPIO1 | BUTTON_MODE |
| GPIO2 | BUTTON_SELECT |
| GPIO3 | BUTTON_HOME |
| GPIO4 | STEPPER_STEP |
| GPIO5 | STEPPER_DIR |
| GPIO6 | STEPPER_nEN |
| GPIO7 | STEPPER_nFAULT |
| GPIO8 | I2C_SDA |
| GPIO9 | I2C_SCL |
| GPIO10..GPIO17 | CAMERA_D0..D7 |
| GPIO18 | CAMERA_PCLK |
| GPIO19 | CAMERA_VSYNC |
| GPIO20 | CAMERA_HREF |
| GPIO21 | CAMERA_XCLK |
| GPIO34 | SD_CS |
| GPIO35 | SPI_MOSI |
| GPIO36 | SPI_SCK |
| GPIO37 | SPI_MISO |
| GPIO38 | HX711_DOUT |
| GPIO39 | HX711_SCK |
| GPIO40 | BUZZER_EN |
| GPIO41 | RGB_LED_R |
| GPIO42 | RGB_LED_G |
| GPIO48 | RGB_LED_B |
| GPIO43 | UART0_TX |
| GPIO44 | UART0_RX |
|
> Final routing still needs the usual ESP32-S3 strap-pin check before fabrication.

## Power Architecture

### Inputs

- **USB-C 5 V** for bench use and charging
- **Single 18650** for portable operation

### Rails

- **5 V LED rail** for backlight current driver
- **3.3 V logic rail** from TPS62162 for SoC, camera, OLED, sensors, RTC, microSD
- **VMOTOR rail** from boosted battery or external 12 V section for the stepper stage

### Protection

- reverse-polarity and switchover handled by **TPS2113A**
- battery charge management by **MCP73831**
- brownout detection handled in firmware with safe motor stop

## Measurement Modes

### 1. Pendant-Drop Surface Tension

The firmware measures:

- neck width
- equator width
- overall drop height
- ambient temperature
- stored density preset or user-entered density

From those it computes:

- surface tension in **mN/m**
- Bond number
- quick class such as water-like / medium / surfactant-rich

Typical use cases:

- rinse-water contamination checks
- detergent concentration drift
- ink / coating wetting adjustment
- agricultural spray additive verification

### 2. Sessile-Drop Contact Angle

The firmware measures:

- left contact angle
- right contact angle
- average contact angle
- base width and height
- spread index

Typical use cases:

- plasma / corona treatment verification
- comparing coated vs uncoated surfaces
- monitoring hydrophobic coating wear
- teaching wetting and adhesion fundamentals

## Signal-Processing and Math Flow

```text
Frame capture
  -> dark correction
  -> threshold + contour extraction
  -> contour metrics (neck, equator, height, base)
  -> calibration scaling (px -> mm)
  -> pendant or sessile solver
  -> QC checks (focus, edge contrast, symmetry, lighting)
  -> classify + display + log + stream
```

The included firmware simulator uses a simplified geometry-based approximation so the repo remains self-contained. A production build can swap in a fuller Young-Laplace fit and spline-based tangent solve while keeping the same interfaces.

## Firmware Layout

```text
firmware/
├── CMakeLists.txt
├── sdkconfig
├── include/
│   ├── analysis.h
│   ├── sensors.h
│   ├── tensi_bead.h
│   └── ui.h
└── src/
    ├── analysis.c
    ├── main.c
    ├── sensors.c
    └── ui.c
```

### Firmware Responsibilities

- sensor and demo-frame acquisition
- geometry extraction abstraction
- pendant vs sessile analysis
- QC flagging
- local report generation

## Scripts

- `scripts/generate_demo_capture.py` creates a small example CSV capture set
- `scripts/analyze_capture.py` summarizes a CSV run for quick sanity checks

## Build and Run

### Firmware simulation

```bash
cd firmware
cmake -S . -B build
cmake --build build
./build/tensi_bead_fw_sim
```

### Python helpers

```bash
python3 scripts/generate_demo_capture.py
python3 scripts/analyze_capture.py scripts/demo_capture.csv
```

## Expected Performance Targets

- surface tension repeatability target: **±1.5 to 2.5 mN/m** after calibration
- contact-angle repeatability target: **±2 to 4°** on clean reference coupons
- capture-to-result time: **< 2 s** for single-shot measurements
- battery runtime: **6-8 hours** of intermittent field use

## Build Notes

- keep the optical chamber matte black internally
- avoid LED hot spots by using an opal diffuser and 6-10 mm LED offset
- use disposable needles/tubing if surfactants or food fluids will be cross-tested
- recalibrate pixel scale after any camera refocus or lens replacement

## Applications

- paint, ink, and adhesive formulation tuning
- detergent and CIP bath verification
- agricultural spray additive setup
- plastics surface-treatment validation
- education in wetting, capillarity, and fluid interfaces
- small lab, field, and line-side quality control

## Repository Contents

```text
tensi-bead/
├── README.md
├── schematic/
│   ├── tensi-bead.kicad_pcb
│   ├── tensi-bead.kicad_pro
│   └── tensi-bead.kicad_sch
├── firmware/
│   ├── CMakeLists.txt
│   ├── sdkconfig
│   ├── include/
│   └── src/
├── hardware/
│   └── BOM.csv
├── docs/
│   ├── api-reference.md
│   └── assembly-guide.md
└── scripts/
    ├── analyze_capture.py
    └── generate_demo_capture.py
```

## Future Extensions

- full Young-Laplace non-linear fit with iterative density correction
- dual-camera side/top imaging for anisotropic wetting
- UV backlight for fluorescent surfactant tracing
- disposable cartridge for hazardous liquids
- Home Assistant or unified TREE node integration for lab fleets
