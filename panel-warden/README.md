# Panel Warden — Magnetic Partial-Discharge, Arc, and Thermal Sentinel for Electrical Panels

> A buildable open-hardware condition-monitoring node that mounts inside switchgear, motor-control cabinets, breaker panels, and combiner boxes to detect overheating, corona, tracking, intermittent arcing, and condensation risk before they become outages or fires.

## What It Is

**Panel Warden** is a practical predictive-maintenance instrument built around the **ESP32-S3-WROOM-1-N8**. It fuses four sensing modes:

1. **Infrared thermal imaging** for hot lugs, overloaded breakers, and imbalanced feeders.
2. **Ultrasonic partial-discharge listening** for corona and surface tracking.
3. **RF burst detection** for arcing and high-frequency discharge activity.
4. **Ambient humidity + temperature monitoring** for condensation and creepage-risk scoring.

The device magnetically attaches **inside** a metal electrical enclosure during a planned outage, then runs from an internal Li-ion pack with optional USB-C power. Once the enclosure is re-energized and closed, Panel Warden watches continuously and reports events over BLE, Wi-Fi, OLED, and microSD.

## Why This Device Matters

Electrical failures often begin as weak, early symptoms:

- a single lug warms up before visible discoloration
- corona starts hissing long before insulation fails
- intermittent arcs generate short RF bursts that humans miss
- humid mornings push bus insulation toward tracking conditions
- overloaded branch circuits show thermal asymmetry before tripping

Most inspections are periodic and manual. Panel Warden turns those invisible precursors into timestamped evidence.

## Main SoC Choice

### ESP32-S3-WROOM-1-N8

The ESP32-S3 is the right SoC for this device because it provides:

- dual-core MCU performance for acquisition + UI/network tasks
- Wi-Fi and BLE without a second wireless processor
- enough RAM and flash for buffering thermal frames and event logs
- SPI, I2C, I2S/PDM, ADC, GPIO, and deep-sleep support
- easy OTA firmware update path
- wide toolchain support and low BOM risk

Panel Warden uses the ESP32-S3 as the **single application SoC**, with precision front ends around it rather than a second measurement MCU.

## Functional Overview

1. The installer mounts the unit inside a de-energized panel using insulated magnets or DIN-rail clips.
2. A **MLX90640 32×24 IR array** watches busbars, lugs, breakers, contactors, and cable exits.
3. A **Murata MA40H1S-R ultrasonic receiver** plus analog gain/envelope chain listens for 20-80 kHz discharge activity.
4. An **AD8318 RF log detector** watches broadband EMI bursts from corona and arcing.
5. A **SHT45** ambient sensor estimates dewpoint and condensation margin.
6. Two **TMP117** sensors compare enclosure air temperature to a magnetic contact pad touching the cabinet wall near the hottest region.
7. An **ADS131M04** simultaneous-sampling ADC digitizes conditioned analog channels for synchronous event scoring.
8. The ESP32-S3 computes fault features, assigns a class, scores severity, stores a log, and emits alerts.

## Target Use Cases

- commercial breaker panels
- industrial MCC buckets
- pump and irrigation control cabinets
- rooftop solar combiner boxes and inverters
- EV charging distribution panels
- refrigeration and HVAC contactor cabinets
- marine and off-grid power enclosures

## Fault Classes

Panel Warden distinguishes these classes on-device:

- **NORMAL** — no abnormal thermal, RF, or ultrasonic indicators
- **OVERLOAD_HEATING** — elevated thermal rise correlated with sustained current
- **LOOSE_LUG** — localized hot spot with moderate current, suggesting bad torque or oxidation
- **CORONA_PD** — ultrasonic activity with modest RF and limited thermal rise
- **SURFACE_TRACKING** — humidity-assisted discharge activity near insulation surfaces
- **INTERMITTENT_ARC** — strong RF bursts and ultrasonic impulses, often with unstable temperature
- **CONDENSATION_RISK** — dew margin too small for safe long-term insulation reliability

## Block Diagram

```text
┌────────────────────────────── Panel Warden ──────────────────────────────┐
│                                                                          │
│  Li-ion / USB-C power                                                    │
│       │                                                                  │
│       ├─► MCP73831 charger ─► TPS63070 3V3 system rail ─► AP7333 analog  │
│       └─► MAX17048 fuel gauge                                            │
│                                                                          │
│  Murata ultrasonic receiver ─► OPA356 band-pass/envelope ─┐             │
│  RF E-field probe ─► AD8318 log detector ────────────────┤             │
│  Split-core CT input ─► burden + rectifier ──────────────┤             │
│  Contact thermistor / analog spare ──────────────────────┤             │
│                                                           ▼             │
│                               ADS131M04 4-ch simultaneous ADC           │
│                                         │ SPI                           │
│                                         ▼                               │
│                               ESP32-S3-WROOM-1-N8                      │
│                         ┌────────────┬────────────┬────────────┐        │
│                         │ I2C        │ SPI        │ GPIO/UART   │        │
│                         ▼            ▼            ▼             │        │
│                 MLX90640 IR array  microSD     OLED/buttons    │        │
│                 TMP117×2           log card    buzzer/LED       │        │
│                 SHT45              storage     service header   │        │
│                                                                          │
│                    BLE + Wi-Fi alerting / web dashboard / OTA            │
└──────────────────────────────────────────────────────────────────────────┘
```

## Full Schematic Design

### Major ICs and Subsystems

| Ref | Part | Function |
|-----|------|----------|
| U1 | ESP32-S3-WROOM-1-N8 | Main SoC, acquisition, classification, UI, BLE/Wi-Fi |
| U2 | ADS131M04IPBS | 4-channel 24-bit simultaneous ADC for analog event channels |
| U3 | MLX90640ESF-BAB | 32×24 IR thermal array |
| U4 | AD8318ACPZ | 1 MHz-8 GHz RF log detector |
| U5 | OPA356AIDBVR | Ultrasonic receive gain / active band-pass stage |
| U6 | MCP73831T-2ATI/OT | 1-cell Li-ion charger |
| U7 | TPS63070RNMR | 3.3 V buck-boost regulator |
| U8 | AP7333-33SAG-7 | Low-noise analog 3.3 V LDO |
| U9 | MAX17048G+T10 | Fuel gauge |
| U10 | SHT45-AD1B-R2 | Humidity and ambient temperature |
| U11 | TMP117AIDRVR | Cabinet-wall contact temperature |
| U12 | TMP117AIDRVR | Internal air temperature reference |
| DS1 | SH1106 128×64 OLED | Local status display |
| J1 | microSD socket | Event logging |
| J2 | 3.5 mm jack | Optional split-core CT accessory input |
| J3 | USB-C receptacle | Charging and service |
| BZ1 | 12 mm piezo buzzer | Audible alarm |
| X1 | Murata MA40H1S-R | Ultrasonic receiver transducer |
| B1 | 2000 mAh Li-ion pouch | Main power source |

### Sensing Topology

#### 1. Thermal Path

The **MLX90640** faces inward toward the electrical hardware. It runs on I2C at 400 kHz and delivers 32×24 thermal frames. Firmware extracts:

- hottest pixel temperature
- background median temperature
- hot-spot area above threshold
- left/right and top/bottom asymmetry
- rate of temperature rise over time

This is enough to separate a general enclosure warm-up from a single lug or breaker terminal issue.

#### 2. Ultrasonic Partial-Discharge Path

A **Murata MA40H1S-R** air ultrasonic receiver feeds an **OPA356** active band-pass stage centered near 40 kHz. The amplified signal is then envelope-detected and low-pass filtered into an **ADS131M04** channel.

Why an envelope path instead of raw 40 kHz capture?

- lower ADC bandwidth requirement
- reduced processing load on the ESP32-S3
- easier event energy estimation for corona / tracking / arc impulses
- better battery life for long unattended runs

#### 3. RF Burst Path

A short insulated E-field probe or PCB monopole couples broadband panel EMI into an **AD8318** logarithmic detector. Its analog output goes to the ADC. Firmware looks for:

- burst rate
- peak intensity
- duty factor
- coincidence with ultrasonic activity

Strong RF with sharp ultrasonic impulses suggests intermittent arcing more than benign corona.

#### 4. Current-Correlation Accessory Path

An optional split-core CT plugs into **J2**. A burden resistor, precision rectifier, and RC smoothing feed the ADC. This does not attempt revenue-grade current metering; it exists to answer one practical question:

> is the heat rise explained by real load, or is it abnormal for the load present?

That distinction helps separate **overload heating** from **loose lug heating**.

#### 5. Humidity / Condensation Path

The **SHT45** measures enclosure humidity and air temperature. Firmware computes dewpoint and condensation margin:

`dew_margin = contact_temperature - dewpoint`

If the margin is small or negative, insulation surfaces may support tracking, especially in dusty or contaminated cabinets.

### Buses and Interconnects

- **I2C0**: MLX90640, SHT45, TMP117 #1, TMP117 #2, OLED
- **SPI2**: ADS131M04 ADC
- **SPI3**: microSD
- **UART0**: USB/service console
- **GPIO**: buzzer, alert LED, user buttons, ADC DRDY, wake input
- **ADC1**: battery divider backup reading

## ESP32-S3 Pin Assignment

| Pin | Function |
|-----|----------|
| GPIO1 | I2C_SCL |
| GPIO2 | I2C_SDA |
| GPIO4 | OLED_RESET |
| GPIO5 | ALERT_LED |
| GPIO6 | BUZZER_EN |
| GPIO7 | BUTTON_MODE |
| GPIO8 | BUTTON_ACK |
| GPIO9 | ADC_DRDY |
| GPIO10 | ADC_RESET |
| GPIO11 | ADC_CS |
| GPIO12 | SPI2_MOSI |
| GPIO13 | SPI2_MISO |
| GPIO14 | SPI2_SCK |
| GPIO15 | SD_CS |
| GPIO16 | SPI3_MOSI |
| GPIO17 | SPI3_MISO |
| GPIO18 | SPI3_SCK |
| GPIO19 | BATTERY_ADC |
| GPIO20 | MAGNET_PRESENT / install switch |
| GPIO21 | CT_PRESENT detect |
| GPIO35 | USB/UART TX |
| GPIO36 | USB/UART RX |
| EN | Reset |

> Final PCB layout should re-check ESP32-S3 strapping and USB/JTAG constraints before fabrication.

## ADS131M04 Channel Assignment

| Channel | Source |
|---------|--------|
| CH0 | Ultrasonic envelope |
| CH1 | RF detector output |
| CH2 | CT/load correlation envelope |
| CH3 | Spare analog / thermistor / future vibration channel |

## Power Architecture

### Inputs

- **USB-C 5 V** for charging, bench power, and firmware service
- **1-cell Li-ion** battery for standalone operation

### Rails

- **VBAT** — raw battery rail
- **SYS_3V3** — buck-boost regulated digital rail from TPS63070
- **ANA_3V3** — low-noise analog rail from AP7333
- **SENSOR_3V3** — switched sensor rail for power savings

### Estimated Consumption

| Mode | Current |
|------|---------|
| Deep sleep sentinel | 1.8 mA |
| Periodic scan idle | 42 mA |
| Thermal frame + ADC active | 108 mA |
| Wi-Fi upload burst | 240 mA peak |
| Audible alarm active | +18 mA |

A 2000 mAh pack supports roughly:

- ~36-40 hours of continuous monitoring with periodic Wi-Fi uploads
- ~3-4 days with event-driven uploads and moderate thermal frame rate

## Protection and Mechanical Safety

Panel Warden is intended for **installation only when the enclosure is de-energized and locked out**. Safety provisions include:

- insulated magnetic feet or DIN-rail clip
- conformal-coating keep-outs around sensor apertures
- flame-rated enclosure plastic (UL94 V-0 recommended)
- PTC on USB input
- reverse-battery protection PFET
- ESD suppression on USB-C and accessory CT jack
- buzzer alert on over-temperature event or enclosure opening

## Mechanical Layout Guidance

Recommended stack-up:

- top face: OLED, buttons, status LED, buzzer vent
- front edge: USB-C and microSD
- underside: four insulated magnets or DIN spring clip
- inward-facing side: IR sensor window, ultrasonic transducer, RF probe trace

Keep the IR sensor away from the buck-boost inductor and provide a black matte baffle around the thermal field of view.

## Firmware Architecture

### Tasks

- **acquisition task** — polls ADC and temperature sensors
- **thermal task** — processes MLX90640 frames into hot-spot features
- **classifier task** — assigns fault class and risk score
- **logger task** — appends CSV and event packets to SD
- **connectivity task** — BLE advertisements, Wi-Fi dashboard, OTA
- **ui task** — OLED status, alarm acknowledge, battery state

### Event Features Used On-Device

- `hotspot_c`
- `delta_t_c`
- `hotspot_area_px`
- `rise_rate_c_per_hr`
- `ultrasound_db`
- `rf_dbm_equiv`
- `burst_rate_hz`
- `load_corr_a`
- `dew_margin_c`
- `confidence`

### Core Decision Logic

- high `delta_t` + elevated load + low RF/ultrasonic -> **OVERLOAD_HEATING**
- high `delta_t` + localized hotspot + modest load -> **LOOSE_LUG**
- strong ultrasonic + small thermal rise + modest RF -> **CORONA_PD**
- humid air + low dew margin + sustained ultrasonic + moderate RF -> **SURFACE_TRACKING**
- strong RF + high burst rate + impulsive ultrasonic -> **INTERMITTENT_ARC**
- low dew margin alone -> **CONDENSATION_RISK**

## Bill of Materials

See [`hardware/BOM.csv`](hardware/BOM.csv) for the complete build list.

## Repository Layout

```text
panel-warden/
├── README.md
├── schematic/
│   ├── panel-warden.kicad_pro
│   ├── panel-warden.kicad_sch
│   └── panel-warden.kicad_pcb
├── firmware/
│   ├── CMakeLists.txt
│   ├── sdkconfig
│   ├── include/
│   └── src/
├── hardware/
│   └── BOM.csv
├── docs/
│   ├── assembly-guide.md
│   ├── api-reference.md
│   └── sample-log.csv
└── scripts/
    ├── analyze_panel_log.py
    └── generate_fixture_log.py
```

## Build and Run Firmware Simulation

This repository includes a host-buildable C simulation of the classification pipeline.

```bash
cd firmware
cmake -S . -B build
cmake --build build
./build/panel_warden_fw_sim
```

## Example Output

```text
MCC-A1          | LOOSE_LUG           | 76.4 | 0.86 |   87.2C |   42.3C |   20.3C |  18.0dB |  -53.0dBm | 36.0A | localized hotspot exceeds expected load correlation; re-torque and inspect lug oxidation
PV-COMB-2       | SURFACE_TRACKING    | 80.0 | 0.86 |   53.1C |   12.4C |    0.9C |  37.5dB |  -45.0dBm | 11.0A | humid enclosure with sustained discharge signature; clean/replace insulation surfaces
```

## Field Workflow

1. De-energize and lock out the target panel.
2. Mount Panel Warden inside the enclosure.
3. Aim the IR sensor at the primary termination area.
4. Place the magnetic contact pad against the inner wall near the monitored equipment.
5. Optionally clip the CT around the feeder of interest.
6. Close the enclosure and re-energize.
7. Review OLED status or connect via BLE/Wi-Fi.
8. Export the CSV log and correlate events with maintenance actions.

## Practical Build Notes

- Use matte black paint or Kapton masking around the thermal aperture to reduce reflections.
- Keep the RF detector input trace short and away from the buck-boost inductor.
- Mount the ultrasonic receiver on silicone grommets to reduce structure-borne false positives.
- For dusty panels, add a PTFE acoustic membrane over the ultrasonic opening.
- The IR sensor does not need absolute emissivity perfection for fault detection; trend analysis matters most.

## Limitations

- Not a safety-rated relay or trip device.
- Not a substitute for energized thermography procedures where those are required by code.
- RF and ultrasonic signatures depend on enclosure geometry.
- CT input is relative, not utility-grade metrology.

## Future Extensions

- tinyML anomaly model trained on site-specific panel baselines
- Modbus TCP bridge for plant historians
- LoRaWAN backhaul for remote pump cabinets
- optional relay output for remote warning light
- dual thermal arrays for wider field of view

## License

MIT
