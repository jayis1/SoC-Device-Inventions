# Mold Scout — Handheld Hidden-Moisture & Mold-Risk Mapper

> A non-invasive building-diagnostics instrument that scans walls, ceilings, cabinets, and HVAC plenums for hidden moisture, condensation risk, and mold-prone conditions by fusing dielectric sensing, thermal imaging, ambient psychrometrics, and VOC data.

## What It Is

**Mold Scout** is a practical field tool built around the **ESP32-S3-WROOM-1-N8**. It is meant for home inspectors, remediation crews, facilities teams, landlords, and energy auditors who need more than a simple pin moisture meter but less bulk and cost than a full thermal camera plus separate IAQ kit.

The device combines:

- **non-contact capacitive dielectric sensing** through paint, drywall, and thin panel materials
- **32×24 thermal imaging** to find cold spots and wet thermal anomalies
- **ambient temperature / humidity** for dew-point analysis
- **VOC / gas sensing** to flag stale damp cavities and microbial off-gassing trends
- **IMU-assisted sweep mode** for repeatable wall scans
- **local OLED + haptic alerts + microSD logging + BLE/Wi-Fi export**

The result is a pocketable instrument that can answer:

- Is this cold corner merely under-insulated, or also wet?
- Is this cabinet back panel below dew point often enough to support mold?
- Is this drywall section measurably wetter than the surrounding baseline?
- Is a room recovering after remediation, or still emitting suspicious VOC signatures?

## Why This Device Matters

Mold investigations often fail because each single sensor lies in a different way:

- thermal cameras confuse insulation defects with moisture
- cheap pinless meters are badly affected by material type
- humidity meters cannot localize hidden leaks
- VOC sensors alone cannot distinguish cleaners from biological activity

**Mold Scout** improves confidence by requiring multiple independent indicators before escalating a finding.

## Main SoC Choice

### ESP32-S3-WROOM-1-N8

The ESP32-S3 is a good fit because it provides:

- dual-core performance for UI, sensing, and light analytics
- Wi-Fi + BLE for mobile sync and web export
- enough RAM/flash for thermal frames, history buffers, and calibration tables
- hardware USB for service and factory calibration
- broad ecosystem support and good low-cost availability

## Functional Overview

1. A guarded front-face electrode drives a swept AC field into the wall surface.
2. The **AD5933** measures complex response magnitude/phase, which shifts with dielectric loading from moisture.
3. An **MLX90640** captures a thermal frame of the same area.
4. **SHT45** ambient data is used to compute dew point and surface condensation margin.
5. **BME688** adds VOC index and gas resistance trend.
6. **ICM-42688-P** tags motion so sweep data can be spatially averaged and poor scans rejected.
7. Firmware computes:
   - moisture proxy
   - condensation risk
   - thermal bridge score
   - VOC risk
   - combined mold risk score
8. Results are shown on an OLED, signaled by haptic pulses, logged to microSD, and exported via BLE/Wi-Fi.

## Block Diagram

```text
                   ┌──────────────── Mold Scout ────────────────┐
Front guarded pad ─► AD5933 impedance AFE ── I2C ─┐             │
                                                   │             │
Thermal scene ────► MLX90640 32x24 IR array ─ I2C ┤             │
Ambient RH/T ─────► SHT45 ─────────────────── I2C ┤             │
VOC / gas ────────► BME688 ────────────────── I2C ┤             │
Motion / tilt ────► ICM-42688-P ───────────── SPI ┤             │
Fuel gauge ───────► MAX17048 ──────────────── I2C ┤             │
RTC ──────────────► DS3231M ───────────────── I2C ┤             │
                                                   ▼             │
                                      ESP32-S3-WROOM-1-N8       │
                                   - scan fusion                 │
                                   - psychrometrics              │
                                   - baseline learning           │
                                   - UI / haptic policy          │
                                   - BLE / Wi-Fi export          │
                                   - microSD logging             │
                                                                 │
OLED 128x64 ─────────────────────────────── I2C                  │
microSD socket ──────────────────────────── SPI                  │
DRV2605L haptic driver ──────────────────── I2C                  │
USB-C / Li-ion charger / buck-boost ────── power                 │
                   └─────────────────────────────────────────────┘
```

## Full Schematic Design

### Major ICs and Subsystems

| Ref | Part | Function |
|-----|------|----------|
| U1 | ESP32-S3-WROOM-1-N8 | Main SoC, analytics, UI, BLE/Wi-Fi |
| U2 | AD5933 | Impedance converter for guarded dielectric moisture probe |
| U3 | OPA320AIDBVR | Guard driver / probe buffer / analog conditioning |
| U4 | MLX90640ESF | 32×24 thermal array for cold-spot and evaporative anomaly mapping |
| U5 | SHT45 | Ambient temperature + humidity |
| U6 | BME688 | VOC / gas resistance / auxiliary temp-humidity cross-check |
| U7 | ICM-42688-P | 6-axis IMU for sweep quality and orientation |
| U8 | MAX17048 | Li-ion fuel gauge |
| U9 | DS3231M | RTC for timestamping |
| U10 | DRV2605L | Haptic driver for silent field alerts |
| DS1 | SH1107 1.3 in OLED | Local display |
| J1 | microSD socket | Log storage |
| U11 | TPS63070 | 3.3 V buck-boost regulator from single-cell Li-ion |
| U12 | MCP73831 | Li-ion charger from USB-C 5 V |
| U13 | TPD4E05U06 | USB ESD protection |
| U14 | TPS22916 | Switched sensor rail |
| BT1 | 18650 or flat LiPo | Main battery |
| H1 | Guarded scan electrode | Non-contact wall moisture probe |
| M1 | Coin haptic motor | User alerts |
| SW1/SW2 | Power / mode buttons | UI input |

### Sensing Front End

The hidden-moisture probe is a **guarded coplanar electrode** on the front PCB edge.

- center sense plate couples electric field into the wall
- driven guard ring follows the sense potential through **OPA320** to reduce fringe loss into the user’s hand
- **AD5933** injects a programmable sinusoid, typically 100 kHz, 250 kHz, and 500 kHz spot measurements
- firmware uses magnitude ratio and phase shift relative to an air baseline and a dry-reference tile

This is not a laboratory moisture percentage meter. It is a **comparative dielectric scanner** that is strongest at detecting *differences* across a surface.

### Thermal Imaging

**MLX90640** provides a low-resolution thermal map that is still sufficient for:

- cold-corner detection
- wet drywall evaporative cooling detection
- HVAC supply register leakage patterns
- condensation halos around pipe chases and window frames

The firmware computes:

- min / max / mean surface temperature
- local contrast
- coldest-pixel dew-point margin
- edge-to-center thermal asymmetry

### Ambient + VOC Sensing

- **SHT45** is the primary psychrometric sensor for accurate room RH / temperature
- **BME688** provides VOC index and gas resistance trend, used cautiously as a secondary risk input
- elevated VOC matters most when paired with moisture or repeated condensation findings

### Motion and Sweep Mode

The **ICM-42688-P** is used to:

- reject scans with excessive hand jerk
- estimate linear sweep consistency
- support “line scan” averaging by segmenting motion windows
- detect when the device is held flush against a wall vs. in free air

## ESP32-S3 Pin Assignment

| ESP32-S3 Pin | Function |
|--------------|----------|
| GPIO1 | I2C_SCL |
| GPIO2 | I2C_SDA |
| GPIO4 | SD_CS |
| GPIO5 | SPI_MOSI |
| GPIO6 | SPI_SCK |
| GPIO7 | SPI_MISO |
| GPIO8 | IMU_CS |
| GPIO9 | OLED_RESET |
| GPIO10 | HAPTIC_EN / INT |
| GPIO11 | BUTTON_MODE |
| GPIO12 | BUTTON_MARK |
| GPIO13 | BATTERY_ALERT |
| GPIO14 | SENSOR_RAIL_EN |
| GPIO15 | USB_CDC_TX indicator / debug |
| GPIO16 | USB_CDC_RX indicator / debug |
| GPIO17 | RGB_LED_R |
| GPIO18 | RGB_LED_G |
| GPIO21 | PROBE_EXCITE_MON |
| GPIO33 | BATTERY_ADC |
| GPIO34 | CHARGER_STAT |
| GPIO35 | THERMAL_INT |
| GPIO36 | BME_INT |
| GPIO37 | AD5933_INT |
| GPIO38 | IMU_INT |
| GPIO39 | WAKE_BUTTON |
| GPIO40 | RGB_LED_B |
|
> Final PCB layout must verify strapping-pin and USB pin constraints for the chosen ESP32-S3 module revision.

## Key Nets

- **VBUS_5V**: USB-C input to charger and ESD network
- **VBAT**: single-cell Li-ion battery rail
- **3V3_MAIN**: regulated logic rail from TPS63070
- **3V3_SENS**: switchable sensor rail via TPS22916
- **I2C0**: AD5933, MLX90640, SHT45, BME688, MAX17048, DS3231M, DRV2605L, OLED
- **SPI2**: microSD + IMU
- **PROBE_GUARD**: buffered guard ring output
- **PROBE_SENSE**: front electrode return into AD5933 network

## Measurement Modes

### 1. Spot Scan
Press the front face to one location and acquire a single fused reading.

### 2. Line Sweep
Drag across a wall seam or baseboard. Firmware averages windows and highlights peaks.

### 3. Baseline Learn
Capture a known-dry reference area in the room. Future scans are normalized against it.

### 4. Remediation Check
Compare pre-clean and post-drying scans over time from the microSD log.

## Signal Processing Chain

```text
AD5933 magnitude/phase + thermal frame + ambient RH/T + VOC + motion
    -> baseline normalization
    -> dew point computation
    -> moisture proxy extraction
    -> thermal bridge / cold spot extraction
    -> VOC confidence weighting
    -> motion quality filter
    -> fused mold risk score
    -> state classification + alert policy
```

### Derived Metrics

- **moisture_proxy**: normalized dielectric loading relative to baseline
- **condensation_risk**: score from dew-point margin and coldest surface pixel
- **thermal_bridge_score**: spatial cold anomaly without strong moisture evidence
- **voc_risk**: scaled VOC index gated by humidity and moisture evidence
- **mold_risk_score**: fused 0-1 scalar used for alerts and logging

## Classification States

| State | Meaning |
|-------|---------|
| Normal | No meaningful anomaly versus baseline |
| Condensation Risk | Surface or near-surface area is near/below dew point |
| Hidden Moisture | Dielectric response is elevated beyond thermal-only explanation |
| Active Mold Risk | Moisture + condensation history or VOC evidence support probable growth conditions |
| Wet Building Material | Strong dielectric anomaly and cold pattern indicate likely active wetting |

## Power Architecture

### Input and Charging

- USB-C 5 V input
- **MCP73831** linear charger for single-cell Li-ion
- charge LED routed to user status window

### Main Regulation

- **TPS63070** buck-boost regulator generates 3.3 V from 2.5-5.5 V input range
- allows stable operation over most Li-ion discharge curve

### Power Budget

| Mode | Approx. Current |
|------|-----------------|
| Deep sleep | 0.35 mA |
| Idle UI | 28 mA |
| Spot scan | 95 mA |
| Thermal + SD logging + Wi-Fi upload | 210 mA peak |
| Haptic pulse add-on | +65 mA short burst |

Typical runtime with a 2500 mAh cell is 10-14 hours of intermittent field work.

## PCB Layout Guidance

1. Keep the **guarded moisture electrode** at the front edge and away from the user-hand battery cavity.
2. Route **PROBE_SENSE** short, shielded by guard copper where possible.
3. Separate the AD5933 analog return from Wi-Fi burst current loops.
4. Keep the **MLX90640** thermally isolated from regulator heat and OLED back emissions.
5. Put the OLED and buttons at the top face, sensor pad at the bottom face.
6. Use a perimeter ground shield around the USB-C and charger section.

## Firmware Architecture

```text
main.c
 ├─ sensors.c      -> demo data loader / sensor abstraction
 ├─ dsp.c          -> psychrometrics and score extraction
 ├─ classifier.c   -> state machine and recommendation logic
 └─ mold_scout.h   -> shared structures
```

The included C firmware is a **portable reference implementation** of the scan-fusion logic. It compiles on a host PC and can be migrated into ESP-IDF tasks.

## Data Logging Format

CSV fields:

```text
timestamp,ambient_temp_c,ambient_rh,surface_temp_c,thermal_delta_c,impedance_kohm,phase_deg,voc_index,gas_resistance_kohm,accel_motion_g,battery_v,state,mold_risk_score
```

## Example Use Cases

- checking a bathroom vanity kickplate for hidden plumbing seepage
- screening an exterior wall corner for winter condensation
- comparing a suspected roof leak boundary before opening drywall
- auditing classroom cabinets and HVAC closets after flood cleanup
- giving tenants a documented dry-vs-wet comparison instead of guesses

## Limitations

- dielectric readings depend on material stackup and salinity
- metal lath, foil-backed insulation, and dense tile can distort readings
- VOC readings are not species-specific and must not be treated as mold identification
- thermal scans require emissivity awareness on glossy surfaces

## Build Notes

- front housing should expose the thermal sensor through an IR window or open baffled aperture
- the scan electrode works best with a thin plastic front skin under 1.5 mm
- include a removable dry-reference tile for field normalization
- haptic alerts are preferred over buzzers in occupied indoor spaces

## Files in This Folder

- `schematic/` — KiCad project placeholders with component placement and net notes
- `firmware/` — C reference firmware and host-buildable demo
- `hardware/BOM.csv` — core BOM
- `docs/assembly-guide.md` — enclosure and assembly details
- `docs/api-reference.md` — telemetry schema and command surface
- `scripts/risk_report.py` — helper to summarize exported scan logs

## Next Hardware Iterations

- add optional contact temperature tongue for high-confidence condensation checks
- add room-mapping phone app that stitches line sweeps onto floor plans
- add calibration coupons for drywall, MDF, plaster, and painted masonry

## License

MIT
