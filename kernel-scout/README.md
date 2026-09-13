# Kernel Scout — Pocket Grain Moisture, Density, and Storage-Risk Analyzer

> A handheld grain-receipt tool that combines calibrated RF capacitance, sample mass, temperature/humidity, and contact acoustics to estimate moisture, test weight, insect activity, and safe-storage time before a bin or truck load becomes a loss.

## Purpose

**Kernel Scout** helps small farms, co-ops, maltsters, and seed banks make a fast, repeatable intake decision from a 120 mL grain cup. Moisture alone does not predict storage loss: warm grain, low bulk density, and insect activity can make an apparently acceptable lot unsafe. The instrument records the whole sample signature and gives a conservative storage recommendation rather than a bare moisture number.

It supports wheat, maize, barley, rice, soybean, and a user calibration profile. It is a screening instrument, not a legal trade reference; calibrate against an oven-dry reference before grading or settlement use.

## SoC and architecture

The **ESP32-S3-WROOM-1-N8** is the only processor. Its dual cores leave one core for deterministic acquisition and the other for UI, BLE/Wi-Fi sync, CSV logging, and OTA. A low-noise external ADC handles the bridge channels while ESP32 ADC/DAC peripherals support excitation and battery monitoring.

```text
 Sample cup ── copper electrode sleeve ── AD5933 impedance converter ── I2C ┐
 Sample cup ── 500 g load cell ── HX711 ── GPIO                              │
 Cup wall ─── TMP117 temperature ── I2C ─────────────────────────────────────┤
 Cup base ─── piezo + OPA320 envelope ── ADS122C04 ── I2C ────────────────────┤
 SHT45 air temperature/RH ── I2C ────────────────────────────────────────────┤ ESP32-S3
 microSD ── SPI ─────────────────────────────────────────────────────────────┤  moisture fusion
 OLED / buttons / buzzer ────────────────────────────────────────────────────┤  risk model
 USB-C + Li-ion ─ charger / 3V3 power tree ──────────────────────────────────┘  BLE/Wi-Fi
```

## What is measured

| Measurement | Circuit | Range / intended resolution |
|---|---|---|
| Dielectric response | AD5933, 100 kHz guarded cylindrical electrode | profile-calibrated 8–30% wet basis, 0.3% target repeatability |
| Sample mass | 500 g single-point load cell + HX711 | 0–500 g, 0.1 g display |
| Bulk density/test weight | fixed 120.0 mL cup + mass | 300–900 kg/m³, derived kg/hL |
| Grain temperature | TMP117 bonded to cup wall | -20–50 °C, ±0.1 °C |
| Ambient RH / temperature | SHT45 | storage-equilibrium context |
| Insect / crack activity | piezo disc, OPA320 band-limited AFE, ADS122C04 | impulse rate over 20 s |

## Measurement model

A dry cup baseline `Z0` is stored per grain profile. The AD5933 returns a calibrated magnitude `M`. Temperature-compensated capacitance response is:

```text
r = (M - M0) / M0
m_wb = a0 + a1*r + a2*r² + a3*T_grain
```

Coefficients are fitted from sealed samples checked by 103 °C oven drying (or the applicable crop method). The firmware rejects readings when fill mass is outside the crop profile range, cup temperature is changing faster than 0.2 °C/min, or the impedance repeatability is poor.

Bulk density:

```text
rho_kg_m3 = mass_g / 120.0e-6 / 1000
kg_per_hL = mass_g / 1.2
```

The conservative storage index is calculated from moisture, grain temperature, ambient RH, and insect impulse rate. It returns one of **DRY**, **CONDITION**, **AERATE**, or **URGENT DRY** and an estimated safe-storage window. It is deliberately capped at 180 days and must not replace local grain-storage guidance.

## Schematic design

### Signal path

1. **J1 / sample cup**: A removable 120 mL anodized-aluminum cup has an insulating liner and two stainless/copper electrode rings. The outer guard ring is driven by the AD5933 excitation guard buffer; the inner sense electrode connects through 1 kΩ + BAV199 clamp diodes.
2. **U2 AD5933**: configured for 100 kHz, 1 Vp-p excitation. RFB is populated per impedance range; use 1 kΩ, 1% as the initial value. Its I²C bus shares SDA/SCL with environmental sensors but has a local 3.3 kΩ pull-up pair.
3. **LC1 + U3 HX711**: 500 g four-wire load cell is ratiometrically excited at 3.3 V. A 100 Ω / 100 nF common-mode filter enters A+/A−. HX711 DOUT and SCK go directly to the SoC.
4. **PZ1 / U4 OPA320**: the piezo disc is AC-coupled to a 100× non-inverting amplifier biased at 1.65 V, then a 100 Hz high-pass / 8 kHz low-pass network. ADS122C04 samples at 2 kS/s; the firmware counts qualified impacts, not audio identity.
5. **U5 TMP117** and **U6 SHT45**: I²C sensors. Thermally bond TMP117 to the cup with electrically insulating epoxy and keep it away from the Li-ion charger.
6. **U7 MAX17048**: battery fuel gauge on I²C; **U8 MCP73831** charges the protected 1200 mAh cell from USB-C.

### Power architecture

```text
USB-C VBUS ─ F1 500 mA ─ TVS ─ MCP73831 ─ LiPo 3.7 V
                                      │
LiPo / USB ─ TPS63031 buck-boost ─ 3V3_DIG ─ ESP32-S3, display, SD
                                   └ 3V3_A via ferrite + 10 uF ─ ADC/AFE/sensors
```

Keep AD5933, OPA320, ADS122C04 and cup connectors in the analog island. The RF antenna end of the ESP32 module requires a copper-free keep-out matching Espressif's module data sheet. A USB-C CC resistor pair (5.1 kΩ to ground) advertises a sink-only port.

### Pin assignment

| ESP32-S3 pin | Net / use |
|---|---|
| GPIO1 / GPIO2 | I²C SDA/SCL: AD5933, TMP117, SHT45, MAX17048, ADS122C04 |
| GPIO4 / GPIO5 | HX711 DOUT / SCK |
| GPIO6 / GPIO7 / GPIO15 | SPI SCK / MOSI / CS to microSD |
| GPIO8 / GPIO9 | OLED I²C reset / interrupt or shared bus connector |
| GPIO10 | Piezo capture DRDY from ADS122C04 |
| GPIO11 | Buzzer MOSFET gate |
| GPIO12 / GPIO13 | buttons: measure / profile |
| GPIO14 | encoder switch |
| GPIO16 / GPIO17 | USB-UART debug RX/TX |
| GPIO18 | AD5933 MCLK enable / calibration switch |
| GPIO21 | battery divider ADC |

Do not use strap pins for external pull-downs. Add 33 Ω series resistors at all off-board digital connectors and ESD protection at USB-C and the cup probe contacts.

## BOM

See [`hardware/BOM.csv`](hardware/BOM.csv). Estimated single-unit electronic cost excluding enclosure and calibration labor: **about US$49**.

## Operation

1. Charge over USB-C, install a profile, and run `EMPTY CUP` to capture the baseline.
2. Fill and level the cup without tapping it; fit the lid to make packing repeatable.
3. Press **Measure**. The device weighs the fill, makes five impedance sweeps, samples grain and air temperature, then listens for 20 seconds.
4. Review moisture, kg/hL, storage class, and confidence. Save to microSD or transfer via BLE/Wi-Fi.
5. Every season, calibrate each crop profile with at least six samples spanning its expected moisture range.

## Data API

The ESP32 hosts a local JSON status endpoint when Wi-Fi is enabled:

```json
{"profile":"wheat","moisture_wb_pct":13.4,"bulk_density_kg_m3":782.1,"test_weight_kg_hl":78.2,"grain_temp_c":24.1,"insect_impulses_min":2,"storage_class":"AERATE","safe_days":35,"confidence_pct":91}
```

CSV rows written to `/kernel-scout.csv` use the same fields preceded by ISO-8601 UTC time and a device serial number. See [`docs/API.md`](docs/API.md).

## Calibration and safety

- The cup and electrode assembly must be dry and free of metallic dust before baseline capture.
- Oven-reference moisture methods and crop-specific regulations take precedence over this design's estimate.
- Do not use on grain treated with flammable solvent; this is not intrinsically safe equipment.
- Never infer infestation species from the piezo count. It is a risk flag, not an entomology classifier.

## Firmware

`firmware/` contains portable C reference code for the fusion and storage-risk model. It builds as a host executable for verification; the HAL integration points are intentionally isolated in `sensors.c`.

```bash
cmake -S firmware -B firmware/build
cmake --build firmware/build
./firmware/build/kernel_scout_demo
```

## Files

```text
kernel-scout/
├── schematic/kernel-scout.kicad_sch  # KiCad 7 architecture sheet
├── hardware/BOM.csv
├── firmware/                          # C11 reference acquisition and model
├── docs/ASSEMBLY.md
├── docs/API.md
└── scripts/calibrate_profile.py
```

## License

MIT.
