# Pasture Pulse — Solar Electric-Fence Pulse Analyzer Node

> A solar-powered fence-line sentinel that clips onto an electric livestock fence, measures each high-voltage pulse without a galvanic connection, estimates vegetation loading and fault severity, and reports fence health over LoRaWAN, BLE, and local OLED.

## What It Is

**Pasture Pulse** is a buildable open-hardware node for ranches, rotational grazing systems, wildlife exclusion fences, and remote paddocks. It is built around the **STM32WL55JC**, which combines an ARM Cortex-M4/M0+ microcontroller pair with an integrated sub-GHz LoRa radio.

The device mounts to a fence post near the energizer, a gate, or a remote line segment. A capacitive pickup plate and optional split-core pulse current transformer observe the fence pulse waveform without becoming part of the high-energy circuit. Firmware converts those measurements into practical maintenance findings:

- pulse peak voltage
- pulse rise time and decay time
- estimated pulse energy trend
- vegetation loading score
- wet-insulator leakage score
- open-gate / broken-wire suspicion
- energizer battery weakness suspicion
- lightning / surge disturbance events

The node is meant to answer a simple but important question for land managers:

**Is the fence still strong enough, and if not, where should I go first?**

## Why This Device Matters

Electric fences fail quietly. Grass growth, wet insulators, corroded splices, weak solar chargers, and half-latched gates can reduce fence effectiveness long before an animal escape becomes obvious. Existing testers are often handheld spot-check tools; they do not provide continuous trend visibility across multiple paddocks.

Pasture Pulse turns a fence into an observable infrastructure network by combining:

1. **non-contact pulse voltage sensing**
2. **pulse-current trend sensing**
3. **solar-powered unattended operation**
4. **LoRaWAN telemetry for long pasture runs**
5. **local triage logic that works even when backhaul is absent**

## Primary Use Cases

- rotational grazing with portable electric netting
- perimeter predator deterrent fences
- solar fence energizer monitoring
- remote gate and splice fault screening
- wildlife exclusion around crops or orchards
- equine pasture fence confidence monitoring
- educational ag-tech / precision livestock projects

## Main SoC Choice

### STM32WL55JC

The STM32WL55JC is a particularly good fit because it provides:

- low-power operation for always-on remote nodes
- integrated **LoRa** radio without a second communications SoC
- strong timer / ADC / DMA support for pulse capture
- plenty of GPIO for sensors, OLED, and alert outputs
- mature development ecosystem and broad module availability

Pasture Pulse uses the STM32WL55JC as the **single application and radio SoC**. Analog front-end protection, energy harvesting, and sensor conditioning are handled with dedicated external circuitry.

## Functional Overview

1. A **stainless capacitive pickup plate** is mounted 8-20 mm from the live fence wire.
2. A very high impedance divider and TVS-protected clamp network scale the pulse into the safe input range of an **ADS131M02** simultaneous-sampling ADC.
3. An optional **clip-on pulse current transformer** around the fence lead measures relative current delivery during each pulse.
4. A **TMP117** and **SHT45** provide temperature, humidity, and dewpoint context used to distinguish vegetation loading from wet-hardware leakage.
5. A **reed switch** or hall accessory can be fitted at a nearby gate to correlate pulse collapse with gate-open events.
6. The **BQ25570** harvests energy from a small solar panel into a LiFePO4 cell and powers the node through day/night cycles.
7. The STM32WL55JC wakes at pulse interrupts, captures each waveform, computes health metrics, logs summaries to FRAM, updates the OLED, and periodically transmits over LoRaWAN.

## Block Diagram

```text
┌────────────────────────────── Pasture Pulse ──────────────────────────────┐
│                                                                            │
│  6 V Solar ─► BQ25570 energy harvester ─► LiFePO4 ─► 3V3_SYS               │
│                              │                     └─► TPS22916 load switch │
│                              └─► battery telemetry                          │
│                                                                            │
│  Fence wire ──||── capacitive pickup plate ─► HV divider ─► clamp/RC ─┐    │
│                                                                     ┌──▼──┐ │
│  Optional pulse CT ─► burden ─► diff amp ─► anti-alias filter ─────►│ADS1 │ │
│                                                                     │31M02│ │
│  TMP117 ─┐                                                          └──┬──┘ │
│  SHT45 ──┼─► I2C                                                        │    │
│  MB85RC ─┤                                                              │    │
│  OLED ───┘                                                              │    │
│                                                                            │
│                      STM32WL55JC (MCU + LoRa radio)                       │
│                ┌───────────────┬────────────────┬────────────────┐         │
│                │ SPI           │ GPIO           │ Sub-GHz Radio   │         │
│                │               │                │                │         │
│                ▼               ▼                ▼                │         │
│             ADS131M02      buzzer/LED       LoRaWAN uplink       │         │
│                              buttons         to gateway           │         │
│                                                                            │
│                    Local OLED + FRAM log + remote health alerts            │
└────────────────────────────────────────────────────────────────────────────┘
```

## Full Schematic Design

### Major ICs and Subsystems

| Ref | Part | Function |
|-----|------|----------|
| U1 | STM32WL55JCI6 | Main SoC, pulse capture, analytics, LoRaWAN radio |
| U2 | ADS131M02IPBS | Dual-channel 24-bit simultaneous ADC for voltage and current pulse capture |
| U3 | OPA2192IDR | Dual precision rail-to-rail op-amp for CT conditioning and virtual mid-rail buffering |
| U4 | TMP117AIDRVR | Ambient board / enclosure temperature reference |
| U5 | SHT45-AD1B-R2 | Ambient RH + temperature for dew/leakage discrimination |
| U6 | MB85RC256V | 256-kbit I2C FRAM for non-volatile event logging |
| U7 | BQ25570RGRR | Solar energy harvester + battery charger + buck converter |
| U8 | TPS22916CYFPR | Switched load output for OLED and accessory rail power saving |
| U9 | TLV3691DBVR | Nano-power comparator generating pulse wake interrupt |
| U10 | MAX17048G+T10 | Optional battery fuel gauge for LiFePO4 state-of-charge estimation |
| D1 | SMBJ58A | Primary surge clamp on capacitive pickup path |
| D2/D3 | PESD3V3L2BT | Secondary low-capacitance ESD clamps at ADC front end |
| DS1 | SSD1306 128×64 OLED | Local status display |
| J1 | Phoenix fence accessory terminal | Optional clip-on pulse CT input |
| J2 | JST-PH-2 | LiFePO4 battery connector |
| J3 | USB-C service connector | Bench power and firmware service |
| J4 | Solar JST-VH-2 | Solar panel input |
| K1 | Reed switch | Gate-open sensor |
| BZ1 | Magnetic buzzer | Local fence-down alert |
| B1 | 18650 LiFePO4 cell | Energy storage |

### High-Voltage Sense Path

The fence output can reach several kilovolts, but Pasture Pulse measures it **capacitively**, not by a direct ohmic connection to the line.

#### Pickup Structure

- A stainless or copper pickup plate is mounted parallel to the fence conductor.
- Coupling capacitance is typically 0.5-3 pF depending on spacing and geometry.
- The pickup plate feeds a **100 MΩ / 2 MΩ effective divider** implemented as a resistor string using multiple series HV resistors.
- A **SMBJ58A TVS** and series resistor limit surge energy.
- A **clamp-to-midrail network** and RC filter present a safe, bandwidth-limited signal to ADC channel AIN0.

This method yields a repeatable representation of pulse shape and relative amplitude while preserving galvanic isolation from the fence energizer.

### Pulse Current Sense Path

For stronger diagnostics, a clip-on pulse CT accessory can be placed around the fence lead or one branch conductor.

Path:

`Pulse CT -> burden resistor -> OPA2192 differential gain stage -> 1.65 V virtual mid-rail offset -> 30 kHz anti-alias filter -> ADS131M02 AIN1`

The current measurement is not meant to be legal metrology. It is used to infer:

- vegetation loading increase
- branch fault severity
- pulse energy collapse
- energizer weakness vs downstream leakage

### Environmental Context Path

- **TMP117** gives stable local temperature for analog compensation.
- **SHT45** provides RH, ambient temperature, and dewpoint.
- Firmware boosts wet-leakage suspicion when RH and dewpoint spread indicate condensation conditions.

### Event Logging and Display

- **MB85RC256V FRAM** stores pulse summaries, alarm transitions, and battery history without flash wear concerns.
- **SSD1306 OLED** shows present voltage, pulse interval, fault state, and last LoRa uplink age.
- **Buzzer + RGB LED** provide simple field indication: green/amber/red.

## Buses and Interconnects

- **SPI1**: ADS131M02 high-speed waveform capture
- **I2C1**: TMP117, SHT45, MB85RC256V, MAX17048, SSD1306
- **GPIO / EXTI**: comparator wake interrupt, gate reed switch, buttons, buzzer, LED
- **UART2**: service console over USB-UART bridge pads
- **SUBGHZSPI / integrated radio**: LoRaWAN uplink
- **ADC internal**: harvester / battery monitor backup measurements

## STM32WL55JC Pin Assignment

| Pin | Function |
|-----|----------|
| PA0 | PULSE_WAKE_COMP |
| PA1 | GATE_REED_IN |
| PA2 | UART2_TX |
| PA3 | UART2_RX |
| PA4 | SPI1_NSS (ADS131M02 CS) |
| PA5 | SPI1_SCK |
| PA6 | SPI1_MISO |
| PA7 | SPI1_MOSI |
| PB3 | ADS_DRDY |
| PB4 | STATUS_LED_R |
| PB5 | STATUS_LED_G |
| PB8 | I2C1_SCL |
| PB9 | I2C1_SDA |
| PB12 | OLED_EN_LOADSW |
| PB13 | BUZZER_EN |
| PB14 | BUTTON_ACK |
| PB15 | BUTTON_MODE |
| PC0 | BATTERY_ADC_BACKUP |
| PC1 | SOLAR_ADC_BACKUP |
| PH3 | RF antenna network / integrated radio |

## Power Architecture

### Power Tree

1. **6 V / 1-3 W solar panel** feeds **BQ25570** through reverse-polarity protection.
2. BQ25570 charges a **single LiFePO4 cell** with undervoltage and cold-start support.
3. The harvester's buck output provides **3V3_SYS** for the STM32WL55JC and core peripherals.
4. **TPS22916** switches OLED and accessory power during low-battery operation.
5. Optional MAX17048 plus internal ADC channels report battery and solar conditions.

### Rails

- `SOLAR_IN` — 4.5-7.0 V panel input
- `VBAT_LFP` — 2.5-3.65 V LiFePO4 cell
- `3V3_SYS` — main regulated logic rail
- `3V3_SW` — switchable display/accessory rail
- `VREF_MID` — 1.65 V analog mid-rail for bipolar pulse-current capture

### Typical Current Consumption

| Mode | Current |
|------|---------|
| Ship mode / battery protected | <10 µA |
| Sleep waiting for pulse interrupt | ~38 µA |
| Pulse capture burst | ~6.5 mA for 20 ms |
| OLED active local inspection | ~14 mA |
| LoRaWAN uplink @ +14 dBm | 42-118 mA burst |

## Detection Logic

Pasture Pulse classifies fence health into these states:

- **FENCE_HEALTHY** — strong voltage, stable interval, low leakage indicators
- **VEGETATION_LOADING** — current trend high relative to voltage, long decay, humidity not fully explanatory
- **WET_INSULATOR_LEAKAGE** — moderate voltage depression correlated with high RH / condensation risk
- **OPEN_GATE_OR_BREAK** — abrupt current collapse with abnormal interval or gate input active
- **WEAK_ENERGIZER_BATTERY** — slow multi-hour drop in pulse peak plus low energizer supply proxy
- **SURGE_DISTURBANCE** — non-periodic large transient unlike the normal pulse cadence
- **FENCE_DOWN** — pulse absent for timeout interval or peak far below configured minimum

## Firmware Architecture

The included firmware is a **portable host-build C simulation** that mirrors the intended embedded split. It compiles and runs on Linux so the signal-processing and classification logic can be exercised in this repository without requiring vendor SDK installation.

### Modules

- `sensors.c` — representative pulse frames for six realistic field conditions
- `pulse_model.c` — feature extraction math and derived indices
- `classifier.c` — fence health scoring and fault decisions
- `ui.c` — text report output matching intended OLED/uplink summaries
- `main.c` — top-level demo harness

## Pulse Features Computed

For each pulse frame, firmware computes:

- peak voltage (kV equivalent after calibration)
- 10-90% rise time
- decay constant proxy
- pulse interval
- peak current proxy
- delivered energy index
- leakage ratio = current / voltage-scaled baseline
- humidity-weighted wetness factor
- confidence score per fault class

## Mechanical Design Notes

- Polycarbonate UV-stable enclosure with fence-post strap slots
- Internal PCB separated from pickup plate by insulated standoff wall
- Stainless pickup plate mounted behind replaceable dielectric window
- External antenna or PCB meander depending range requirement
- Bottom vent with ePTFE membrane for humidity sensor exposure
- Service USB kept behind gasketed cap

## Safety Notes

Pasture Pulse is designed to be **electrically isolated from the live fence conductor** during normal use. Even so:

- use only rated HV resistors in the pickup divider string
- maintain creepage around the pickup node and surge clamp area
- never connect the ADC or MCU ground directly to the fence wire
- install only on legally compliant fence energizers
- keep the service USB disconnected during storm conditions
- validate local electric-fence regulations before deployment

## Bring-Up Procedure

1. Assemble power system only and verify solar charging and 3V3 rail.
2. Program the STM32WL55JC through SWD or service header.
3. Confirm I2C enumeration of TMP117, SHT45, FRAM, and OLED.
4. Inject low-voltage bench pulses into the analog frontend using an isolated function generator.
5. Verify comparator wake interrupt and ADS131M02 captures.
6. Calibrate capacitive pickup scale factor using a known commercial fence tester.
7. Mount on a non-critical fence segment and compare readings across several pulse cycles.
8. Tune threshold tables for local vegetation and fence geometry.

## Repository Contents

```text
pasture-pulse/
├── README.md
├── schematic/
│   ├── pasture-pulse.kicad_pro
│   ├── pasture-pulse.kicad_sch
│   └── pasture-pulse.kicad_pcb
├── firmware/
│   ├── CMakeLists.txt
│   ├── sdkconfig
│   ├── include/
│   │   ├── pasture_pulse.h
│   │   ├── sensors.h
│   │   ├── pulse_model.h
│   │   ├── classifier.h
│   │   └── ui.h
│   └── src/
│       ├── main.c
│       ├── sensors.c
│       ├── pulse_model.c
│       ├── classifier.c
│       └── ui.c
├── hardware/
│   └── BOM.csv
├── docs/
│   ├── assembly-guide.md
│   ├── api-reference.md
│   └── sample-log.csv
└── scripts/
    ├── generate_fixture_log.py
    └── analyze_fixture_log.py
```

## Unified TREE Role

Pasture Pulse is primarily an **Observe** node with secondary **Coordinate** behavior:

- observes pulse health and fence environmental context
- coordinates maintenance response by identifying likely fault class and affected segment

## Future Extensions

- dual pickup plates for directional fault isolation between upstream and downstream segments
- mesh relay mode using multiple fence nodes
- gate latch microswitch accessory
- GNSS-tagged pasture audit mode
- acoustic arcing detector for insulator flashover characterization

## Build Status

The host simulation firmware is intended to compile with standard GCC/Clang using CMake. See `docs/assembly-guide.md` and `docs/api-reference.md` for build and integration details.
