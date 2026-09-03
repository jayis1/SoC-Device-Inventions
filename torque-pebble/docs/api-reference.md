# Torque Pebble API Reference

## Session Record

Each completed tightening event is stored as one CSV row and can also be serialized as JSON.

### JSON schema

```json
{
  "timestamp": "2026-09-03T02:15:14Z",
  "preset": "m6_frame_bolt",
  "peak_torque_nm": 9.42,
  "snug_torque_nm": 3.11,
  "angle_after_snug_deg": 58.4,
  "prevailing_torque_nm": 0.64,
  "stiffness_nm_per_deg": 0.112,
  "backoff_deg": 1.2,
  "yield_risk": 0.18,
  "cross_thread_score": 0.07,
  "state": "PASS",
  "action": "Accept joint"
}
```

## Fields

| Field | Type | Description |
|-------|------|-------------|
| `timestamp` | string | UTC ISO-8601 event completion time |
| `preset` | string | active job recipe |
| `peak_torque_nm` | float | maximum measured torque magnitude |
| `snug_torque_nm` | float | detected seating torque |
| `angle_after_snug_deg` | float | angle between snug point and final pull |
| `prevailing_torque_nm` | float | early-turn friction torque before seating |
| `stiffness_nm_per_deg` | float | post-snug slope estimate |
| `backoff_deg` | float | reverse motion after peak |
| `yield_risk` | float | 0.0-1.0 normalized slope-collapse indicator |
| `cross_thread_score` | float | 0.0-1.0 assembly-damage suspicion score |
| `state` | enum | `PASS`, `UNDER_TORQUE`, `OVER_TORQUE`, `SOFT_JOINT`, `CROSS_THREAD_RISK`, `REWORK_DETECTED` |
| `action` | string | operator recommendation |

## BLE Characteristics

Suggested custom service UUID base: `7b0d4000-4a11-4c2f-a4f7-545150424c45`

| Characteristic | UUID suffix | Direction | Payload |
|----------------|-------------|-----------|---------|
| Live summary | `0001` | notify | compact CSV line |
| Preset select | `0002` | write | preset name string |
| Start capture | `0003` | write | `0` or `1` |
| Device status | `0004` | read/notify | battery, storage, temp, wireless state |
| Last record JSON | `0005` | read | UTF-8 JSON blob |

## MQTT Topics

| Topic | Direction | Payload |
|-------|-----------|---------|
| `torque-pebble/<id>/status` | pub | online state, battery, firmware version |
| `torque-pebble/<id>/event` | pub | event JSON |
| `torque-pebble/<id>/preset/set` | sub | preset name |
| `torque-pebble/<id>/cmd/capture` | sub | `start`, `stop`, `clear` |

## Error Flags

- `bridge_saturated`
- `angle_sensor_fault`
- `imu_fault`
- `sd_write_failed`
- `battery_low`
- `overrange_event`
- `calibration_missing`
