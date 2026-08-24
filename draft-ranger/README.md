# Draft Ranger — Window, Door, and Envelope Air-Leak Mapper

> A handheld building-envelope instrument that measures infiltration drafts, surface condensation risk, seal whistle signatures, and frame thermal losses around windows, doors, skylights, and access panels.

## What It Is

**Draft Ranger** is a portable diagnostics node built around the **ESP32-S3-WROOM-1-N8**. It helps homeowners, weatherization crews, HVAC technicians, facilities teams, and energy auditors quickly answer four practical questions:

1. **Where is outside air leaking in?**
2. **How fast is that leakage under the current pressure differential?**
3. **Is the nearby surface cold enough to condense indoor moisture and grow mold?**
4. **Is the leak dominated by loose weatherstripping, a pressure imbalance, or a warped sash / door frame?**

Instead of relying on incense smoke or expensive blower-door-only workflows, Draft Ranger combines a compact differential-pressure sensor, a hot-wire crack probe, dual surface-temperature pads, ambient humidity sensing, and an ultrasonic-capable microphone to quantify leak severity and classify likely failure modes.

## Why This Device Matters

Air leakage around windows and doors causes:

- higher heating and cooling bills
- occupant discomfort from cold drafts
- condensation on frames and glazing
- hidden mold growth in trim cavities
- poor room balancing when HVAC pressure is wrong

Professional envelope diagnostics are often either too qualitative or too expensive for routine checks. Draft Ranger is designed as a **field-buildable, lower-cost, repeatable leakage mapper** that can be used during retrofits, commissioning, renter inspections, and maintenance rounds.

## Main SoC Choice

### ESP32-S3-WROOM-1-N8

The ESP32-S3 is a strong fit because it provides:

- Wi-Fi for direct upload of leak maps and reports
- BLE for mobile commissioning and live graphs
- I2S support for the MEMS acoustic sensor
- enough CPU and RAM for DSP, dew-point math, and local UI
- native USB for future high-rate debug and calibration workflows

## Functional Overview

1. A narrow **hot-wire probe** is inserted near a suspect crack or gasket.
2. Two silicone-tipped **surface temperature paddles** contact the frame and nearby glazing/wall.
3. The **SDP810** differential-pressure sensor measures pressure drop between indoor air and the probe tip.
4. The **CTA hot-wire bridge** measures local air velocity in the leak path.
5. An **ICS-43434 I2S microphone** listens for whistle tones associated with turbulent gaps.
6. **SHT45** measures indoor air temperature and humidity for dew-point and comfort calculations.
7. Firmware computes:
   - leak velocity estimate
   - leak severity score
   - condensation margin
   - whistle intensity and dominant tone
   - probable fault class
8. Results are shown on the OLED, logged to microSD, and exported over BLE/Wi-Fi.

## Block Diagram

```text
           ┌──────────────────────── Draft Ranger ────────────────────────┐
           │                                                              │
Indoor tap ─┐                                                             │
            ├─► SDP810 differential pressure sensor ── I2C ─┐             │
Probe tip ──┘                                                │             │
                                                             │             │
Crack probe hot wire ─► OPA388 CTA bridge ─► ADS1115 ADC ───┤             │
                                                             │             │
Frame pad ─► TMP117 #1 ──────────────────────────────────────┤             │
Glass/wall pad ─► TMP117 #2 ─────────────────────────────────┤             │
Ambient T/RH ─► SHT45 ───────────────────────────────────────┤             │
Whistle acoustics ─► ICS-43434 I2S MEMS mic ────────────────┤             │
RTC ─► DS3231 ───────────────────────────────────────────────┤             │
                                                             ▼             │
                                                    ESP32-S3-WROOM-1-N8   │
                                                  - draft DSP fusion       │
                                                  - dew-point computation  │
                                                  - fault classification   │
                                                  - OLED UI / logger       │
                                                  - BLE + Wi-Fi telemetry  │
                                                             │             │
                   OLED ── I2C ──────────────────────────────┤             │
               microSD ── SPI ───────────────────────────────┤             │
          buttons/buzzer/RGB ── GPIO ────────────────────────┤             │
 USB-C / Li-ion ─► charger + 3V3 buck + sensor rail switch ─┘             │
           └───────────────────────────────────────────────────────────────┘
```

## Full Schematic Design

### Major ICs and Subsystems

| Ref | Part | Function |
|-----|------|----------|
| U1 | ESP32-S3-WROOM-1-N8 | Main SoC, DSP, UI, BLE/Wi-Fi |
| U2 | SDP810-500Pa | Differential pressure sensor across leak path |
| U3 | ADS1115IDGST | 16-bit ADC for hot-wire bridge measurement |
| U4 | OPA2388AIDR | Chopper op-amp pair for constant-temperature anemometer bridge |
| U5 | TMP117AIDRVR | Frame contact temperature pad |
| U6 | TMP117AIDRVR | Glass / wall contact temperature pad |
| U7 | SHT45 | Ambient temperature + RH |
| U8 | ICS-43434 | Digital MEMS microphone for whistle detection |
| U9 | DS3231SN | RTC with backup cell support |
| DS1 | SH1106 128x64 OLED | Local display |
| J1 | microSD socket | CSV and binary logging |
| U10 | MCP73831T | Single-cell Li-ion charger |
| U11 | TPS63031 | 3.3 V buck-boost regulator |
| U12 | TPS22916 | Switched sensor rail |
| B1 | 18650 / flat LiPo | Portable field battery |
| U13 | TLV3691 | Hardware brownout / overrun comparator for hot-wire bridge shutdown |
| Q1 | AO3400A | Buzzer low-side driver |
| Q2 | AO3400A | Probe heater enable / hard cutoff |
| LS1 | Magnetic buzzer | Alarm / UI feedback |

### Pressure Sensing Path

- **SDP810** ports connect to:
  - **P+**: probe tip tubing inserted near crack
  - **P-**: room-reference static port on handle body
- The sensor reports ±500 Pa differential pressure over I2C.
- A short hydrophobic PTFE vent is recommended on the room-reference port to reduce dust ingress.

### Hot-Wire Leak Probe

The crack probe uses a fine platinum-coated tungsten wire operated in a **constant-temperature anemometer (CTA)** loop.

- One OPA2388 channel regulates bridge drive to maintain wire overheat ratio
- The second channel buffers bridge imbalance into ADS1115
- The ESP32-S3 reads bridge voltage and converts it to **velocity proxy** using King’s-law coefficients stored in NVS
- Q2 allows hard power cutoff to the probe during standby or fault conditions

### Thermal / Condensation Sensing

- **TMP117 #1** presses against the frame, mullion, or door jamb
- **TMP117 #2** presses against adjacent glazing, wallboard, or metal threshold
- **SHT45** measures indoor air conditions for dew-point math
- Firmware computes:
  - dew point
  - minimum surface margin to dew point
  - thermal gradient between frame and nearby surface
  - mold-risk persistence estimate

### Acoustic Leak Sensing

- **ICS-43434** is mounted behind a narrow slot at the probe tip
- Sampled via I2S at 16 kHz
- DSP evaluates RMS, crest factor, and Goertzel bins from 400 Hz to 6 kHz
- Strong narrowband peaks indicate classic weatherstrip whistle or poorly seated sash compression

### Data / UI / Storage

- **I2C bus**: SDP810, ADS1115, TMP117 #1, TMP117 #2, SHT45, DS3231, OLED
- **I2S bus**: ICS-43434 microphone
- **SPI bus**: microSD
- **UART0**: manufacturing and calibration console

## ESP32-S3 Pin Assignment

| ESP32-S3 Pin | Function |
|--------------|----------|
| GPIO1 | UART0_TX debug |
| GPIO2 | UART0_RX debug |
| GPIO4 | BUZZER_EN |
| GPIO5 | PROBE_HEATER_EN |
| GPIO6 | RGB_LED_R |
| GPIO7 | RGB_LED_G |
| GPIO8 | RGB_LED_B |
| GPIO9 | BUTTON_MODE |
| GPIO10 | BUTTON_MARK |
| GPIO11 | BUTTON_LOG |
| GPIO12 | I2C_SDA |
| GPIO13 | I2C_SCL |
| GPIO14 | OLED_RESET |
| GPIO15 | MIC_BCLK |
| GPIO16 | MIC_WS |
| GPIO17 | MIC_DATA |
| GPIO18 | SD_CS |
| GPIO19 | SPI_MOSI |
| GPIO20 | SPI_SCK |
| GPIO21 | SPI_MISO |
| GPIO35 | BATTERY_ADC |
| GPIO36 | SENSOR_RAIL_EN |
| GPIO37 | HOTWIRE_FAULT_IN |
| GPIO38 | USB_DETECT / charge present |
| GPIO39 | WAKE / service pad |

> Final PCB layout should validate ESP32-S3 strapping and USB/JTAG pin interactions for the selected module revision.

## Measurement Modes

### 1. Spot Leak Mode
Probe is held at one point for 2-5 seconds to quantify local draft severity.

### 2. Sweep Mode
User drags the probe across an edge; firmware records peak leak values across distance marks.

### 3. Condensation Scan
Thermal pads are used without the hot-wire probe to find surfaces below safe dew-point margin.

### 4. Pressure Balance Check
At supply grilles / closed rooms, the sensor can compare under-door pressure imbalance and resulting leakage tendency.

## Fault Classes

- **SEAL_OK** — negligible leak and healthy condensation margin
- **MINOR_WEATHERSTRIP_GAP** — low pressure, low-to-medium velocity, mild whistle
- **PRESSURE_DRIVEN_DRAFT** — elevated pressure differential and broad leak response
- **WARPED_FRAME_OR_SASH** — high velocity localized with stable narrow whistle tone
- **CONDENSATION_RISK** — acceptable airflow but poor dew-point margin
- **CRITICAL_COMBINED** — high leak severity plus negative condensation margin

## DSP / Physics Chain

```text
pressure Pa + hot-wire bridge + ambient RH/T + 2 surface temps + mic samples
    -> pressure-to-velocity estimate
    -> hot-wire King's-law refinement
    -> dew point / RH / condensation margin
    -> FFT-lite / Goertzel whistle score
    -> weighted severity fusion
    -> fault classification with hysteresis
```

### Derived Metrics

- **draft_velocity_mps** — fused from differential pressure and hot-wire bridge
- **leak_severity_0_100** — normalized comfort + energy-loss score
- **dew_point_c** — Magnus approximation from ambient T/RH
- **condensation_margin_c** — coldest surface minus dew point
- **whistle_score** — tonal prominence from Goertzel bins
- **seal_health_0_100** — inverse of persistent leakage and whistle severity

## Power Architecture

### Input Sources

- USB-C 5 V for charging and bench use
- 1-cell Li-ion / LiPo for handheld operation

### Rails

- **VBAT**: raw cell voltage
- **VSYS_3V3**: TPS63031 regulated main rail
- **SENS_3V3**: switched sensor rail via TPS22916
- **PROBE_HEAT**: gated bridge excitation for hot-wire element

### Estimated Consumption

| Mode | Current |
|------|---------|
| Deep sleep | ~180 µA |
| UI idle, sensors on | ~42 mA |
| Sweep logging with OLED + SD + mic | ~92 mA |
| Probe heating active | ~135 mA peak |

Expected field runtime with a 2500 mAh cell: **~18 hours intermittent scanning** or **~9 hours continuous sweep logging**.

## Mechanical Architecture

- 3D-printed ABS/ASA handheld body
- stainless steel 3 mm probe tube with PTFE tubing pair
- spring-loaded dual thermal pads with silicone caps
- front acoustic slit with replaceable dust mesh
- rear USB-C and tripod-mount insert for repeatable scans

## Bill of Materials Summary

See [`hardware/BOM.csv`](hardware/BOM.csv) for the full list.

Approximate prototype electronics cost: **~$53-60 USD** depending on display, battery choice, and enclosure fabrication.

## Firmware Layout

```text
firmware/
├── CMakeLists.txt
├── include/
│   ├── acoustics.h
│   ├── classifier.h
│   ├── draft_ranger.h
│   ├── psychrometrics.h
│   ├── sensors.h
│   └── ui.h
└── src/
    ├── acoustics.c
    ├── classifier.c
    ├── main.c
    ├── psychrometrics.c
    ├── sensors.c
    └── ui.c
```

The checked-in firmware is a **host-buildable simulation** of the measurement stack so the algorithms can be exercised before hardware exists.

## Build

```bash
cd firmware
cmake -S . -B build
cmake --build build
./build/draft_ranger_fw_sim
```

## Helper Scripts

- `scripts/analyze_draft_log.py` — summarize CSV leakage scans
- `scripts/generate_fixture_log.py` — generate a synthetic field log for testing dashboards and post-processing

## Calibration Workflow

1. Zero the SDP810 with both ports open to room air.
2. Run the hot-wire zero with probe heater enabled but no airflow.
3. Sweep the probe across a reference slit at known fan pressure.
4. Fit King’s-law coefficients `A` and `B` in the helper script.
5. Touch both thermal pads to an isothermal plate to remove sensor offsets.
6. Save coefficients to NVS.

## Example Use Cases

- apartment move-in inspection
- blower-door guided retrofit work
- historic building sash restoration
- cold-room and walk-in freezer door maintenance
- hotel room comfort complaints
- mold-risk triage around skylights and aluminum frames

## Repository Contents

- `schematic/` — conceptual KiCad project files
- `firmware/` — C firmware simulation and build system
- `hardware/` — bill of materials
- `docs/` — assembly and API documentation
- `scripts/` — post-processing and data-generation tools

## Safety Notes

- The hot-wire probe becomes warm during operation; keep away from flammable lint and soft plastics.
- Do not use the instrument inside energized switchgear or explosive atmospheres.
- Pressure ports should be filtered when probing dusty cavities.
- The device estimates leak severity; it is not a code-certified blower-door replacement.
