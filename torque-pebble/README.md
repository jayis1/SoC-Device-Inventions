# Torque Pebble — Inline Fastener Torque-Angle Auditor

> A palm-sized smart square-drive adapter that measures torque, angle, rundown behavior, and joint quality while a technician tightens a fastener, then logs a build trace over OLED, microSD, BLE, and Wi-Fi.

## What It Is

**Torque Pebble** is a buildable assembly-quality instrument built around the **ESP32-S3-WROOM-1-N8**. It sits between a ratchet / driver and a socket or bit holder, turning an ordinary hand tool into a traceable torque-angle analyzer.

Instead of reporting only peak torque, Torque Pebble captures the full tightening event:

- **bidirectional torque** from a full-bridge foil strain-gauge torsion shaft
- **absolute angle** from a magnetic rotary encoder
- **tool pose / motion** from a 6-axis IMU
- **temperature** for strain-gauge compensation
- **trigger and state-machine timing** for rundown analytics

That lets it distinguish real assembly problems that a click wrench misses:

- under-torque
- over-torque
- soft-joint behavior
- high prevailing torque / thread damage
- cross-thread risk
- angle-after-snug nonconformance
- suspicious rework or back-off events

## Why This Device Matters

Field service, maintenance, and small-batch manufacturing often rely on manual tightening with little traceability. Torque Pebble gives builders a way to verify and log fastener quality without buying a bench controller or servo nutrunner.

Useful applications:

- solar rack and battery-pack service
- bicycle and motorcycle maintenance
- electrical lug and busbar verification
- lab fixture assembly
- robotics and CNC service logs
- training technicians on torque-angle tightening technique

## Main SoC Choice

### ESP32-S3-WROOM-1-N8

The ESP32-S3 is a good fit because it provides:

- dual-core MCU headroom for acquisition + UI + wireless stack
- BLE for phone-based work instructions and result export
- Wi-Fi for shop-floor dashboards or MQTT/HTTP upload
- abundant SPI / I2C / ADC / GPIO resources
- low cost and strong toolchain support

## Functional Overview

1. A hex-through or square-drive mechanical body transfers torque through an **instrumented torsion shaft**.
2. Four foil strain gauges form a full Wheatstone bridge on the shaft.
3. An **ADS1232** digitizes the bridge at low noise for torque resolution better than hand-tool repeatability.
4. An **AS5600** reads a diametric magnet on the rotating shaft to measure fastener angle.
5. An **ICM-42688-P** IMU tracks motion and detects back-off / impact / wobble.
6. A **TMP117** provides temperature compensation for bridge gain and zero drift.
7. The ESP32-S3 computes torque-angle features in real time:
   - peak torque
   - snug torque
   - angle after snug
   - prevailing torque
   - torsional stiffness proxy
   - yield-risk index
   - cross-thread suspicion
8. Results are shown locally, stored to microSD, and streamed over BLE/Wi-Fi.

## Block Diagram

```text
          ┌──────────────────── Torque Pebble ────────────────────┐
ratchet ─►│ square-drive input                                     │
          │        │                                                │
          │ instrumented torsion shaft                              │
          │        ├─ foil strain gauge bridge ─► ADS1232 ─┐       │
          │        ├─ diametric magnet ─► AS5600 (I2C) ────┤       │
          │        └─ body motion ─► ICM-42688-P (SPI) ────┤       │
          │                                                 ▼       │
          │                                     ESP32-S3-WROOM-1-N8│
          │                                  - event segmentation  │
          │                                  - torque-angle rules  │
          │                                  - OLED UI             │
          │                                  - BLE/Wi-Fi telemetry │
          │                                  - CSV logging         │
          │                                                         │
          │ TMP117 temp comp ─────── I2C ───────────────────────────┤
          │ DS3231 RTC ───────────── I2C ───────────────────────────┤
          │ SH1106 OLED ──────────── I2C ───────────────────────────┤
          │ microSD ───────────────── SPI ──────────────────────────┤
          │ trigger / mode buttons ─ GPIO                          │
          │ USB-C 5 V / LiPo ─ charger / buck / gauge ─ 3V3 rail   │
          └─────────────────────────────────────────────────────────┘
```

## Full Schematic Design

### Major ICs and Subsystems

| Ref | Part | Function |
|-----|------|----------|
| U1 | ESP32-S3-WROOM-1-N8 | Main SoC, analytics, UI, BLE/Wi-Fi |
| U2 | ADS1232IPWR | 24-bit bridge ADC for torsion strain gauges |
| U3 | INA333AIDGKR | Low-offset instrumentation amplifier / bridge conditioning |
| U4 | AS5600-ASOM | Absolute magnetic angle encoder |
| U5 | ICM-42688-P | 6-axis IMU for back-off / wobble / shock detection |
| U6 | TMP117AIDRVR | Temperature sensor for compensation |
| U7 | DS3231SN | RTC for timestamped audit logs |
| DS1 | SH1106 1.3 in OLED | Local display |
| J1 | microSD push-push socket | Session storage |
| U8 | MCP73831T-2ACI/OT | Single-cell LiPo charger |
| U9 | TPS62172DSGR | 3.3 V buck regulator |
| U10 | MAX17048G+T10 | Battery fuel gauge |
| U11 | TPS22916 | Sensor/load switch |
| B1 | 500–800 mAh LiPo | Portable power |
| SG1-SG4 | 350 Ω foil strain gauges | Full bridge on torsion shaft |
| M1 | diametric magnet, 6 mm | Angle encoder target |
| SW1 | trigger | starts / ends capture |
| SW2 | mode | preset and menu |
| Q1 | AO3400A | buzzer drive |
| LS1 | piezo buzzer | pass/fail feedback |

### Mechanical Measurement Core

The measurement shaft is a short 1/4 in square-drive torsion bar machined from 17-4PH or 4140 steel. Four 350 Ω foil strain gauges are bonded in a +/-45° arrangement to create a full bridge with high sensitivity to torsion and good bending rejection.

Recommended geometry for a compact prototype:

- shaft OD: 8 mm
- reduced gauge section length: 14 mm
- gauge section OD: 6 mm
- nominal full-scale torque: 0–30 N·m
- overload stop: 45 N·m mechanical clutch or shear sleeve

### Bridge Signal Chain

- SG1-SG4 form the Wheatstone bridge excited from 3.0 V analog rail
- **INA333** provides gain of 64 V/V and filters EMI from nearby motors / static discharge
- **ADS1232** digitizes the conditioned bridge output at 80 SPS or 640 SPS test mode
- sense lines use Kelvin routing from the bridge pads
- firmware applies two-point calibration plus temperature coefficient correction

### Angle Measurement

A diametric magnet is bonded to the rotating shaft. **AS5600** sits on the stationary PCB under the magnet and reports 12-bit absolute angle.

- usable angle resolution after filtering: ~0.2°
- wraparound logic permits multi-turn cumulative tightening angle
- the firmware detects direction reversals to catch back-off and rework

### IMU Role

The **ICM-42688-P** is not used for torque measurement; it improves event quality classification.

- detects impulsive hits that can corrupt manual readings
- flags wobble / extension-bar oscillation
- helps identify repeated ratcheting without fastener movement
- allows pose-aware UI if the tool is used upside down or overhead

### Communications and Logging

- I2C bus: AS5600, TMP117, DS3231, OLED, MAX17048
- SPI bus: microSD, ICM-42688-P
- GPIO / bit-bang serial: ADS1232 DOUT/SCLK
- USB-C UART: debug / CSV dump / firmware update

## ESP32-S3 Pin Assignment

| ESP32-S3 Pin | Function |
|--------------|----------|
| GPIO0 | BOOT / service pad |
| GPIO1 | UART0_TX debug |
| GPIO2 | UART0_RX debug |
| GPIO4 | ADS1232_DOUT |
| GPIO5 | ADS1232_SCLK |
| GPIO6 | BUZZER_EN |
| GPIO7 | TRIGGER_BTN |
| GPIO8 | MODE_BTN |
| GPIO9 | I2C_SDA |
| GPIO10 | I2C_SCL |
| GPIO11 | OLED_RESET |
| GPIO12 | SD_CS |
| GPIO13 | SPI_MOSI |
| GPIO14 | SPI_MISO |
| GPIO15 | SPI_SCK |
| GPIO16 | IMU_CS |
| GPIO17 | IMU_INT |
| GPIO18 | SENSOR_EN |
| GPIO19 | BAT_ALERT |
| GPIO20 | CHARGE_STAT |
| GPIO21 | RGB_LED_R |
| GPIO38 | RGB_LED_G |
| GPIO39 | RGB_LED_B |
| GPIO40 | USB sense / wake |
| GPIO41 | ADC battery divider |
|
> Final PCB layout should confirm boot-strapping constraints for the exact ESP32-S3 module revision and chosen USB/JTAG configuration.

## Tightening Analytics

### Event Segmentation

The trigger button arms a capture. A tightening event begins when either:

- torque exceeds 0.15 N·m, or
- cumulative angle changes by more than 1.0° while armed.

The event ends after 400 ms of inactivity or when the operator presses trigger again.

### Derived Metrics

- **peak_torque_nm** — highest absolute torque reached
- **snug_torque_nm** — torque at the knee where slope rises above prevailing torque baseline
- **angle_after_snug_deg** — cumulative angle from snug to final torque
- **prevailing_torque_nm** — average torque before seating
- **stiffness_nm_per_deg** — fitted slope after seating
- **backoff_deg** — reverse rotation after peak
- **yield_risk** — normalized ratio of slope collapse near the end of pull
- **cross_thread_score** — high prevailing torque + low angle progression + noisy motion

### Joint States

1. **PASS** — peak torque inside band and angle-after-snug inside band
2. **UNDER_TORQUE** — final torque below lower limit
3. **OVER_TORQUE** — peak torque above upper limit
4. **SOFT_JOINT** — too much angle after snug for the target torque
5. **CROSS_THREAD_RISK** — excessive prevailing torque with erratic angle growth
6. **REWORK_DETECTED** — back-off beyond threshold before final settle

## Power Architecture

### Input Options

1. **USB-C 5 V** for charging, tethered data, and bench work
2. **Single-cell LiPo** for portable field use

### Rails

- **VBUS_5V**: USB input
- **VBAT**: LiPo pack after charger
- **3V3_MAIN**: ESP32-S3, OLED, microSD, IMU, encoder, RTC
- **3V0_ANALOG**: bridge excitation and INA333 reference network
- **3V3_SENS**: switchable sensor rail via TPS22916

### Estimated Power Budget

| Mode | Current |
|------|---------|
| deep idle | ~1.8 mA |
| armed, display on | ~48 mA |
| logging + BLE | ~78 mA |
| Wi-Fi upload burst | ~210 mA peak |
|
With a 650 mAh LiPo, intermittent field use lasts roughly one workday between charges.

## Calibration Strategy

### Torque Calibration

1. Fixture the body in a bench vise with the output square drive horizontal.
2. Apply known weights on a measured lever arm.
3. Record ADC code at 0, 5, 10, 15, 20, 25, and 30 N·m in both CW and CCW directions.
4. Fit gain, offset, and hysteresis compensation coefficients.
5. Store coefficients in NVS.

### Angle Calibration

1. Align shaft to mechanical zero stop.
2. Read AS5600 raw count.
3. Store offset.
4. Verify linearity every 90° over one full turn.

## Example Assembly Presets

| Preset | Target Torque | Target Angle After Snug | Window |
|--------|---------------|-------------------------|--------|
| M5 stainless cover screw | 4.5 N·m | 35° | +/-0.4 N·m, +/-10° |
| M6 aluminum frame bolt | 9.0 N·m | 55° | +/-0.6 N·m, +/-12° |
| battery lug clamp | 6.0 N·m | 20° | +/-0.5 N·m, +/-8° |
| bike stem faceplate | 5.5 N·m | angle not enforced | +/-0.3 N·m |

## Firmware Layout

```text
firmware/
├── CMakeLists.txt
├── include/
│   ├── torque_pebble.h
│   ├── sensors.h
│   ├── analysis.h
│   └── classifier.h
├── src/
│   ├── main.c
│   ├── sensors.c
│   ├── analysis.c
│   └── classifier.c
└── sdkconfig
```

The included firmware is a portable C reference implementation that simulates several tightening traces and demonstrates the decision logic. Porting it to ESP-IDF mainly requires replacing the stub acquisition layer in `sensors.c` with real drivers.

## Host-Build Demo

```bash
cd firmware
cmake -S . -B build
cmake --build build
./build/torque_pebble_demo
```

## Logged CSV Format

```text
timestamp,preset,peak_torque_nm,snug_torque_nm,angle_after_snug_deg,prevailing_torque_nm,stiffness_nm_per_deg,backoff_deg,yield_risk,cross_thread_score,state,action
```

## Build Notes

- keep the strain-gauge bridge physically close to INA333 inputs
- use shielded twisted pair or flex PCB from shaft gauges to the main board
- mechanically isolate the PCB from bending loads outside the torsion section
- add a rigid overload stop; the electronics cannot prevent mechanical overrange
- conformal-coat the gauge region after calibration if used in the field

## Limitations

- this reference design is for low-to-medium torque hand-tool work, not impact wrenches
- torque accuracy depends heavily on shaft machining, gauge bonding, and calibration discipline
- absolute angle is referenced to the tool, not to bolt stretch or clamp load

## Repository Contents

- `schematic/` — KiCad project and schematic placeholders
- `firmware/` — C analytics demo and build files
- `hardware/BOM.csv` — core bill of materials
- `docs/assembly-guide.md` — mechanical/electrical build guidance
- `docs/api-reference.md` — data model and telemetry contract
- `scripts/analyze_log.py` — CSV summarizer for tightening runs
- `scripts/demo_log.csv` — example output dataset

## Future Extensions

- interchangeable 1/4 in, 3/8 in, and bit-driver mechanical heads
- barcode / QR work-order scan via phone companion app
- torque transducer auto-ID EEPROM on interchangeable heads
- on-device SPC charts and torque drift alarms
- Matter or MQTT bridge for digital manufacturing dashboards
