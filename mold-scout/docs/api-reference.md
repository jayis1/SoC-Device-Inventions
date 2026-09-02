# Mold Scout API Reference

## Overview

Mold Scout can export spot scans and sweep summaries over BLE UART, USB CDC, or Wi-Fi HTTP/MQTT bridges.

## Telemetry JSON

```json
{
  "device": "mold-scout",
  "timestamp": "2026-09-02T02:14:00Z",
  "ambient_temp_c": 23.4,
  "ambient_rh": 58.2,
  "surface_temp_c": 18.9,
  "thermal_delta_c": -3.7,
  "dewpoint_margin_c": -0.8,
  "impedance_kohm": 31.2,
  "phase_deg": -18.4,
  "voc_index": 182.0,
  "gas_resistance_kohm": 18.7,
  "accel_motion_g": 0.05,
  "battery_v": 3.88,
  "moisture_proxy": 0.74,
  "condensation_risk": 0.91,
  "thermal_bridge_score": 0.63,
  "voc_risk": 0.52,
  "mold_risk_score": 0.83,
  "state": "ACTIVE_MOLD_RISK",
  "recommended_action": "Open the cavity or keep drying equipment deployed; rescan in 24 h."
}
```

## BLE UART Framing

One JSON object per line, UTF-8, `\n` terminated.

## Commands

| Command | Description |
|---------|-------------|
| `SCAN:SPOT` | take one fused spot scan |
| `SCAN:SWEEP` | begin sweep mode until stopped |
| `BASELINE:STORE` | store current area as room baseline |
| `BASELINE:CLEAR` | clear stored baseline |
| `LOG:MARK` | place a bookmark in the current session log |
| `WIFI:STATUS` | return network and upload status |
| `SELFTEST` | run sensor presence and calibration checks |

## HTTP Endpoints

| Method | Path | Description |
|--------|------|-------------|
| GET | `/api/v1/status` | device state, battery, Wi-Fi, last reading |
| POST | `/api/v1/scan/spot` | trigger spot scan and return JSON result |
| POST | `/api/v1/baseline` | store current baseline |
| GET | `/api/v1/log/latest` | fetch latest CSV/JSON summary |

## State Strings

- `NORMAL`
- `CONDENSATION_RISK`
- `HIDDEN_MOISTURE`
- `ACTIVE_MOLD_RISK`
- `WET_BUILDING_MATERIAL`

## CSV Export Fields

```text
timestamp,ambient_temp_c,ambient_rh,surface_temp_c,thermal_delta_c,impedance_kohm,phase_deg,voc_index,gas_resistance_kohm,accel_motion_g,battery_v,moisture_proxy,condensation_risk,thermal_bridge_score,voc_risk,mold_risk_score,state
```
