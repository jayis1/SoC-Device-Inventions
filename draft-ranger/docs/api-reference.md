# Draft Ranger API Reference

## Telemetry Record

Each measurement frame can be serialized as JSON or CSV.

### JSON Shape

```json
{
  "timestamp": "2026-08-24T12:00:00Z",
  "mode": "spot",
  "pressure_pa": 22.4,
  "bridge_v": 0.842,
  "draft_velocity_mps": 1.86,
  "ambient_temp_c": 21.7,
  "ambient_rh_pct": 48.2,
  "frame_temp_c": 13.4,
  "surface_temp_c": 11.9,
  "dew_point_c": 10.4,
  "condensation_margin_c": 1.5,
  "whistle_hz": 1800,
  "whistle_score": 0.63,
  "leak_severity": 58.0,
  "seal_health": 41.0,
  "fault_class": "PRESSURE_DRIVEN_DRAFT"
}
```

## BLE Characteristics

### Service UUID
`7b1b2000-4f6d-4b50-8f29-8d70fd77a100`

### Characteristics

- `...a101` live measurement frame (notify)
- `...a102` configuration / calibration (read-write)
- `...a103` mark event / annotation (write)
- `...a104` device status (read-notify)

## Wi-Fi Endpoints

### `GET /api/v1/status`
Returns battery, storage, and current mode.

### `GET /api/v1/last`
Returns most recent measurement frame.

### `POST /api/v1/mark`
Adds a user annotation to the current scan.

### `POST /api/v1/calibration/zero`
Zeros pressure and hot-wire offsets.

## CSV Columns

```text
timestamp,mode,pressure_pa,bridge_v,draft_velocity_mps,ambient_temp_c,ambient_rh_pct,frame_temp_c,surface_temp_c,dew_point_c,condensation_margin_c,whistle_hz,whistle_score,leak_severity,seal_health,fault_class
```

## Fault Class Enumeration

- `SEAL_OK`
- `MINOR_WEATHERSTRIP_GAP`
- `PRESSURE_DRIVEN_DRAFT`
- `WARPED_FRAME_OR_SASH`
- `CONDENSATION_RISK`
- `CRITICAL_COMBINED`

## Calibration Parameters

```text
pressure_zero_pa
bridge_zero_v
kings_A
kings_B
kings_n
frame_offset_c
surface_offset_c
whistle_floor
```

## Log Rate

- Spot mode: 2 Hz summaries
- Sweep mode: 10 Hz summaries + optional 16 kHz audio snippets
- Condensation mode: 1 Hz

## Error Conditions

- `ERR_PRESSURE_STUCK`
- `ERR_PROBE_OVERTEMP`
- `ERR_SENSOR_MISSING`
- `ERR_SD_WRITE`
- `ERR_LOW_BATTERY`
