# Sump Shepherd — Pit-Level, Pump-Current, and Flood-Risk Sentinel

> A retrofittable sump-pump monitor that watches pit water level, motor current, vibration, humidity, float state, and backup battery health to predict flooding, stuck floats, failed check valves, dry-run events, and short-cycling before a basement fills with water.

## What It Is

**Sump Shepherd** is a practical residential and light-commercial infrastructure node built around the **ESP32-S3-WROOM-1-N8**. It mounts above a sump pit lid, points a waterproof ultrasonic sensor downward, clips a current transformer around the pump mains lead, and adds a few non-invasive auxiliaries:

- **downward-looking ultrasonic pit level sensing**
- **non-contact pump current measurement**
- **contact vibration sensing on the discharge pipe or lid**
- **float switch state input**
- **ambient humidity / temperature sensing near the pit**
- **leak-rope / floor-water input**
- **backup battery voltage tracking**

That lets the device answer the questions that matter when storms hit:

- Is the water rising but the pump not starting?
- Is the pump running but not lowering the pit level?
- Is the pit short-cycling because a check valve is leaking back?
- Is the basement becoming humid enough to support mold even before a flood?
- Is the backup battery healthy enough for an outage?

## Why This Device Matters

Sump pump failures are common, expensive, and often silent until damage is already underway. The problem is usually not just one sensor missing; it is the lack of **context fusion**. Water level alone does not reveal whether a pump is healthy, and current alone does not show whether the pit is emptying.

Sump Shepherd fuses multiple low-cost measurements to detect:

- **no-start / dead pump**
- **jammed impeller / seized motor**
- **short cycling**
- **failed check valve backflow**
- **dry-run / intake starvation**
- **slow pumping due to partial blockage**
- **high humidity / condensation around the pit**
- **actual floor leak via wet rope sensor**

## Main SoC Choice

### ESP32-S3-WROOM-1-N8

The ESP32-S3 is a good fit because it provides:

- Wi-Fi for direct home-network telemetry and OTA updates
- BLE for phone commissioning in basements where keyboards are inconvenient
- enough RAM and CPU for feature extraction and local classification
- multiple SPI / I2C / UART peripherals for sensor integration
- low-cost module availability and strong open-source tooling

## Functional Overview

1. A **waterproof ultrasonic ranger** mounted on the pit lid measures distance to the water surface every few seconds.
2. A **split-core current transformer** around the pump hot lead estimates RMS motor current when the pump starts.
3. A **LIS2DW12 accelerometer** on the enclosure measures vibration during pump operation.
4. An **SHT45** tracks humidity and temperature near the pit to estimate condensation / mold risk.
5. A **reed or float input** confirms when the mechanical float switch has actuated.
6. A **leak rope input** detects actual floor wetness outside the pit.
7. The **ESP32-S3** fuses the signals into pump-cycle features and classifies system health.
8. Results are shown locally on an OLED, logged to microSD, and reported over BLE / Wi-Fi.

## Block Diagram

```text
                    ┌──────────── Sump Shepherd ────────────┐
                    │                                       │
Pit water surface ─►│ A02YYUW ultrasonic ── UART ─┐         │
Pump mains lead ─► CT clamp ─► burden + rectifier ├─ ADC    │
Discharge pipe ─► LIS2DW12 accel ─────── I2C ─────┤         │
Pit air ────────► SHT45 temp/RH ──────── I2C ─────┤         │
Float switch ───► opto-isolated GPIO ─────────────┤         │
Leak rope ──────► comparator / GPIO ──────────────┤         │
Battery divider ─► ADC ───────────────────────────┤         │
RTC / OLED / microSD ─ I2C / SPI ─────────────────▼         │
                    │           ESP32-S3-WROOM-1-N8         │
                    │      - cycle segmentation             │
                    │      - fault classification           │
                    │      - OLED UI                        │
                    │      - BLE / Wi-Fi telemetry          │
                    │      - SD logging                     │
                    │                                       │
12 V in / UPS ─► buck 3V3 / charger / ideal OR-ing          │
                    └───────────────────────────────────────┘
```

## Full Schematic Design

### Major ICs and Subsystems

| Ref | Part | Function |
|-----|------|----------|
| U1 | ESP32-S3-WROOM-1-N8 | Main SoC, UI, telemetry, local analytics |
| U2 | A02YYUW waterproof ultrasonic sensor | Pit water distance measurement |
| U3 | LIS2DW12TR | Pump vibration / lid shock sensing |
| U4 | SHT45 | Ambient temperature + humidity |
| U5 | DS3231SN | RTC with coin-cell backup |
| U6 | SH1106 128×64 OLED | Local status display |
| U7 | microSD socket | Event and trend logging |
| U8 | LMV358 | Current-sense envelope / conditioning front end |
| U9 | TLV3691 | Leak-rope / wet-floor comparator |
| U10 | TPS62162 | 12 V / UPS to 3.3 V buck regulator |
| U11 | MCP73831 | Single-cell backup charger |
| U12 | TPS2113A | Ideal power-path OR between adapter and battery rail |
| T1 | SCT-013-030 or equivalent CT | Non-invasive pump current sensing |
| J1 | Float switch terminal | Dry contact or external reed float input |
| J2 | Leak rope terminal | Floor wetness detection |
| B1 | 18650 Li-ion cell | Backup power for telemetry during outage |
| LS1 | Magnetic buzzer | Local alarm output |

### Sensor Interfaces

#### 1. Ultrasonic Level Channel

The **A02YYUW** is mounted through the pit lid, aimed vertically down.

- Power: 3.3 V rail
- Interface: UART at 9600 baud to `U1.GPIO17/GPIO18`
- Typical range used: 50 mm to 1200 mm
- Protective gasket around the lid penetration to keep condensation off the PCB

The firmware converts distance-to-surface into:

- pit level height
- rise rate (mm/min)
- drawdown rate during pumping
- cycle depth and recovery time

#### 2. Pump Current Channel

A **split-core CT** around the pump hot conductor feeds a burden resistor and precision rectifier / envelope stage using **LMV358**.

- CT secondary burden sized for ~0–15 A primary equivalent
- RC envelope output sampled by ESP32 ADC at 200 Hz during a pump cycle
- optional raw AC node exposed on a test pad for validation

Derived features:

- current RMS proxy
- start surge magnitude
- steady-state current band
- run-time energy proxy
- current without drawdown fault signature

#### 3. Vibration Channel

The **LIS2DW12** is mounted near the cable gland edge to pick up pump and pipe vibration.

- I2C shared bus
- 100 Hz normal monitoring, 400 Hz during active cycle capture
- vibration RMS and shock count support jam / cavitation / lid impact detection

#### 4. Humidity and Temperature

The **SHT45** measures pit-air conditions.

Used for:

- mold-risk warning
- enclosure condensation warning
- environmental correlation during heavy rain events

#### 5. Float and Leak Inputs

- **Float input** is opto-isolated so it can interface to noisy legacy float-switch wiring safely.
- **Leak rope input** uses a high-value resistive sense plus **TLV3691** threshold comparator to detect actual floor wetness.

### Communications and Storage

- **I2C**: LIS2DW12, SHT45, DS3231, OLED
- **SPI**: microSD
- **UART1**: ultrasonic range sensor
- **UART0**: debug / manufacturing test header
- **Wi-Fi**: MQTT, HTTP JSON, OTA update path
- **BLE**: commissioning + quick local inspection

## ESP32-S3 Pin Assignment

| ESP32-S3 Pin | Function |
|--------------|----------|
| GPIO1 | UART0_TX debug |
| GPIO2 | UART0_RX debug |
| GPIO4 | BUZZER_EN |
| GPIO5 | OLED_RESET |
| GPIO6 | I2C_SDA |
| GPIO7 | I2C_SCL |
| GPIO8 | SD_CS |
| GPIO9 | SPI_MOSI |
| GPIO10 | SPI_SCK |
| GPIO11 | SPI_MISO |
| GPIO12 | FLOAT_IN |
| GPIO13 | LEAK_IN |
| GPIO14 | BUTTON_ACK |
| GPIO15 | BUTTON_MODE |
| GPIO16 | CT_ENVELOPE_ADC |
| GPIO17 | ULTRASONIC_RX |
| GPIO18 | ULTRASONIC_TX |
| GPIO19 | BATTERY_ADC |
| GPIO20 | STATUS_LED_R |
| GPIO21 | STATUS_LED_G |
| GPIO38 | STATUS_LED_B |
| GPIO39 | POWER_GOOD |
| GPIO40 | SENSOR_RAIL_EN |
| GPIO41 | RTC_INT |
| GPIO42 | TAMPER_SW |

> Final PCB routing should re-check boot strapping and module revision notes before fabrication.

## Fault Models and Detection Modes

### 1. Healthy Cycle

- float activates
- current starts within a few seconds
- vibration rises with current
- water level falls at expected drawdown rate
- cycle ends before high-water timeout

### 2. No-Start / Dead Pump

- level rising
- float asserted
- little or no motor current
- little vibration

### 3. Failed Check Valve / Backflow

- pump empties pit normally
- after shutdown, water level rebounds rapidly
- repeated short intervals between cycles

### 4. Short Cycling

- frequent pump restarts with shallow level excursion
- often correlated with backflow or undersized pit volume

### 5. Dry-Run / Intake Starvation

- pump current present
- vibration high or erratic
- water level already low or drawdown stalls near bottom

### 6. Slow Pump / Partial Blockage

- current normal or elevated
- vibration present
- water level decreases too slowly
- cycle time longer than commissioned baseline

### 7. Basement Leak Confirmed

- leak rope wet or floor sensor active
- regardless of pit status

### 8. Mold / Condensation Risk

- RH sustained > 80%
- wall or pit environment near dewpoint margin

## Signal Processing Chain

```text
ultrasonic samples (0.2 Hz idle / 2 Hz active)
    -> median filter
    -> pit level estimate
    -> rise/fall slope
    -> cycle segmentation

CT envelope (200 Hz during cycle)
    -> moving RMS proxy
    -> surge peak
    -> run-state confidence

accelerometer (100/400 Hz)
    -> vibration RMS
    -> shock / cavitation heuristic

all features
    -> rule-based fault classifier with hysteresis
```

### Derived Metrics

- `level_mm`
- `rise_rate_mm_min`
- `drawdown_rate_mm_min`
- `pump_current_a`
- `cycle_duration_s`
- `rebound_mm_60s`
- `humidity_rh`
- `battery_v`
- `fault_score`
- `state`

## Power Architecture

```text
12V adapter / sump UPS ─► TVS + fuse ─► TPS62162 3V3 main rail
                                        ├─► ESP32-S3
                                        ├─► OLED / RTC / SHT45 / LIS2DW12
                                        ├─► microSD
                                        └─► analog front end

18650 cell ─► MCP73831 charger
           └─► TPS2113A ideal-OR backup path to 3V3 rail
```

### Estimated Current Draw

- deep monitoring, OLED dim, Wi-Fi idle: ~18 mA
- active pump-cycle capture, logging only: ~42 mA
- Wi-Fi publish burst: 160–240 mA peak
- backup-only notification mode: 10–15 hours on a 2600 mAh 18650, depending on publish interval

## Mechanical Design

- enclosure mounts above or beside a sump pit lid
- ultrasonic sensor passes through the lid using a rubber compression gland
- CT clamp snaps around the pump hot conductor outside the mains splice box
- leak rope runs around the pit perimeter and nearby floor low spots
- recommended enclosure footprint: **96 mm × 64 mm × 32 mm**
- cable entries: ultrasonic, CT, leak rope, float input, 12 V power

## Firmware Layout

```text
firmware/
├── CMakeLists.txt
├── sdkconfig
├── include/
│   ├── sump_shepherd.h
│   ├── sensors.h
│   ├── classifier.h
│   └── ui.h
└── src/
    ├── main.c
    ├── sensors.c
    ├── classifier.c
    └── ui.c
```

The included firmware is a **host-buildable C simulation** of the embedded logic. It exercises pump-cycle classification using representative demo frames and compiles with standard CMake on Linux before being ported to ESP-IDF.

## Bill of Materials Summary

Full BOM: [`hardware/BOM.csv`](hardware/BOM.csv)

Top cost drivers:

- ESP32-S3-WROOM-1-N8
- A02YYUW waterproof ultrasonic sensor
- split-core CT
- OLED display
- microSD socket
- battery backup components

Expected prototype electronics cost is roughly **$40–$55** depending on battery, enclosure, and whether a packaged UPS input is used.

## Data Model

Each log record captures:

```json
{
  "timestamp": "2026-08-31T02:15:00Z",
  "level_mm": 462.0,
  "rise_rate_mm_min": 14.2,
  "drawdown_rate_mm_min": -88.0,
  "pump_current_a": 7.1,
  "vibration_g_rms": 0.22,
  "humidity_rh": 81.4,
  "float_active": true,
  "leak_active": false,
  "battery_v": 4.01,
  "state": "HEALTHY"
}
```

## Build and Run the Firmware Simulation

```bash
cd firmware
cmake -S . -B build
cmake --build build
./build/sump_shepherd_fw_sim
```

## Python Helpers

In `scripts/`:

- `generate_demo_log.py` creates a synthetic CSV containing healthy, no-start, backflow, dry-run, and flood-leak scenarios
- `analyze_log.py` summarizes the CSV by state counts, maximum level, minimum battery, and average current while pumping

## Recommended Future Revisions

- direct AC waveform ADC for better motor diagnosis
- optional pressure transducer in the discharge line
- external LTE-M telemetry variant for vacation homes without Wi-Fi
- support for dual-pump pits and generator status inputs
- Home Assistant auto-discovery payload generator

## Repository Contents

- [`schematic/`](schematic/) — KiCad project skeleton with architecture notes
- [`firmware/`](firmware/) — host-buildable C classification simulation
- [`hardware/BOM.csv`](hardware/BOM.csv) — full component list
- [`docs/assembly-guide.md`](docs/assembly-guide.md) — mechanical and wiring guidance
- [`docs/api-reference.md`](docs/api-reference.md) — telemetry schema and command set
- [`scripts/`](scripts/) — synthetic log generation and analysis tools

## Safety Notes

- The CT must be installed only around **one insulated conductor**, never around a full mains cable containing both line and neutral.
- Float input wiring should remain isolated from mains pump wiring.
- This device is a **monitor and alert node**, not a safety-rated pump controller.
- Use GFCI protection and sealed cable glands in damp basements.
