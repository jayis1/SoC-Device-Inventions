# Seal Whisper — Portable Package Leak and Seal-Integrity Analyzer

> A bench-top-sized field instrument that evacuates a small test chamber, measures pressure-decay and package acoustic ring-down, and produces a traceable pass / investigate result for pouches, blister packs, sachets, and capped vials.

## Purpose

Small food, diagnostic, and packaging teams often discover weak seals only after a shelf-life or dye-ingress test. **Seal Whisper** provides a non-destructive screen before those tests: it runs a controlled vacuum profile around a package and combines compensated pressure decay with a piezo-excited acoustic resonance measurement. A pinhole, incomplete heat seal, or loose cap causes a repeatable change in either channel.

It is a screening and process-control instrument, **not** a certified replacement for ASTM F2338, ASTM F2054, ISO 11607 validation, a microbial barrier test, or a pharmaceutical release test.

## Architecture

**ESP32-S3-WROOM-1-N8** is the sole SoC. It acquires the 24-bit pressure ADC and acoustic envelope, controls the proportional vacuum valve/pump, logs to microSD, and exposes BLE/Wi-Fi telemetry.

```text
USB-C -> charger -> LiPo -> TPS63070 3V3
                                  |
Test chamber -> SDP816-500Pa -> ADS122C04 -- I2C --\
Chamber wall -> TMP117 ----------------------- I2C ---+-> ESP32-S3 -> OLED / microSD / BLE / Wi-Fi
Piezo drive <- DRV2700 <- PWM ------------------------|
Piezo pickup -> OPA320 bandpass -> ADS122C04 ---------|
Vacuum pump <- AO3400A <- GPIO; vent valve <- AO3400A <- GPIO
```

## Measurement principle

1. The pump lowers chamber pressure to the selected setpoint (typically -35 kPa gauge), then closes an upstream check valve.
2. After a 2 s settling period, the device fits pressure over a 10 s window. Temperature normalization uses the ideal-gas first-order term:

```text
leak_pa_s = slope(P, t) - P_mean * slope(T_K, t) / T_mean_K
```

3. A 2.8 kHz, 20 ms burst excites a bonded piezo disc. The pickup envelope is sampled at 2 kS/s. The log-envelope slope estimates ring-down time `tau_ms`; a gross opening or loose cap usually changes pneumatic damping.
4. A reference package establishes median leak and tau. A sample is flagged when either robust z-score exceeds its profile limit. The pressure result is primary; acoustic data increases confidence and helps localize intermittent seal faults.

The test chamber must be intrinsically tighter than the smallest package leak of interest. Run an empty-chamber blank before every lot.

## Design details

### Pneumatic path

- **J1** is a 4 mm push-to-connect bulkhead feeding a 250 mL polycarbonate chamber with a silicone gasket.
- **P1** is a 5 V diaphragm vacuum pump. A check valve between P1 and the chamber isolates pump leakage.
- **V1** is a normally-closed 3.3 V miniature vent valve, fitted with a sintered intake filter.
- **U2 SDP816-500Pa** measures a 0–500 Pa differential range across a 100:1 calibrated pneumatic divider. The divider converts the -35 kPa chamber range into the sensor range; characterize its ratio against a traceable gauge. Use a second, direct **MPRLS0025PA** absolute sensor for safety/setpoint control.
- A 45 kPa mechanical relief valve protects thin packages; software never commands below -45 kPa gauge.

### Acoustic path

PZ1 is bonded to a removable aluminum chamber platen. PZ2 is a pickup disc on the opposing wall. U5 (DRV2700) drives PZ1 through a 1:10 transformer at <= 30 Vpp. U6 (OPA320) is a 2.0–4.5 kHz band-pass gain stage centered at 1.65 V. U4 (ADS122C04) digitizes pressure and the acoustic envelope with separate configurations. Keep PZ drive return away from the analog ground island.

### Power and protection

```text
USB-C VBUS -> 500 mA PTC -> TVS -> MCP73831 -> protected 1200 mAh LiPo
LiPo -> TPS63070 -> 3V3_DIG -> ESP32-S3, OLED, SD
                  -> ferrite -> 3V3_A -> sensors, ADC, OPA320
LiPo -> boost 5V -> pump; Q1 flyback diode clamp
```

The piezo boost is enabled only during a burst. Add 5.1 kΩ CC1/CC2 sink resistors, ESD diodes at USB-C, a 100 nF / 10 uF local decoupling pair at every IC, and a copper keep-out below the ESP32 antenna.

### Pin assignment

| ESP32-S3 GPIO | Net | Function |
|---:|---|---|
| 1 / 2 | I2C_SDA / I2C_SCL | ADS122C04, MPRLS0025PA, TMP117, MAX17048, OLED |
| 4 | PUMP_EN | AO3400A pump switch |
| 5 | VENT_EN | AO3400A vent-valve switch |
| 6 | PIEZO_PWM | DRV2700 burst reference |
| 7 | ADC_DRDY | ADS122C04 ready |
| 10 / 11 / 12 | SD_SCK / SD_MOSI / SD_CS | microSD SPI |
| 13 | BTN_TEST | start/abort test |
| 14 | BTN_PROFILE | profile select |
| 15 | BUZZER_EN | haptic/buzzer driver |
| 16 / 17 | UART_RX / UART_TX | service header |
| 18 | DOOR_INTERLOCK | lid reed switch, low = safe |
| 21 | VBAT_ADC | 1:2 battery divider |

## Firmware and API

The `firmware/` reference implementation is portable C11. It contains deterministic leak compensation, least-squares fitting, and pass logic. Hardware functions are represented by an input frame so the core can be unit-tested on a host.

```bash
cmake -S firmware -B firmware/build
cmake --build firmware/build
./firmware/build/seal_whisper_demo
```

Example record:

```json
{"profile":"sachet-250ml","pressure_pa_s":8.14,"ringdown_ms":24.8,"pressure_z":1.03,"acoustic_z":0.55,"result":"PASS","confidence_pct":89}
```

See [docs/API.md](docs/API.md) and [docs/ASSEMBLY.md](docs/ASSEMBLY.md). `scripts/analyze_run.py` verifies and plots CSV captures.

## Operating procedure

1. Inspect and clean the gasket; run the empty-chamber blank. Reject a blank above the profile's chamber-leak limit.
2. Place one dry, room-temperature package on the platen without obstructing the port. Close the lid until the interlock shows closed.
3. Select a package profile and press **TEST**. The run is about 15 seconds.
4. Save the record only after checking the displayed setpoint, pressure trace, and confidence. Retest an **INVESTIGATE** result twice after rotating the package.
5. Confirm any failed lot with the applicable validated method and quarantine rules.

## Safety and limitations

- The chamber is a vacuum vessel: use 6 mm polycarbonate or rated metal, a shield, pressure relief, and a lid interlock. Do not use cracked acrylic.
- Never test pressurized, reactive, flammable, sharp, or biologically hazardous packages.
- The default profiles are development starting points, not production acceptance limits. Establish limits from known-good and intentionally defective packages.
- Do not interpret a pass as sterility, shelf-life, or regulatory compliance.

## Files

```text
seal-whisper/
├── schematic/seal-whisper.kicad_sch
├── hardware/BOM.csv
├── firmware/                 # C11 reference model and CMake build
├── docs/ASSEMBLY.md
├── docs/API.md
└── scripts/analyze_run.py
```

## License

MIT.
