# Trap Sage API Reference

## CSV Log Fields

| Field | Type | Description |
|-------|------|-------------|
| timestamp | ISO8601 | Route timestamp |
| asset_id | string | User-assigned trap or valve identifier |
| upstream_temp_c | float | Upstream pipe-wall temperature |
| downstream_temp_c | float | Downstream pipe-wall temperature |
| ambient_temp_c | float | Ambient air temperature |
| clamp_force_n | float | Estimated clamp contact force |
| ultrasonic_rms | float | Envelope RMS from contact ultrasonic channel |
| crest_factor | float | Peak / RMS ratio |
| burstiness | float | Normalized standard deviation / mean |
| active_fraction | float | Fraction of time above activity threshold |
| cycle_rate_cpm | float | Estimated discharge cycle rate |
| health_score | float | 0-100 derived health metric |
| classification | enum | Fault class |

## BLE Characteristic Sketch

### Service: `0x73A0` Trap Sage Diagnostics

| Characteristic | UUID | Access | Purpose |
|----------------|------|--------|---------|
| Live summary | `0x73A1` | Notify | Packed 32-byte status frame |
| Asset label | `0x73A2` | Read/Write | Current route asset name |
| Start capture | `0x73A3` | Write | Trigger a 10 s measurement |
| Route export | `0x73A4` | Read | Chunked CSV transfer |

## Wi-Fi JSON Payload

```json
{
  "asset_id": "BLDG-A-TRAP-07",
  "classification": "NORMAL_CYCLING",
  "upstream_temp_c": 158.0,
  "downstream_temp_c": 93.0,
  "ultrasonic_rms": 0.357,
  "burstiness": 0.91,
  "cycle_rate_cpm": 6.7,
  "health_score": 71.8
}
```

## Fault Logic Summary

- low force -> `SENSOR_MISPLACED`
- low-temperature, high-noise signatures -> `CAVITATING_VALVE`
- high continuous noise + small ΔT -> `BLOW_THROUGH`
- almost silent + large ΔT + cold outlet -> `COLD_BLOCKED`
- bursty intermittent discharge + healthy ΔT -> `NORMAL_CYCLING`
- moderate continuous activity + middling ΔT -> `FLOODING`
