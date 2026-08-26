# Chloro Clip — Pocket PAM Chlorophyll Fluorometer

> A clip-on leaf photosynthesis health meter that quantifies **Fo, Fm, Fv/Fm, ΦPSII, NPQ, and qP** using modulated fluorescence, then turns those numbers into actionable drought / heat / nutrient stress diagnostics.

## What It Is

**Chloro Clip** is a field-buildable handheld chlorophyll fluorescence instrument built around the **ESP32-S3-WROOM-1-N8**. It clamps onto a leaf, delivers a low-duty modulated blue measuring beam, fires a short saturating flash, and reads the red/far-red chlorophyll fluorescence response through an optical filter and transimpedance front end.

The result is a portable plant-physiology tool for growers, agronomists, greenhouse operators, researchers, and educators who need more than a simple color meter.

## Why This Device Matters

Plant stress often becomes visible only after yield has already been lost. Chlorophyll fluorescence reveals stress **before** leaves wilt or yellow.

Chloro Clip targets practical use cases:

- irrigation scheduling from early photochemical stress
- greenhouse heat-stress checks at canopy level
- nutrient deficiency screening before visible chlorosis
- cultivar comparison in breeding trials
- school and university photosynthesis labs
- rapid field scouting after herbicide drift or transplant shock

Commercial PAM fluorometers are often expensive and closed. Chloro Clip is designed to be open, documented, and buildable from common parts.

## Main SoC Choice

### ESP32-S3-WROOM-1-N8

The ESP32-S3 is a strong fit because it provides:

- enough CPU for fluorescence feature extraction and classification
- native USB for calibration and service
- BLE + Wi-Fi for phone sync and greenhouse dashboards
- flexible timers / RMT / LED PWM for modulated optical drive
- SPI bandwidth for microSD logging and optional TFT/OLED updates
- low enough system cost to keep the total BOM practical

## Measurement Principle

Chloro Clip implements a simplified **pulse-amplitude modulation (PAM)** workflow:

1. A blue measuring LED pulses at low duty cycle so it excites fluorescence without strongly driving photosynthesis.
2. A filtered photodiode + TIA observes emitted chlorophyll fluorescence near 680–740 nm.
3. In dark-adapted mode, the device captures:
   - **Fo**: minimal fluorescence
   - **Fm**: maximal fluorescence under a saturating flash
   - **Fv/Fm = (Fm - Fo)/Fm**
4. In light-adapted mode, the device captures:
   - **Fs**: steady-state fluorescence
   - **Fm'**: maximal light-adapted fluorescence
   - **Fo'** after a far-red relaxation pulse
5. Firmware derives:
   - **ΦPSII = (Fm' - Fs)/Fm'**
   - **NPQ = (Fm - Fm')/Fm'**
   - **qP = (Fm' - Fs)/(Fm' - Fo')**
6. A rule-based classifier turns those metrics into a field diagnosis.

## Functional Overview

```text
Leaf clip closes
  -> hall switch confirms clamp position
  -> blue measuring LED emits low-duty pulses
  -> photodiode + optical long-pass filter collect chlorophyll fluorescence
  -> ADS122C04 digitizes TIA output
  -> white/blue saturating flash LED fires for 300 ms
  -> optional far-red LED assists Fo' recovery estimate
  -> ESP32-S3 computes Fo/Fm/Fv/Fm/Fs/Fm'/Fo'/ΦPSII/NPQ/qP
  -> result shown on OLED, stored on microSD, broadcast over BLE/Wi-Fi
```

## Block Diagram

```text
┌──────────────────────────── Chloro Clip ────────────────────────────┐
│                                                                    │
│  Blue measuring LED ─┐                                             │
│  White saturating LED ├─► leaf clip optics ─► 680 nm LP filter ─┐  │
│  Far-red LED ─────────┘                                           │  │
│                                                                    ▼  │
│                                                         BPW34 photodiode
│                                                                    │
│                                              OPA320 transimpedance amp
│                                                                    │
│                                                    ADS122C04 24-bit ADC
│                                                                    │
│ TMP117 leaf temp ──────────────── I2C ─┐                           │
│ VEML7700 ambient PAR proxy ────── I2C ─┤                           │
│ DS3231 RTC ────────────────────── I2C ─┤                           │
│ SH1106 OLED ───────────────────── I2C ─┤                           │
│ Hall clamp switch ─────────────── GPIO │                           │
│ Buttons / buzzer / RGB ────────── GPIO │                           │
│ microSD socket ────────────────── SPI  │                           │
│ LED current DAC MCP4725 ───────── I2C ─┤                           │
│                                      ┌─▼─────────────────────────┐  │
│                                      │ ESP32-S3-WROOM-1-N8      │  │
│                                      │ - LED timing engine       │  │
│ USB-C / Li-ion charger / 3V3 rail ─►│ - fluorescence DSP        │  │
│                                      │ - diagnosis logic         │  │
│                                      │ - BLE/Wi-Fi + logging     │  │
│                                      └───────────────────────────┘  │
└────────────────────────────────────────────────────────────────────┘
```

## Full Schematic Design

### Major ICs and Subsystems

| Ref | Part | Function |
|-----|------|----------|
| U1 | ESP32-S3-WROOM-1-N8 | Main SoC, timing engine, UI, logging, BLE/Wi-Fi |
| U2 | ADS122C04IPWR | 24-bit delta-sigma ADC for fluorescence TIA readout |
| U3 | OPA320AIDBVR | Low-bias transimpedance amplifier for photodiode |
| U4 | MCP4725A0T-E/CH | I2C DAC for measuring LED current setpoint |
| U5 | TMP117AIDRVR | Leaf contact temperature sensor |
| U6 | VEML7700 | Ambient light / PAR proxy sensor |
| U7 | DS3231SN | RTC with timestamp retention |
| DS1 | SH1106 128x64 OLED | Local display |
| J1 | microSD push-push socket | Local session logging |
| U8 | MCP73831T-2ACI | Single-cell Li-ion charger |
| U9 | TPS63031DSKR | 3.3 V buck-boost regulator |
| U10 | TPS22918DBVR | Optics rail switch |
| Q1 | AO3400A | Saturating flash LED low-side switch |
| Q2 | AO3400A | Far-red LED low-side switch |
| Q3 | AO3400A | Buzzer driver |
| D1 | Cree XLamp XP-E2 blue | Measuring LED |
| D2 | Nichia NF2W757GR-V3 | Saturating flash LED |
| D3 | SFH4715AS | Far-red recovery LED |
| PD1 | BPW34 | Silicon photodiode |
| SW1 | A3213 hall switch | Clamp closed detect |

### Optical Front End

- **Blue measuring LED (470 nm)** is driven at low average current using PWM bursts and analog setpoint trimming.
- **Saturating flash LED** delivers a short high-current optical pulse to close PSII reaction centers and measure **Fm** or **Fm'**.
- **Far-red LED (~730 nm)** helps estimate **Fo'** during light-adapted workflows.
- A **black acetal clip tunnel** isolates ambient light.
- The photodiode sees the leaf through a **680 nm long-pass or 690 nm band-pass filter** to reject excitation leakage.

### Analog Chain

- **BPW34** runs in photoconductive mode for lower capacitance and faster settling.
- **OPA320** is configured as a transimpedance amplifier with switchable feedback resistor footprints:
  - 330 kΩ default for broad crop range
  - 1 MΩ for low-signal thin leaves
- ADC sampling is performed by **ADS122C04** over I2C.
- A grounded guard ring is recommended around the TIA input node on the PCB.

### LED Drive Network

- **MCP4725 DAC** sets measuring LED current reference.
- **ESP32-S3 PWM/RMT** gates the measuring LED in a known pulse pattern.
- **Q1** drives the saturating flash LED from VBAT to avoid brownout on the 3V3 rail.
- **Q2** gates the far-red LED.
- A 0.05 Ω current-sense resistor footprint is included for flash characterization during bring-up.

### Data / UI / Storage

- **I2C bus**: ADS122C04, MCP4725, TMP117, VEML7700, DS3231, OLED
- **SPI bus**: microSD
- **UART0 / USB**: manufacturing console and calibration
- **GPIOs**: hall switch, buttons, buzzer, RGB LED, flash trigger, far-red gate

## ESP32-S3 Pin Assignment

| ESP32-S3 Pin | Function |
|--------------|----------|
| GPIO1 | UART0_TX debug |
| GPIO2 | UART0_RX debug |
| GPIO4 | BUZZER_EN |
| GPIO5 | FLASH_LED_EN |
| GPIO6 | FAR_RED_EN |
| GPIO7 | MEASURE_LED_PWM |
| GPIO8 | RGB_LED_R |
| GPIO9 | RGB_LED_G |
| GPIO10 | RGB_LED_B |
| GPIO11 | BUTTON_MODE |
| GPIO12 | BUTTON_MARK |
| GPIO13 | HALL_CLAMP_DETECT |
| GPIO14 | OLED_RESET |
| GPIO15 | I2C_SDA |
| GPIO16 | I2C_SCL |
| GPIO17 | SD_CS |
| GPIO18 | SPI_MOSI |
| GPIO19 | SPI_SCK |
| GPIO20 | SPI_MISO |
| GPIO35 | BATTERY_ADC |
| GPIO36 | OPTICS_RAIL_EN |
| GPIO37 | FLASH_CURRENT_MONITOR |
| GPIO38 | USB_DETECT |
| GPIO39 | SERVICE_PAD / wake |

> Final PCB layout must verify module strapping, flash current transients, and optical-noise isolation before fabrication.

## Measurement Modes

### 1. Dark-Adapted Fv/Fm
A leaf is dark-clipped for 15-20 minutes, then Fo and Fm are measured to assess maximum PSII efficiency.

### 2. Light-Adapted Yield
Leaf remains in ambient light; device records Fs, Fm', and Fo' to estimate effective quantum yield.

### 3. Rapid Stress Scout
Short workflow optimized for greenhouse rounds. Less accurate than full dark-adapted mode but useful for screening.

### 4. Classroom Demo
Runs repeated pulse sequences and streams raw curves for teaching photosynthesis.

## Diagnosis Classes

- **HEALTHY** — Fv/Fm near 0.79-0.84 and solid ΦPSII
- **MILD_DROUGHT_STRESS** — moderate NPQ rise and reduced photochemical efficiency
- **HEAT_STRESS** — high leaf temperature with suppressed yield and elevated non-photochemical quenching
- **PHOTOINHIBITION** — strongly depressed Fv/Fm indicating damaged or overexcited PSII
- **NITROGEN_DEFICIENCY** — acceptable dark efficiency but poor operating yield / low qP under light
- **CLAMP_ERROR** — weak optical coupling or bad dark seal

## Power Architecture

### Input Sources

- USB-C 5 V for charging and bench use
- 1-cell 3.7 V Li-ion / LiPo battery

### Rails

- **VBAT**: raw cell rail for flash LED pulse energy
- **VSYS_3V3**: buck-boost regulated digital / logic rail
- **OPT_3V3**: switched quiet analog / optics rail

### Estimated Consumption

| Mode | Current |
|------|---------|
| Deep sleep | ~160 µA |
| Idle, OLED on | ~38 mA |
| Measuring LED active | ~54 mA |
| Saturating flash pulse | ~420 mA peak from VBAT |
| Wi-Fi sync burst | ~180 mA |

With a 1200 mAh LiPo, expected runtime is roughly **3000-4000 spot measurements** or a full day of intermittent scouting.

## Firmware Architecture

```text
sensor acquisition
  -> fluorescence curve extraction
  -> Fo/Fm/Fs/Fm'/Fo' derivation
  -> ΦPSII/NPQ/qP computation
  -> diagnosis rules
  -> OLED / CSV / BLE / Wi-Fi export
```

The included C firmware is a **host-buildable simulation** that exercises the metric pipeline on representative captures.

## Build and Run

```bash
cd firmware
cmake -S . -B build
cmake --build build
./build/chloro_clip_fw_sim
```

## Calibration Workflow

1. Measure dark current with clip closed and no leaf.
2. Adjust measuring LED current until raw fluorescence remains linear across a healthy reference leaf.
3. Characterize saturating pulse current and ensure Fm plateaus before pulse end.
4. Store photodiode gain and LED coefficients in NVS.
5. Validate against a known healthy leaf target with expected Fv/Fm near 0.80.

## Bill of Materials

A complete BOM is provided in [`hardware/BOM.csv`](hardware/BOM.csv). Estimated prototype material cost is approximately **$49-55**, depending on optics and enclosure choices.

## Repository Contents

```text
chloro-clip/
├── README.md
├── schematic/
├── firmware/
├── hardware/
├── docs/
└── scripts/
```

## Suggested Mechanical Design

- spring clip body in matte black nylon or ASA
- removable rubber leaf pads to avoid bruising
- optical tunnel lined with flocking tape
- thumb-operated trigger for repeatable clamp pressure
- optional detachable dark-adaptation shutter cap

## Practical Limitations

- This is a screening fluorometer, not a lab-grade imaging system.
- Readings depend on dark adaptation, leaf geometry, and optical seal quality.
- PAR is estimated from a proxy ambient sensor, not a full quantum sensor.
- Thin or waxy leaves may require gain profile changes.

## Future Extensions

- add FRAM for power-loss-safe logging
- add machine-learned crop-specific stress models
- integrate QR crop recipes and species metadata
- add unified TREE telemetry role as an Observe node
