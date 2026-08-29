# Chill Warden — Clamp-On Refrigeration and Heat-Pump Fault Sentinel

> A buildable open-hardware service instrument that clips onto a compressor rack, condensing unit, walk-in cooler, freezer, or residential heat-pump outdoor unit and turns temperature, current, vibration, humidity, and acoustics into actionable maintenance diagnoses.

## What It Is

**Chill Warden** is a field-deployable diagnostic node built around the **ESP32-S3-WROOM-1-N8**. It is designed for refrigeration techs, facility teams, and HVAC contractors who need something between a basic clamp meter and a full manifold-data-logger setup.

The device clamps to a system and observes:

1. **Suction-line temperature** using a copper-jaw digital clamp probe.
2. **Discharge-line temperature** using a high-temperature stainless strap probe.
3. **Ambient temperature and humidity** for condenser condition and icing context.
4. **Compressor current** from a split-core CT accessory.
5. **Cabinet / shell vibration** from a 6-axis IMU plus piezo contact disc.
6. **Fan / compressor audio** from an I2S MEMS microphone.
7. **Cycle timing** from current, vibration, and optional thermostat-call opto input.

Chill Warden then computes fault signatures and reports probable root causes over OLED, microSD, BLE, and Wi-Fi.

## Why This Device Matters

Refrigeration and heat-pump service work is often performed under time pressure with incomplete instrumentation. Many expensive failures show early symptoms that are easy to miss:

- high discharge temperature before compressor damage
- short cycling before contactor failure or nuisance lockout
- condenser airflow restriction before cooling capacity collapses
- low-charge symptoms before evaporator icing becomes obvious
- bearing wear before a fan or compressor seizes
- liquid slugging before reed valves or scroll sets are damaged

Chill Warden turns those hints into repeatable, timestamped evidence using a single compact node.

## Main SoC Choice

### ESP32-S3-WROOM-1-N8

The ESP32-S3 is a strong fit because it provides:

- dual-core MCU performance for acquisition, DSP, UI, and networking
- integrated Wi-Fi + BLE without a second radio MCU
- native support for I2C, SPI, I2S, ADC, GPIO, and deep sleep
- enough RAM and flash for feature buffers, web UI, and event logs
- robust OTA and wide ecosystem support

Chill Warden uses the ESP32-S3 as the **single application SoC**. Precision sensing is handled by external front ends and digital sensors.

## Functional Overview

1. The technician straps the suction and discharge probes to the refrigeration lines.
2. A split-core CT clips around the compressor feed to estimate running load and cycle state.
3. The enclosure magnetically attaches to the condensing-unit shell or nearby service panel.
4. An **ICM-42688-P** measures vibration, while a **piezo contact disc** listens for shell-borne knocks and slugging impulses.
5. An **ICS-43434** MEMS microphone measures fan / compressor acoustic energy and tonal roughness.
6. Dual **TMP117** sensors track suction and discharge temperature.
7. A **SHT45** provides ambient temperature / RH for condenser-split and icing context.
8. An **ADS122C04** digitizes conditioned analog paths from the piezo and CT accessory.
9. Firmware classifies operating state and recommends the next technician action.

## Target Use Cases

- walk-in cooler and freezer diagnostics
- residential mini-split and heat-pump service
- rooftop-unit preventive maintenance
- beverage / florist / grocery cold room monitoring
- refrigerated truck service-bay screening
- light commercial condensing-unit asset surveys

## Fault Classes

Chill Warden distinguishes these on-device:

- **NORMAL** — temperatures, current, and mechanical activity are inside expected bounds
- **AIRFLOW_RESTRICTED** — condenser split and discharge rise indicate blocked fins, failed fan, or recirculation
- **LOW_CHARGE_SUSPECT** — high superheat proxy with weak condenser split and light load
- **BEARING_WEAR** — elevated vibration and broadband shell acoustics
- **LIQUID_SLUGGING** — impulsive piezo events with abnormally low superheat proxy
- **SHORT_CYCLING** — repeated starts with very short off-time or run-time
- **DEFROST_ISSUE** — freezer / heat-pump operating pattern suggests poor or absent defrost behavior

## Block Diagram

```text
┌────────────────────────────── Chill Warden ───────────────────────────────┐
│                                                                           │
│  USB-C / Li-ion power                                                     │
│      │                                                                    │
│      ├─► MCP73831 charger ─► Li-ion ─► TPS63070 SYS_3V3 ─► AP7333 ANA_3V3 │
│      └─► MAX17048 fuel gauge                                              │
│                                                                           │
│  Suction clamp TMP117 ─┐                                                  │
│  Discharge clamp TMP117├─► I2C                                            │
│  Ambient SHT45 ────────┤                                                  │
│  ICM-42688-P IMU ──────┤                                                  │
│  SH1106 OLED ──────────┤                                                  │
│  ADS122C04 24-bit ADC ─┘                                                  │
│          ▲                 ICS-43434 I2S mic                              │
│          │                 ▲                                              │
│  Piezo disc ─► OPA1652 ────┤                                              │
│  CT accessory ─► burden + rectifier ─► ADS122C04                          │
│                                                                           │
│                            ESP32-S3-WROOM-1-N8                            │
│                         ┌─────────────┬─────────────┐                      │
│                         │ SPI         │ GPIO/UART   │                      │
│                         ▼             ▼             │                      │
│                      microSD       LED/buzzer       │                      │
│                                   buttons/service   │                      │
│                                                                           │
│                BLE + Wi-Fi telemetry / local OLED / CSV logging           │
└───────────────────────────────────────────────────────────────────────────┘
```

## Full Schematic Design

### Major ICs and Subsystems

| Ref | Part | Function |
|-----|------|----------|
| U1 | ESP32-S3-WROOM-1-N8 | Main SoC, acquisition, DSP, classification, UI, wireless |
| U2 | ADS122C04IPWR | 4-channel 24-bit delta-sigma ADC for piezo + CT + spare analog |
| U3 | TMP117AIDRVR | Suction-line digital temperature sensor |
| U4 | TMP117AIDRVR | Discharge-line digital temperature sensor |
| U5 | SHT45-AD1B-R2 | Ambient humidity and temperature |
| U6 | ICM-42688-P | 6-axis IMU for vibration and orientation |
| U7 | ICS-43434 | Digital MEMS microphone |
| U8 | OPA1652AIDR | Low-noise dual op-amp for piezo buffer and active band-pass |
| U9 | MCP73831T-2ATI/OT | Li-ion charger |
| U10 | TPS63070RNMR | 3.3 V buck-boost regulator |
| U11 | AP7333-33SAG-7 | Analog 3.3 V low-noise LDO |
| U12 | MAX17048G+T10 | Fuel gauge |
| DS1 | SH1106 128×64 OLED | Local UI |
| J1 | microSD socket | Log storage |
| J2 | 3.5 mm TRS jack | Split-core CT accessory input |
| J3 | USB-C receptacle | Power and service |
| J4 | JST-PH-2 | Li-ion battery connector |
| K1 | PC817 or LTV-817 | Optional thermostat / call optoisolated input |
| X1 | 27 mm piezo disc | Shell-borne knock / slugging pickup |
| B1 | 2000 mAh Li-ion pouch | Portable power |

### Sensing Topology

#### 1. Refrigerant Line Temperature Path

- **TMP117 #1** is mounted in a copper clamp jaw on the suction line.
- **TMP117 #2** is mounted in a stainless spring strap on the discharge line.
- Both probes connect over I2C and are sampled once per second.
- Firmware derives:
  - suction temperature
  - discharge temperature
  - discharge rise above ambient
  - condenser split proxy
  - simple superheat proxy when line-map presets are selected

#### 2. Ambient Context Path

The **SHT45** measures condenser-air temperature and RH. Firmware uses it to derive:

- ambient dry-bulb
- dewpoint
- icing propensity during low-ambient operation
- expected condenser approach window

#### 3. Vibration + Slugging Path

The **ICM-42688-P** provides accelerometer RMS and band-limited vibration features. A bonded **piezo contact disc** is buffered by the **OPA1652** and filtered into the **ADS122C04**. This combination catches:

- fan imbalance
- compressor shell roughness
- bearing wear
- liquid hammer / slugging impulses
- relay chatter

#### 4. Compressor Current Path

A service accessory split-core CT enters at **J2**. The path is:

`CT -> burden resistor -> full-wave precision rectifier -> RC averaging -> ADS122C04`

The system does not replace a revenue-grade power analyzer. Its purpose is practical cycle-state detection and approximate relative load tracking.

#### 5. Acoustic Path

The **ICS-43434** microphone captures condenser fan and compressor sound over I2S. Firmware extracts:

- broadband dBA proxy
- roughness / variance
- blade-pass harmonic strength
- impulsiveness coincidence with piezo events

### Buses and Interconnects

- **I2C0**: ADS122C04, TMP117 suction, TMP117 discharge, SHT45, ICM-42688-P, MAX17048, SH1106 OLED
- **I2S0**: ICS-43434 digital microphone
- **SPI2**: microSD
- **UART0**: service console over USB-UART bridge
- **GPIO**: LED, buzzer, buttons, charger status, thermostat call input, magnet switch
- **ADC1**: battery divider backup sense

## ESP32-S3 Pin Assignment

| Pin | Function |
|-----|----------|
| GPIO1 | I2C_SCL |
| GPIO2 | I2C_SDA |
| GPIO4 | I2S_WS |
| GPIO5 | I2S_SCK |
| GPIO6 | I2S_SD |
| GPIO7 | OLED_RESET |
| GPIO8 | ALERT_LED |
| GPIO9 | BUZZER_EN |
| GPIO10 | BUTTON_MODE |
| GPIO11 | BUTTON_ACK |
| GPIO12 | SD_CS |
| GPIO13 | SPI_MOSI |
| GPIO14 | SPI_MISO |
| GPIO15 | SPI_SCK |
| GPIO16 | CHARGER_STAT |
| GPIO17 | THERMOSTAT_CALL_IN |
| GPIO18 | CT_PRESENT_DETECT |
| GPIO19 | BATTERY_ADC |
| GPIO20 | ADC_DRDY |
| GPIO21 | MAGNET_REED |
| GPIO35 | UART_TX |
| GPIO36 | UART_RX |
| EN | Reset |

## Power Architecture

### Rails

- **VBUS_5V** from USB-C service/power input
- **VBAT** from single-cell Li-ion pack
- **SYS_3V3** from TPS63070 buck-boost
- **ANA_3V3** from AP7333 low-noise analog rail

### Power Tree

1. USB-C 5 V enters through ESD and resettable fuse protection.
2. **MCP73831** charges the Li-ion cell at 500 mA.
3. The battery and USB source feed the **TPS63070** to maintain 3.3 V system power.
4. **AP7333** post-regulates the analog front end and ADC.
5. **MAX17048** reports state of charge to the ESP32-S3.

### Typical Consumption

| Mode | Current |
|------|---------|
| Deep sleep, wake on timer | ~180 µA |
| Logging only, no Wi-Fi | ~52 mA |
| Logging + OLED + BLE | ~78 mA |
| Wi-Fi upload burst | 160-240 mA |

## Firmware Architecture

The included firmware is a **portable C simulation build** that exercises the classification logic on host Linux while mirroring the intended embedded module split.

### Modules

- `sensors.c` — loads representative frames from field scenarios
- `classifier.c` — computes fault classes, risk scores, and maintenance advice
- `ui.c` — prints a technician-oriented report
- `main.c` — end-to-end demo entry point

### Classification Heuristics

The current C reference firmware uses deterministic heuristics to keep the design auditable:

- low cycle period + repeated starts ⇒ **SHORT_CYCLING**
- high condenser split + high discharge rise + loud airflow ⇒ **AIRFLOW_RESTRICTED**
- high superheat proxy + low current + weak split ⇒ **LOW_CHARGE_SUSPECT**
- elevated vibration + shell acoustic energy ⇒ **BEARING_WEAR**
- high impulsive piezo energy + near-zero superheat ⇒ **LIQUID_SLUGGING**
- low ambient / high humidity / long run with cold suction ⇒ **DEFROST_ISSUE**

The design can later be upgraded to an ML classifier without changing the hardware.

## Bill of Materials

A complete BOM is provided in `hardware/BOM.csv`.

### Cost Snapshot

| Subsystem | Approx. cost (USD) |
|-----------|--------------------|
| ESP32-S3 module | 5.80 |
| Temperature probes + ambient sensor | 8.40 |
| IMU + mic | 4.60 |
| ADC + analog front end | 5.70 |
| Power path + fuel gauge | 5.40 |
| OLED + microSD + UI | 5.20 |
| Enclosure, clamps, piezo, connectors | 11.00 |
| **Estimated total** | **~46-52** |

## Mechanical Concept

- magnet-backed main enclosure sized roughly 94 mm × 62 mm × 24 mm
- detachable suction clamp with insulated copper jaw
- stainless band clamp for discharge line
- removable CT accessory lead
- silicone-isolated piezo mounting foot for shell contact

## Build and Verification

### Host Simulation Build

```bash
cd firmware
cmake -S . -B build
cmake --build build
./build/chill_warden_fw_sim
```

### Example Output

The simulation prints a compact diagnostic report for several field scenarios and should be used as a firmware sanity check before porting the code into ESP-IDF tasks.

## Calibration Outline

1. **Temperature probes** — compare both TMP117 clamps against a calibrated pipe simulator at two temperatures.
2. **CT path** — use a known-current source or reference compressor load to map CT output to RMS current proxy.
3. **Vibration path** — verify ICM RMS against a shaker or known motor reference.
4. **Piezo path** — tap-test shell coupling and adjust slugging threshold for the chosen adhesive pad.
5. **Acoustic path** — confirm fan blade-pass tone capture and SPL proxy repeatability.

## Example Service Workflow

1. Attach suction probe, discharge probe, and CT.
2. Stick the enclosure to the outdoor unit.
3. Start logging for 10-15 minutes.
4. Review live OLED diagnosis.
5. Export CSV to the helper script.
6. Use the device recommendation to guide manifold hookup, coil cleaning, or mechanical inspection.

## Repository Contents

```text
chill-warden/
├── README.md
├── schematic/
│   ├── chill-warden.kicad_pro
│   ├── chill-warden.kicad_sch
│   └── chill-warden.kicad_pcb
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
    ├── generate_fixture_log.py
    └── analyze_fixture_log.py
```

## unified TREE Role

Chill Warden is primarily an **Observe** node with optional **Coordinate** behavior when fleet-level refrigeration assets stream alerts into a larger maintenance deployment.

## Future Extensions

- optional discharge pressure transducer accessory
- Modbus bridge for packaged rooftop units
- fan-RPM optical accessory
- per-system refrigerant library and target envelopes
- technician phone app with QR-coded asset association
