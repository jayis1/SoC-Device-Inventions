# API Reference — Sump Shepherd

## Telemetry Topic Layout

Suggested MQTT topics:

```text
sump-shepherd/<device_id>/telemetry
sump-shepherd/<device_id>/events
sump-shepherd/<device_id>/status
sump-shepherd/<device_id>/command
```

## Telemetry Payload

```json
{
  "device_id": "sump-shepherd-01",
  "ts": "2026-08-31T02:15:00Z",
  "level_mm": 462.0,
  "rise_rate_mm_min": 14.2,
  "drawdown_rate_mm_min": -88.0,
  "pump_current_a": 7.1,
  "vibration_g_rms": 0.22,
  "humidity_rh": 81.4,
  "temperature_c": 19.8,
  "float_active": true,
  "leak_active": false,
  "battery_v": 4.01,
  "power_fail": false,
  "state": "HEALTHY",
  "fault_score": 8.0,
  "flood_risk_score": 34.0,
  "cycle_health_score": 92.0
}
```

## Event Types

- `healthy_cycle`
- `no_start`
- `backflow_detected`
- `short_cycle_detected`
- `dry_run_detected`
- `slow_pump_detected`
- `leak_confirmed`
- `humidity_alert`
- `battery_low`
- `power_fail`

## Commands

### `ack_alarm`
Silence buzzer until the next fault transition.

### `capture_baseline`
Store the next complete pump cycle as the comparison baseline.

### `set_thresholds`
Update alarm level, leak debounce time, and humidity thresholds.

Example:

```json
{
  "cmd": "set_thresholds",
  "alarm_level_mm": 500,
  "humidity_alert_rh": 82,
  "short_cycle_rebound_mm": 40
}
```

### `reboot`
Restart the node.

## HTTP Endpoints

Suggested local endpoints:

- `GET /api/v1/status`
- `GET /api/v1/logs/latest`
- `POST /api/v1/command`
- `POST /api/v1/baseline`

## CSV Log Columns

```text
timestamp,level_mm,rise_rate_mm_min,drawdown_rate_mm_min,pump_current_a,vibration_g_rms,humidity_rh,float_active,leak_active,battery_v,state
```
