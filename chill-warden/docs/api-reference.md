# Chill Warden API Reference

## Telemetry Record

Each exported CSV row or JSON object represents one summarized operating window.

| Field | Type | Units | Description |
|------|------|-------|-------------|
| timestamp | string | ISO8601 | Window start time |
| asset_id | string | - | System or unit identifier |
| mode | string | - | cool, heat, freezer, idle |
| suction_c | float | °C | Suction-line temperature |
| discharge_c | float | °C | Discharge-line temperature |
| ambient_c | float | °C | Ambient dry-bulb |
| rh_pct | float | %RH | Relative humidity |
| current_a | float | A | Compressor RMS current proxy |
| vibration_g | float | g RMS | IMU-derived vibration |
| acoustic_db | float | dB proxy | Airborne sound level |
| piezo_db | float | dB proxy | Shell-borne impulse level |
| cycle_period_s | float | s | Time between starts or representative cycle period |
| superheat_proxy_c | float | °C | Derived line-temperature proxy |
| condenser_split_c | float | °C | Discharge minus ambient proxy |
| fault | string | enum | Classifier result |
| risk_score | float | 0-100 | Severity score |
| action | string | - | Recommended technician action |

## Fault Enumeration

- `NORMAL`
- `AIRFLOW_RESTRICTED`
- `LOW_CHARGE_SUSPECT`
- `BEARING_WEAR`
- `LIQUID_SLUGGING`
- `SHORT_CYCLING`
- `DEFROST_ISSUE`

## Service Console Commands

| Command | Description |
|---------|-------------|
| `status` | Print battery, sensor, and storage status |
| `capture 600` | Record a 10-minute window |
| `classify` | Run immediate classification on the latest window |
| `export csv` | Write current log slice to microSD |
| `wifi on` | Enable Wi-Fi upload mode |
| `ble adv` | Advertise live technician telemetry |

## Suggested BLE Characteristics

- live operating state
- fault class
- current temperatures
- vibration RMS
- battery state of charge
- session log transfer
