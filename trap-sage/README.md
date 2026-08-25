# Trap Sage — Clamp-On Steam Trap & Cavitation Inspector

> A handheld ultrasonic + thermal maintenance instrument that clamps onto steam traps, condensate lines, and adjacent valves to distinguish healthy cycling from blow-through, blockage, flooding, and cavitation.

## What It Is

**Trap Sage** is a practical industrial maintenance node built around the **ESP32-S3-WROOM-1-N8**. It helps facilities teams, boiler operators, campus utilities crews, hospitals, food plants, laundries, and district-energy technicians answer a stubborn question quickly:

**Is this steam trap working correctly, wasting live steam, waterlogged, blocked shut, or is the nearby valve cavitating?**

Instead of relying only on a mechanic's stethoscope and touch judgement, Trap Sage combines:

- a **clamp-on contact ultrasonic sensor**
- dual **pipe contact temperature sensors**
- a local **ambient compensation sensor**
- a simple **force-sensed spring clamp**
- on-device DSP that scores burstiness, duty cycle, and cycle rate
- OLED + haptic + buzzer feedback for gloved field use
- microSD/BLE/Wi-Fi logging for asset history

## Why This Device Matters

Failed steam traps can waste large amounts of energy, collapse process temperature control, hammer return lines, and hide mechanical problems until damage is expensive. Typical maintenance routes still depend on technician experience and proprietary trap testers. Trap Sage is designed as a **field-buildable open instrument** for repeatable inspections.

## Main SoC Choice

### ESP32-S3-WROOM-1-N8

The ESP32-S3 is a good fit because it provides:

- enough CPU for envelope DSP and rule-based classification
- I2S for a dedicated ultrasonic audio ADC
- Wi-Fi for plant maintenance uploads
- BLE for mobile route apps
- native USB for bench calibration and firmware updates
- plenty of GPIO for clamp switch, haptics, OLED, SD, and status LEDs

## Functional Overview

1. The spring clamp is attached with one jaw upstream of the trap and one jaw downstream.
2. Two **TMP117** contact pads measure upstream and downstream pipe-wall temperatures.
3. A **piezo contact sensor** bonded to the upstream jaw listens to ultrasonic/mechanical energy from 8-60 kHz.
4. An **OPA1652** analog front end band-pass filters and amplifies the contact signal.
5. A **PCM1808** I2S stereo ADC digitizes the ultrasonic channel for the ESP32-S3.
6. A **FSR strip** behind the clamp hinge confirms adequate contact force.
7. Firmware computes:
   - ultrasonic RMS level
   - crest factor
   - burstiness / quiet-interval contrast
   - estimated cycle rate
   - upstream-to-downstream temperature drop
   - combined health score and likely fault class
8. Results are shown locally and logged per asset.

## Target Fault Classes

- **NORMAL_CYCLING** — intermittent discharge bursts, healthy temperature drop
- **BLOW_THROUGH** — continuous high ultrasonic energy with very small temperature drop
- **COLD_BLOCKED** — little acoustic activity and a very large temperature drop to a cold outlet
- **FLOODING** — moderate/continuous activity and reduced but non-zero ΔT indicating condensate backup
- **CAVITATING_VALVE** — strong noisy broadband contact signal on a lower-temperature valve body or bypass path
- **SENSOR_MISPLACED** — poor clamp force or an implausible signal pattern

## Block Diagram

```text
        ┌──────────────────────────── Trap Sage ────────────────────────────┐
        │                                                                    │
 Clamp jaw A ─► TMP117 upstream pad ── I2C ─┐                                │
 Clamp jaw B ─► TMP117 downstream pad ─ I2C ├───────────────┐                │
 Ambient air ─► SHT45 temp/RH ───────── I2C ┤               │                │
 Clamp hinge ─► FSR divider ─────────── ADC ┤               │                │
 RTC ─► DS3231 ──────────────────────── I2C ┤               │                │
                                             ▼               │                │
 Piezo contact puck ─► OPA1652 BPF ─► PCM1808 I2S ADC ─────► ESP32-S3-WROOM │
                                                             - DSP envelope   │
                                                             - trap classifier│
                                                             - asset logger   │
                                                             - BLE / Wi-Fi    │
                                             ▲               │                │
 OLED SH1106 ────────────────────────────────┤               │                │
 DRV2605 haptic driver ──────────────────────┤               │                │
 Buzzer / buttons / RGB ─────────────────────┤               │                │
 microSD ──────────────────────────────────── SPI ───────────┘                │
 USB-C / Li-ion ─► charger + 3V3 buck-boost + analog LDO rails               │
        └─────────────────────────────────────────────────────────────────────┘
```

## Full Schematic Design

### Major ICs and Subsystems

| Ref | Part | Function |
|-----|------|----------|
| U1 | ESP32-S3-WROOM-1-N8 | Main SoC, DSP, logging, BLE/Wi-Fi |
| U2 | PCM1808DB | 24-bit stereo audio ADC used as ultrasonic contact-sensor digitizer |
| U3 | OPA1652AIDR | Low-noise dual op-amp for band-pass gain stage |
| U4 | TMP117AIDRVR | Upstream pipe pad temperature |
| U5 | TMP117AIDRVR | Downstream pipe pad temperature |
| U6 | SHT45-AD1B | Ambient temperature and humidity compensation |
| U7 | DS3231SN | RTC for timestamped rounds |
| U8 | DRV2605L | Haptic alert driver for no-look diagnosis |
| DS1 | SH1106 128x64 OLED | Local display |
| J1 | microSD socket | Route logging |
| U9 | MCP73831 | 1-cell Li-ion charger |
| U10 | TPS63031 | Main 3.3 V buck-boost rail |
| U11 | TLV75533P | Quiet analog 3.3 V rail for op-amp + ADC |
| U12 | ADS1115IDGST | Auxiliary ADC for battery, clamp force, and analog diagnostics |
| U13 | TLV3691 | Analog overload comparator for front-end clip/fault detect |
| Q1 | AO3400A | Buzzer low-side driver |
| Q2 | AO3400A | Sensor rail power gating |
| FSR1 | Interlink 402 short FSR | Clamp-force verification |
| X1 | Piezo contact puck, 27 mm | Ultrasonic/acoustic pickup |
| B1 | 2000-3000 mAh Li-ion cell | Portable power |

### Contact Ultrasonic Front End

The acoustic jaw uses a bonded piezo puck feeding a two-stage analog path:

1. **Charge-to-voltage interface** using OPA1652 input bias network
2. **Band-pass shaping** centered on the fault-rich 12-42 kHz region
3. **Selectable gain** (×4 / ×12) using analog switch pads
4. **PCM1808** digitization at 96 kS/s over I2S

Recommended values:

- input bleed: 1 MΩ
- high-pass pole: 6.8 nF / 2.2 kΩ ≈ 10.6 kHz
- low-pass pole: 1 nF / 3.9 kΩ ≈ 40.8 kHz
- nominal midband gain: 11.7 dB

### Thermal Path

- **TMP117 upstream** is mounted in a copper jaw shoe with silicone thermal pad
- **TMP117 downstream** mirrors the same geometry on the outlet side
- the clamp is spring loaded to equalize contact pressure
- firmware uses only the temperature difference and trend, avoiding false certainty from absolute steam pressure assumptions

### Clamp Force Verification

An **FSR strip** under the hinge detects whether the operator has fully seated the clamp. This avoids bad classifications caused by weak coupling.

### Data / UI / Storage

- **I2C bus**: TMP117 x2, SHT45, DS3231, DRV2605L, OLED, ADS1115
- **I2S bus**: PCM1808 ADC
- **SPI bus**: microSD
- **UART0**: debug and manufacturing calibration

## ESP32-S3 Pin Assignment

| ESP32-S3 Pin | Function |
|--------------|----------|
| GPIO1 | UART0_TX |
| GPIO2 | UART0_RX |
| GPIO4 | BUZZER_EN |
| GPIO5 | HAPTIC_INT / DRV2605 status |
| GPIO6 | RGB_LED_R |
| GPIO7 | RGB_LED_G |
| GPIO8 | RGB_LED_B |
| GPIO9 | BUTTON_MODE |
| GPIO10 | BUTTON_MARK |
| GPIO11 | BUTTON_ROUTE |
| GPIO12 | I2C_SDA |
| GPIO13 | I2C_SCL |
| GPIO14 | OLED_RESET |
| GPIO15 | I2S_BCLK |
| GPIO16 | I2S_LRCLK |
| GPIO17 | I2S_DATA_IN |
| GPIO18 | SD_CS |
| GPIO19 | SPI_MOSI |
| GPIO20 | SPI_SCK |
| GPIO21 | SPI_MISO |
| GPIO35 | BATTERY_ADC via ADS1115 alert / spare analog monitor |
| GPIO36 | SENSOR_RAIL_EN |
| GPIO37 | ANALOG_CLIP_FAULT |
| GPIO38 | USB_PRESENT |
| GPIO39 | SERVICE_PAD / wake |

> Final layout must re-check strapping pins against the exact ESP32-S3 module revision.

## Power Architecture

### Inputs

- USB-C 5 V for charging and bench operation
- 1-cell Li-ion battery for field use

### Rails

- **VBAT** — raw battery
- **VSYS_3V3** — TPS63031 main digital rail
- **ANA_3V3** — TLV75533 low-noise rail for OPA1652 + PCM1808 reference domain
- **SENS_3V3** — switched sensor rail via AO3400A + load switch

### Estimated Consumption

| Mode | Current |
|------|---------|
| Deep sleep asset cache only | ~210 µA |
| Idle UI + sensors | ~46 mA |
| Live ultrasonic inspection | ~78 mA |
| Wi-Fi upload + SD logging peak | ~142 mA |

With a 2500 mAh cell, route runtime is about **22-26 hours of intermittent inspection**.

## Mechanical Architecture

- 3D-printed glass-filled nylon or CNC Delrin clamp body
- interchangeable curved copper jaw shoes for 1/2 in, 3/4 in, and 1 in pipe
- silicone thermal pads over each TMP117 pocket
- rubber-isolated electronics handle to reduce hand-borne noise
- magnetic docking cap for the piezo jaw when stored

## Firmware Architecture

The included firmware folder is a **host-buildable C simulation** of the embedded DSP chain. It demonstrates how the device fuses thermal and acoustic features before a full ESP-IDF port.

### Source Layout

```text
firmware/
├── CMakeLists.txt
├── include/
│   ├── trap_sage.h
│   ├── sensors.h
│   ├── dsp.h
│   ├── thermals.h
│   ├── classifier.h
│   └── ui.h
└── src/
    ├── main.c
    ├── sensors.c
    ├── dsp.c
    ├── thermals.c
    ├── classifier.c
    └── ui.c
```

### Diagnostic Feature Chain

```text
piezo envelope + upstream/downstream temperatures + clamp force
    -> RMS level
    -> crest factor
    -> burstiness
    -> active duty fraction
    -> cycle-rate estimate from autocorrelation
    -> delta-T and superheat proxy
    -> rule-based fault class + 0..100 health score
```

## Build and Run the Firmware Simulation

```bash
cd firmware
cmake -S . -B build
cmake --build build
./build/trap_sage_fw_sim
```

## Example Output

```text
Trap Sage diagnostic simulation
asset_id,classification,upstream_C,downstream_C,deltaT_C,ultra_rms,burstiness,active_fraction,cycle_rate_cpm,health
BLDG-A-TRAP-07,NORMAL_CYCLING,...
BLDG-A-TRAP-11,BLOW_THROUGH,...
```

## BOM

See [`hardware/BOM.csv`](hardware/BOM.csv) for the build list.

## Documentation

- [Assembly guide](docs/assembly-guide.md)
- [API reference](docs/api-reference.md)
- [Sample log](docs/sample-log.csv)

## Helper Scripts

- `scripts/generate_fixture_log.py` — synthesizes a maintenance route CSV
- `scripts/analyze_trap_log.py` — summarizes route severity by fault class

## Validation Notes

This repository revision includes:

- full device README
- conceptual KiCad project files
- compile-tested C firmware simulation
- route-log helper scripts
- BOM and assembly docs

## Future Extensions

- pressure-aware steam saturation lookup using operator-entered line pressure
- magnetic asset tags with NFC route binding
- onboard TinyML anomaly detection for plant-specific signatures
- optional intrinsically safe enclosure variant for hazardous areas
