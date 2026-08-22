# Pipe Halo — Clamp-On Plumbing Freeze & Leak Sentinel

> A non-invasive pipe monitor that straps onto copper, PEX, or steel plumbing, listens to structure-borne flow noise, watches pipe-wall temperature, and predicts freeze, drip-leak, water-hammer, and valve-tamper events before they become expensive damage.

## What It Is

**Pipe Halo** is a buildable smart-home / light-commercial infrastructure node built around the **ESP32-C6-WROOM-1-N8**. Instead of cutting into plumbing, it clamps onto the outside of a pipe and fuses:

- **dual RTD pipe temperature measurements**
- **ambient humidity / temperature**
- **contact acoustic energy from a piezo film sensor**
- **low-g accelerometer vibration**
- **magnetic valve-position sensing**
- **time-domain leak rhythm analysis**

The result is a device that can answer practical questions homeowners, facilities teams, and landlords care about:

- Is this line flowing normally, dripping, or being hammered?
- Is the pipe at risk of freezing in the next hour?
- Was the nearby shutoff valve opened or left partially closed?
- Is a crawlspace or wall cavity becoming humid enough to support mold after a micro-leak?

## Why This Device Matters

Most water-damage monitoring starts **after** water is already on the floor. Pipe Halo is meant to move detection earlier:

- **Freeze prevention** for vacation homes, cabins, RVs, and mechanical rooms
- **Leak triage** for under-sink plumbing, crawlspaces, attics, and risers
- **Infrastructure telemetry** for apartments, schools, and light commercial buildings
- **Retrofit-friendly deployment** because the sensor never touches potable water

## Main SoC Choice

### ESP32-C6-WROOM-1-N8

The ESP32-C6 is a good fit because it combines:

- Wi-Fi 6 for direct building-network connectivity
- BLE 5 for commissioning and local diagnostics
- low-power operation suitable for battery-backed monitoring
- enough CPU for feature extraction and rule-based classification
- plentiful GPIO / I2C / SPI / ADC resources for mixed-signal sensing

## Functional Overview

1. A spring clamp presses a **piezo film strip** and **MEMS accelerometer board** against the pipe.
2. Two **PT1000 RTDs** in silicone wrap bands measure pipe-wall temperature upstream and downstream.
3. An **SHT45** measures local ambient temperature and humidity.
4. A **TMAG5273** Hall sensor looks at a small magnet attached to the shutoff valve handle.
5. The ESP32-C6 periodically samples:
   - acoustic envelope from the piezo front-end
   - vibration magnitude from LIS2DW12
   - valve angle estimate from the Hall vector
   - ambient and pipe temperature
6. DSP computes:
   - flow index
   - drip periodicity confidence
   - water-hammer impulsiveness
   - freeze margin
   - burst-risk score
7. Results are shown locally on an OLED, logged to microSD, and exported over BLE/Wi-Fi MQTT/HTTP.

## Block Diagram

```text
                    ┌──────────────── Pipe Halo ────────────────┐
                    │                                           │
Pipe wall ─► piezo film ─► OPA320 gain/filter ─► ADC ┐          │
                    │                                │          │
                    │   LIS2DW12 accel ── I2C ──────┤          │
                    │   SHT45 temp/RH ─── I2C ──────┤          │
                    │   TMAG5273 Hall ─── I2C ──────┤          │
                    │   DS3231 RTC ────── I2C ──────┤          │
                    │                                ▼          │
                    │                     ESP32-C6-WROOM-1-N8  │
                    │                  - feature extraction     │
                    │                  - state classification   │
                    │                  - OLED UI                │
                    │                  - MQTT / BLE telemetry   │
                    │                  - SD logging             │
                    │                                           │
PT1000 #1 ─► MAX31865 SPI ─┐                                   │
PT1000 #2 ─► MAX31865 SPI ─┼── SPI ────────────────────────────┤
microSD ───────────────────┤                                   │
OLED 128x64 ───────────────┴── I2C                             │
Buzzer / RGB LED / buttons ─── GPIO                            │
12 V in / LiFePO4 backup ─► buck 3V3 / charger / ideal OR-ing  │
                    └───────────────────────────────────────────┘
```

## Full Schematic Design

### Major ICs and Subsystems

| Ref | Part | Function |
|-----|------|----------|
| U1 | ESP32-C6-WROOM-1-N8 | Main SoC, telemetry, DSP, UI |
| U2 | OPA320AIDBVR | Low-noise rail-to-rail piezo amplifier / envelope front-end |
| U3 | LIS2DW12TR | Low-power 3-axis vibration sensor |
| U4 | SHT45 | Ambient temperature + humidity |
| U5 | TMAG5273A1 | 3-axis Hall sensor for valve position / tamper |
| U6 | MAX31865 | RTD interface for PT1000 #1 |
| U7 | MAX31865 | RTD interface for PT1000 #2 |
| U8 | DS3231SN | RTC with battery backup |
| DS1 | SH1106 1.3 in OLED | Local display |
| J1 | microSD socket | Logging storage |
| U9 | TPS62162DSG | 12 V / battery to 3.3 V main buck regulator |
| U10 | MCP73871 | Backup cell charger / power-path controller |
| U11 | TPS22918 | Sensor rail load switch |
| B1 | LiFePO4 14500 cell | Backup during mains drop |
| Q1-Q3 | AO3400A | buzzer / LED / heater / misc switching |
| LS1 | Piezo film strip | structure-borne flow + drip acoustics |
| RTD1 | PT1000 strap sensor | upstream pipe-wall temperature |
| RTD2 | PT1000 strap sensor | downstream pipe-wall temperature |
| M1 | 3 V magnetic buzzer | local alarm |

### Analog Front End

The piezo strip is AC-coupled into **OPA320** configured as a non-inverting gain stage (~26 dB) with a mid-supply virtual reference at 1.65 V.

- High-pass corner: ~8 Hz to reject slow thermal drift / handling
- Low-pass corner: ~2.4 kHz to focus on plumbing signatures
- Envelope detector node optionally sampled by ADC for low-power monitoring
- Raw amplified waveform routed to ESP32-C6 ADC1_CH3 through a 220 Ω series resistor and clamp network

### Pipe Temperature Sensing

Two **PT1000 Class B RTDs** are used instead of NTCs for better repeatability and easier field replacement.

- **U6/MAX31865** reads RTD1 at the supply side
- **U7/MAX31865** reads RTD2 at the room/load side
- 4-wire Kelvin routing recommended from RTD tails to the PCB JST connectors
- The firmware uses the gradient `RTD1 - RTD2` to estimate moving water and detect stagnant cold sections

### Motion / Valve Sensing

- **LIS2DW12**: detects continuous vibration and impulsive hammer events
- **TMAG5273**: reads a small adhesive N52 magnet mounted on the shutoff handle; valve angle is inferred from the X/Y field vector

### Communications and Logging

- I2C bus: LIS2DW12, SHT45, TMAG5273, DS3231, OLED
- SPI bus 0: microSD
- SPI bus 1: MAX31865 #1 and #2 with separate chip selects
- UART0 exposed on a debug header for commissioning and manufacturing test

## ESP32-C6 Pin Assignment

| ESP32-C6 Pin | Function |
|--------------|----------|
| GPIO0 | BOOT / service pad |
| GPIO1 | UART0_TX debug |
| GPIO2 | UART0_RX debug |
| GPIO3 | PIEZO_ADC |
| GPIO4 | BUZZER_EN |
| GPIO5 | RGB_LED_R |
| GPIO6 | RGB_LED_G |
| GPIO7 | RGB_LED_B |
| GPIO8 | I2C_SDA |
| GPIO9 | I2C_SCL |
| GPIO10 | OLED_RESET |
| GPIO11 | SD_CS |
| GPIO12 | SPI_MOSI |
| GPIO13 | SPI_MISO |
| GPIO14 | SPI_SCK |
| GPIO15 | RTD1_CS |
| GPIO16 | RTD2_CS |
| GPIO17 | BUTTON_ACK |
| GPIO18 | BUTTON_MODE |
| GPIO19 | TAMPER_LOOP |
| GPIO20 | SENSOR_RAIL_EN |
| GPIO21 | BATTERY_ADC |
| GPIO22 | POWER_GOOD |
|
> Final PCB layout should verify strapping-pin behavior and ADC attenuation constraints for the selected ESP32-C6 module revision.

## Detection Modes

### 1. Normal Flow
Stable broad-spectrum pipe noise, moderate vibration, acceptable freeze margin.

### 2. Drip Leak
Periodic acoustic bursts with low overall flow index and high drip periodicity confidence.

### 3. Water Hammer
High crest-factor impulsive event plus short vibration spike.

### 4. Freeze Risk
Pipe wall near 0 °C with low or absent flow and cold ambient trend.

### 5. Burst Risk Escalation
Freeze risk plus rapid pressure/thermal transients or repeated hammering.

### 6. Valve Tamper / Partial Closure
Hall-based valve angle differs significantly from the commissioned fully-open baseline.

## Signal Processing Chain

```text
piezo samples (2 kHz, 2 s window)
    -> DC removal
    -> RMS energy
    -> crest factor
    -> zero-crossing rate
    -> simple 4-band Goertzel energy split
    -> drip periodicity estimate
    -> fused with accelerometer RMS, pipe temperatures, humidity, valve angle
    -> rule-based classifier with hysteresis
```

### Derived Metrics

- **flow_index** = weighted mix of acoustic RMS, vibration RMS, and temperature gradient
- **drip_confidence** = periodicity score from repeated transients in the 0.2-3.0 s range
- **hammer_index** = impulsiveness score from crest factor and accelerometer spikes
- **freeze_margin_c** = min(pipe temps) - 0.5 °C safety threshold
- **burst_risk_score** = combines freeze severity, humidity, hammer recurrence, and valve restriction

## Power Architecture

```text
12V wall adapter ──► TVS + resettable fuse ──► TPS62162 3V3 main rail
                                            ├─► ESP32-C6
                                            ├─► OLED / I2C sensors / microSD
                                            ├─► MAX31865 #1/#2
                                            └─► TPS22918 switched sensor rail

USB-C 5V / aux input ─► MCP73871 ─► LiFePO4 backup cell
                                  └─► ideal-OR support during mains failure

CR1220 ─► DS3231 backup only
```

### Estimated Current Draw

- deep idle with periodic wake: ~9 mA
- monitoring window active, Wi-Fi off: ~42 mA
- monitoring + OLED + SD write: ~63 mA
- Wi-Fi publish burst: 130-180 mA
- backup runtime on 600 mAh LiFePO4: roughly 8-12 hours depending on telemetry duty cycle

## Mechanical Design

- 3D-printed clamp body with replaceable silicone pipe pads
- piezo strip mounted under a floating pressure finger for repeatable contact force
- RTDs use stainless strap bands and thermal grease pads
- optional secondary remote-sensor cable for frozen crawlspaces
- enclosure targets **78 mm × 54 mm × 28 mm** electronics pod with a hinged clamp up to 32 mm pipe OD

## Firmware Layout

```text
firmware/
├── CMakeLists.txt
├── sdkconfig
├── include/
│   ├── pipe_halo.h
│   ├── sensors.h
│   ├── dsp.h
│   └── classifier.h
└── src/
    ├── main.c
    ├── sensors.c
    ├── dsp.c
    └── classifier.c
```

The included firmware is a **portable C reference implementation** of the sensing, feature extraction, and classification logic. It can be compiled on a desktop to validate the algorithms before porting into ESP-IDF tasks and drivers.

## Telemetry Model

Each log record contains:

- ISO8601 timestamp
- pipe temperature 1 / 2
- ambient temperature / humidity
- acoustic RMS and spectral centroid
- vibration RMS
- valve angle
- flow index
- risk state
- recommended action string

## Build Instructions

### Host simulation build

```bash
cd firmware
cmake -S . -B build
cmake --build build
./build/pipe_halo_demo
```

### ESP-IDF porting notes

- Replace the demo sensor providers in `sensors.c` with ADC / I2C / SPI drivers
- Move the main loop into a FreeRTOS task
- Replace `printf` logging with SD/MMC + MQTT + BLE GATT transport
- Store calibration baselines in NVS

## Calibration Workflow

1. Strap both RTDs to a known-temperature pipe and enter calibration mode.
2. Capture zero-flow background for 60 seconds.
3. Open faucet to low, medium, and high flows and store acoustic baselines.
4. Attach valve magnet and record fully-open / half-open / closed references.
5. Validate freeze threshold using an ice-water wrapped reference tube section.

## Example Use Cases

- vacation-home freeze alert under a sink
- apartment riser monitoring in utility closets
- dishwasher / washing-machine feed line leak detection
- crawlspace pipe telemetry feeding Home Assistant or unified TREE

## BOM Summary

See [`hardware/BOM.csv`](hardware/BOM.csv) for a complete line-item bill of materials.

- estimated one-off prototype electronics cost: **~$47-58**
- recommended PCB: **4-layer, 1.6 mm, 70 mm × 50 mm**
- assembly level: moderate; no BGA required if using the ESP32-C6 module

## unified TREE Role

Pipe Halo naturally acts as an **Observe** node with light **Coordinate** behavior:

- observes building-health signals
- publishes hazard states upstream
- can request intervention from actuator nodes such as smart shutoff valves, heaters, or sirens

## Repository Contents

- [`schematic/`](schematic/) — KiCad project skeleton and top-level interconnect description
- [`firmware/`](firmware/) — portable C reference firmware
- [`hardware/BOM.csv`](hardware/BOM.csv) — bill of materials
- [`docs/assembly-guide.md`](docs/assembly-guide.md) — mechanical and wiring assembly notes
- [`docs/api-reference.md`](docs/api-reference.md) — serial / JSON telemetry schema
- [`scripts/`](scripts/) — host-side log generation and analysis helpers

## Safety Notes

- This device is **advisory instrumentation**, not a certified life-safety system.
- Never rely on it as the only freeze-protection method for occupied buildings.
- Maintain electrical isolation and insulation when mounting near grounded metal plumbing.
- If used near potable-water lines, keep all wetted surfaces external; this design never contacts water directly.
