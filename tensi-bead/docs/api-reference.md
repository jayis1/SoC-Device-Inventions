# Tensi Bead API Reference

## BLE GATT sketch

### Service: `8d17a600-719d-4f31-9276-9f5222a10000`

- `...0001` Measurement control (write)
- `...0002` Live result packet (notify)
- `...0003` Calibration state (read/notify)
- `...0004` CSV export chunk (notify)

### Measurement control payload

```json
{
  "mode": "pendant|sessile",
  "dose_uL": 4.5,
  "frame_average": 8,
  "temperature_comp": true,
  "sample_name": "rinse-water-a"
}
```

### Live result payload

```json
{
  "sample_id": 17,
  "mode": "pendant",
  "surface_tension_mn_m": 71.6,
  "contact_angle_deg": 0.0,
  "bond_number": 1.48,
  "spread_index": 0.0,
  "classification": "high-surface-tension / water-like",
  "qc_ok": true,
  "ambient_temp_c": 23.8,
  "ambient_rh_pct": 44.2
}
```

## Wi-Fi endpoints

- `GET /api/v1/status`
- `POST /api/v1/capture`
- `GET /api/v1/captures.csv`
- `POST /api/v1/calibrate`

## CSV log columns

- `timestamp_iso8601`
- `sample_id`
- `mode`
- `surface_tension_mn_m`
- `contact_angle_deg`
- `bond_number`
- `spread_index`
- `classification`
- `qc_ok`
- `ambient_temp_c`
- `ambient_rh_pct`
- `illumination_lux`
- `dose_mass_mg`

## UART manufacturing test commands

- `PING`
- `CAM SNAP`
- `PUMP STEP <n>`
- `LED <0..255>`
- `LOADCELL TARE`
- `RUN DEMO`
