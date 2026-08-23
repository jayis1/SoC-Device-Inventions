# Stoma Clip — Leaf Stomatal Conductance & Water-Stress Meter

> A handheld clamp-on porometer that measures leaf stomatal conductance, transpiration proxy, leaf-to-air temperature delta, and drought stress in under 30 seconds using an ESP32-S3.

## What It Is

**Stoma Clip** is a buildable field instrument for growers, plant scientists, greenhouse operators, and advanced gardeners who need a fast answer to a practical question:

**Is this plant actually exchanging water and CO₂ normally right now?**

The device clips onto a leaf, briefly flushes a small chamber, closes it, watches humidity rise, and estimates **stomatal conductance** from the measured water-vapor accumulation rate. It also measures:

- **Leaf temperature**
- **Ambient temperature / RH / pressure**
- **Chamber temperature / RH**
- **PAR trend proxy** using a spectral light sensor
- **Battery and blower current**
- **Derived water-stress score**

This makes it useful for:

- irrigation timing in orchards, vineyards, and greenhouses
- drought-stress screening in breeding trials
- comparing cultivars or fertilizer treatments
- detecting midday stomatal closure
- validating misting / VPD control strategies in controlled environments
- classroom demonstrations of transpiration and plant physiology

Unlike a soil probe, Stoma Clip measures the **leaf’s physiological response**, not just the environment around the roots.

## Why This Device Matters

Growers often infer plant stress indirectly from soil moisture, weather, or visual wilt. But plants can close stomata well before visible stress appears, and they can remain stressed even when soil moisture looks acceptable due to root damage, salinity, disease, or high vapor pressure deficit.

Commercial porometers exist, but they are often expensive, proprietary, and difficult for makers or smaller labs to customize. Stoma Clip provides an open, practical path to a leaf gas-exchange screening tool built from available components.

## Core SoC Choice

### Main SoC: ESP32-S3-WROOM-1-N8R8

The **ESP32-S3** was chosen because it combines:

- enough compute for sensor fusion and local analytics
- Wi-Fi + BLE for phone sync and greenhouse dashboards
- native USB for service, calibration, and data export
- plentiful GPIO for display, SD, I2C sensors, motor/valve control, and haptics
- low enough cost to keep the whole instrument realistic

## Measurement Principle

Stoma Clip uses a **small leaf chamber** with a known internal volume. The measurement cycle is:

1. **Clamp** the leaf between a soft silicone gasket and a spring jaw.
2. **Flush** the chamber with ambient air using a miniature blower.
3. **Close** the chamber flap using a latching micro-servo.
4. **Monitor** the rise in chamber humidity over a fixed interval.
5. **Estimate transpiration flux** from water-vapor density slope.
6. **Compute stomatal conductance** from transpiration and vapor pressure deficit.
7. **Report a water-stress score** using conductance, leaf temperature delta, and light level.

This is not a full research photosynthesis system; it is a **portable, rapid-screening porometer**.

## Block Diagram

```text
             ┌──────────────────── Stoma Clip ────────────────────┐
             │                                                     │
Leaf ───────►│ Silicone-sealed clip chamber                        │
             │     │                                               │
             │     ├── SHT45_CHAMBER  ──┐                          │
             │     ├── TMP117_LEAF   ───┼─ I2C ─► ESP32-S3         │
Ambient ────►│     ├── SHT45_AMBIENT ───┤                          │
Light ──────►│     ├── AS7341 PAR proxy ┤                          │
             │     └── INA219 current ──┘                          │
             │                                                     │
             │ Mini blower ─ MOSFET PWM                            │
             │ Servo flap valve ─ PWM                              │
             │ Haptic motor ─ DRV2605                              │
             │ 1.3" OLED ─ I2C                                     │
             │ microSD ─ SPI                                       │
             │ USB-C / BLE / Wi-Fi                                 │
             │                                                     │
             │ LiPo -> MCP73831 -> TPS63031 3V3                    │
             │             └-> TPS61023 5V actuator rail           │
             └─────────────────────────────────────────────────────┘
```

## Full Hardware Design

### Major ICs and Subsystems

| Ref | Part | Function |
|-----|------|----------|
| U1 | ESP32-S3-WROOM-1-N8R8 | Main SoC, control, analytics, BLE/Wi-Fi |
| U2 | SHT45-DIS-B | Chamber humidity + chamber temperature |
| U3 | SHT45-DIS-B | Ambient humidity + ambient temperature |
| U4 | TMP117AIDRVR | Leaf-contact temperature sensor in copper tongue |
| U5 | AS7341 | Spectral light sensor used as PAR trend proxy |
| U6 | INA219AIDCNR | Battery / blower current monitor |
| U7 | DRV2605L | Haptic driver for pass/fail cue and UI |
| U8 | SSD1306 128x64 OLED | User display |
| U9 | MicroSD socket | Data logging |
| U10 | MCP73831T-2ACI | Single-cell Li-ion charger |
| U11 | TPS63031 | 3.3 V buck-boost system rail |
| U12 | TPS61023 | 5 V boost rail for servo and blower |
| U13 | AO3400A | Low-side blower switch |
| U14 | AO3407A | High-side actuator rail isolation |
| U15 | BSS138 level shifter | Optional 5 V servo PWM level shifting |
| U16 | PCF85063A | RTC for timestamped logs |
| M1 | 5 V micro blower | Chamber purge airflow |
| M2 | SG90-class micro-servo or custom 5 V linear flap actuator | Closes isolation flap |
| B1 | 1200 mAh LiPo | Portable power |

### Sensor Layout

- **SHT45 chamber sensor** is mounted inside the clip body but shielded from direct condensation droplets.
- **SHT45 ambient sensor** is vented to the outside and isolated from blower exhaust.
- **TMP117** is bonded to a thin ENIG copper tongue under Kapton insulation so it lightly touches the back of the leaf.
- **AS7341** faces upward through a diffuser window to estimate incident light category.
- Silicone gaskets define an approximately **4.8 cm² exposed leaf area** inside the chamber.

## Power Architecture

### Rails

| Rail | Source | Loads |
|------|--------|-------|
| VBAT | 1-cell LiPo, 3.0–4.2 V | Main battery rail |
| VSYS_3V3 | TPS63031 | ESP32-S3, sensors, OLED, microSD, RTC |
| VACT_5V | TPS61023 | blower, servo actuator, haptic headroom |
| VUSB_5V | USB-C input | charger input and service power |

### Power Path

1. USB-C feeds **MCP73831** for LiPo charging.
2. Battery feeds **TPS63031** for the always-on 3.3 V logic rail.
3. Battery also feeds **TPS61023** when measurements start, generating 5 V for blower/servo.
4. The 5 V rail is switched off outside active cycles to reduce quiescent loss.
5. INA219 monitors battery current so the firmware can detect blower stalls and low-battery conditions.

### Typical Consumption

| Mode | Current |
|------|---------|
| Sleep advertising | ~18 mA |
| Idle display on | ~52 mA |
| Purge + servo move | ~260–420 mA peak |
| Closed-chamber measurement | ~78 mA |
| Wi-Fi sync burst | ~180 mA average |

## ESP32-S3 Pin Assignment

| ESP32-S3 Pin | Function |
|--------------|----------|
| GPIO1 | UART_TX debug |
| GPIO2 | UART_RX debug |
| GPIO4 | I2C_SDA |
| GPIO5 | I2C_SCL |
| GPIO6 | OLED_RESET |
| GPIO7 | HAPTIC_INT |
| GPIO8 | SERVO_PWM |
| GPIO9 | BLOWER_PWM |
| GPIO10 | ACTUATOR_EN |
| GPIO11 | BUTTON_MEASURE |
| GPIO12 | BUTTON_MENU |
| GPIO13 | BUTTON_BACK |
| GPIO14 | RGB_LED_R |
| GPIO15 | RGB_LED_G |
| GPIO16 | RGB_LED_B |
| GPIO17 | SD_CS |
| GPIO18 | SPI_SCK |
| GPIO19 | SPI_MISO |
| GPIO20 | SPI_MOSI |
| GPIO21 | RTC_INT |
| GPIO35 | CLIP_CLOSED_SWITCH |
| GPIO36 | LEAF_PRESENT_SWITCH |
| GPIO37 | USB_D- |
| GPIO38 | USB_D+ |
| GPIO39 | BATT_ALERT |
| GPIO40 | BUZZER_PWM |
| GPIO41 | FACTORY_TEST_PAD |
| GPIO42 | BOOT_STATUS_PAD |

> Verify final strap-pin compatibility during PCB routing. The table above is an electrical planning reference.

## Chamber Geometry and Measurement Model

### Mechanical chamber assumptions

| Parameter | Value |
|----------|-------|
| Exposed leaf area | 4.8 cm² |
| Chamber free volume | 7.6 cm³ |
| Purge flow | 0.85 L/min nominal |
| Closure interval | 18 s default |
| Sample cadence | 1 Hz chamber RH/T |

### Physics model

The firmware converts chamber RH and temperature to water-vapor density, then estimates the slope during the sealed interval:

```text
rho_v = RH × rho_sat(T)
E = (V / A) × d(rho_v)/dt
g_sw = E / VPD
```

Where:

- `rho_v` = chamber water-vapor density
- `rho_sat(T)` = saturation vapor density at chamber temperature
- `V` = chamber free volume
- `A` = exposed leaf area
- `E` = transpiration mass flux proxy
- `VPD` = vapor pressure deficit between leaf and ambient air
- `g_sw` = stomatal conductance to water vapor

The instrument reports **mol·m⁻²·s⁻¹** after the density-to-molar conversion.

### Derived metrics

- **Leaf-air ΔT** = leaf temperature − ambient temperature
- **VPD** from ambient temperature and RH
- **Stomatal conductance** `g_sw`
- **Transpiration proxy** `E`
- **Stress score 0–100**
- **Condition bucket**: hydrated / watch / stressed / severe

## Schematic Design Details

### 1) ESP32-S3 core

- ESP32-S3 module powered from **VSYS_3V3**
- EN pin pulled up with 10 kΩ, reset switch to ground
- GPIO0 boot button to ground through tact switch
- USB-C D+/D− routed directly for USB serial/JTAG
- 100 nF local decoupling near each module power pin access point

### 2) I2C sensor bus

The following devices share the main I2C bus with 4.7 kΩ pull-ups to 3.3 V:

- U2 SHT45 chamber
- U3 SHT45 ambient
- U4 TMP117
- U5 AS7341
- U6 INA219
- U7 DRV2605L
- U8 SSD1306 OLED
- U16 PCF85063A RTC

Suggested addresses:

| Device | Address |
|--------|---------|
| SHT45 chamber | 0x44 |
| SHT45 ambient | 0x45 via address translator / mux footprint |
| TMP117 | 0x48 |
| AS7341 | 0x39 |
| INA219 | 0x40 |
| DRV2605L | 0x5A |
| SSD1306 | 0x3C |
| PCF85063A | 0x51 |

Because SHT45 parts share a fixed address, the board includes an **optional TCA9546A-compatible mux footprint** or alternate one-wire bus gating jumper for the second humidity sensor.

### 3) Blower drive

- Micro blower supplied from **VACT_5V**
- Low-side **AO3400A** MOSFET controlled by GPIO9 PWM
- Flyback path added if inductive blower type requires it
- Shunt monitored by INA219 for blockage detection

### 4) Servo flap drive

- 5 V servo rail gated by AO3407A high-side switch
- PWM control from GPIO8
- Optional BSS138-based level shifting for 5 V-compatible pulse amplitude
- End-stop switch on chamber flap returns closure confirmation

### 5) Leaf-contact temperature tongue

- TMP117 mounted near flex tongue root
- Thin copper finger extends into silicone jaw with Kapton cover
- Sensor thermally coupled with filled epoxy bead
- Tongue isolated from user touch and direct ambient drafts

### 6) Storage and UI

- SSD1306 OLED on I2C
- microSD on SPI with 47 kΩ pull-up on CS
- Three tact buttons
- RGB LED + piezo buzzer + haptic motor for glanceable status

## Firmware Architecture

```text
main.c
 ├── sensors.c      -> raw sensor capture and demo trace generation
 ├── porometer.c    -> vapor-density, VPD, slope fit, conductance, stress model
 └── ui.c           -> human-readable console/report formatting
```

### Measurement state machine

1. **IDLE**
2. **CLAMP_CHECK**
3. **PURGE**
4. **SEAL**
5. **MEASURE**
6. **COMPUTE**
7. **DISPLAY / LOG / SYNC**

### Fault checks

- chamber RH fails to rise after seal
- blower current too low or too high
- leaf absent switch open
- battery below measurement threshold
- chamber leak inferred from poor linear fit
- ambient/chamber sensor disagreement during purge

## Example Output

```text
Stoma Clip demo measurement
leaf temp: 29.42 C
ambient temp: 31.10 C
ambient RH: 43.50 %
VPD: 2.57 kPa
transpiration proxy: 0.0051 g m^-2 s^-1
stomatal conductance: 0.183 mol m^-2 s^-1
stress score: 31.7 / 100
condition: hydrated
fit r^2: 0.996
```

## Bill of Materials Summary

See [`hardware/BOM.csv`](hardware/BOM.csv) for the line-item BOM. Estimated prototype electronics cost is about **$49–$63**, depending on clip mechanics and enclosure choices.

## Files in This Device Folder

```text
stoma-clip/
├── README.md
├── schematic/
│   ├── stoma-clip.kicad_pro
│   ├── stoma-clip.kicad_sch
│   └── stoma-clip.kicad_pcb
├── firmware/
│   ├── CMakeLists.txt
│   ├── sdkconfig
│   ├── include/
│   │   ├── porometer.h
│   │   ├── sensors.h
│   │   ├── stoma_clip.h
│   │   └── ui.h
│   └── src/
│       ├── main.c
│       ├── porometer.c
│       ├── sensors.c
│       └── ui.c
├── hardware/
│   └── BOM.csv
├── docs/
│   ├── assembly-guide.md
│   └── api-reference.md
└── scripts/
    ├── analyze_leaf_log.py
    └── generate_porometer_dataset.py
```

## Build

### Desktop simulation build

```bash
cd firmware
cmake -S . -B build
cmake --build build
./build/stoma_clip_fw_sim
```

### ESP-IDF adaptation notes

The included `sdkconfig` is a planning sketch showing target and peripheral expectations. The simulation code is intentionally host-buildable to make the measurement model testable before hardware bring-up.

## Calibration Workflow

1. Zero the clip with no leaf inserted.
2. Run ambient sensor cross-check for 60 s in open air.
3. Verify chamber volume using a reference humidity step.
4. Calibrate blower purge time so chamber matches ambient within ±1.5 %RH.
5. Validate slope fit using a wetted calibration membrane.
6. Compare against a reference porometer on at least 3 species.

## Practical Use Cases

- **Greenhouse tomatoes:** spot midday stomatal closure before yield drops.
- **Vineyards:** compare deficit irrigation blocks.
- **Tree nurseries:** screen transplant shock.
- **Plant breeding:** rank drought-tolerant lines quickly.
- **Education:** demonstrate transpiration, VPD, and stomatal regulation.

## Limitations

- It estimates porometer-style conductance, not full photosynthetic assimilation.
- Chamber leaks and poor leaf sealing can bias low.
- Pubescent or highly curved leaves may need alternate gaskets.
- AS7341 is only a PAR proxy, not a calibrated quantum sensor.
- Species-specific validation is recommended before agronomic decisions.

## Future Extensions

- replace servo with bistable magnetic flap
- add barometric correction in conductance model
- add IR leaf thermopile for non-contact cross-check
- add greenhouse LoRa relay variant
- support fleet uploads into unified TREE irrigation dashboards

## License

MIT
