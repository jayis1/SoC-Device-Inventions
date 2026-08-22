# Pipe Halo API Reference

## Serial Console Summary Line

The demo/reference firmware prints one summary line per analysis window:

```text
2026-08-22T00:00:00Z,state=DRIP_LEAK,flow=0.302,hammer=0.124,drip=0.910,freeze_margin=6.100,burst=0.211,action=Inspect nearby fixture or trap for periodic drip.
```

## JSON Telemetry Schema

```json
{
  "device": "pipe-halo",
  "timestamp": "2026-08-22T00:00:00Z",
  "pipe_temp_c": [8.4, 6.6],
  "ambient_temp_c": 11.2,
  "humidity_rh": 74.0,
  "acoustic_rms_mv": 42.0,
  "spectral_centroid_hz": 260.0,
  "vibration_rms_g": 0.028,
  "drip_period_s": 0.92,
  "valve_angle_deg": 87.0,
  "flow_index": 0.302,
  "hammer_index": 0.124,
  "drip_confidence": 0.910,
  "freeze_margin_c": 6.1,
  "burst_risk_score": 0.211,
  "state": "DRIP_LEAK",
  "recommended_action": "Inspect nearby fixture or trap for periodic drip."
}
```

## State Enum

| State | Meaning |
|-------|---------|
| `IDLE` | No significant flow or hazard detected |
| `NORMAL_FLOW` | Healthy flow observed |
| `DRIP_LEAK` | Periodic low-energy leak signature |
| `WATER_HAMMER` | Impulsive plumbing shock event |
| `FREEZE_RISK` | Pipe wall near freezing with low flow |
| `BURST_RISK` | Elevated combined damage risk |
| `VALVE_TAMPER` | Valve position diverges from commissioned open state |

## Suggested BLE GATT Layout

- Service UUID: `7c42f200-8b3a-4c75-a3bb-0d7d7ef10001`
- Characteristic 1: live summary JSON (notify)
- Characteristic 2: calibration command RX (write)
- Characteristic 3: historical log export chunk (read/notify)
- Characteristic 4: valve angle / freeze alarm thresholds (read/write)

## Suggested MQTT Topics

- `pipe-halo/<device-id>/state`
- `pipe-halo/<device-id>/telemetry`
- `pipe-halo/<device-id>/alarm`
- `pipe-halo/<device-id>/cmd`

## Commands

| Command | Payload | Effect |
|---------|---------|--------|
| `ack_alarm` | none | silences buzzer until next fault |
| `set_freeze_margin` | float °C | changes warning threshold |
| `capture_baseline` | mode string | stores zero-flow / valve-open baseline |
| `export_log` | date range | streams CSV/JSON history |
