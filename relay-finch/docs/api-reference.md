# Relay Finch API Reference

## UART Record From STM32 to ESP32-C3

Each completed test is serialized as one JSON line.

```json
{
  "timestamp": "2026-09-04T02:20:00Z",
  "preset": "plc_relay_24v",
  "mode": "pull_in_sweep",
  "pull_in_v": 18.8,
  "drop_out_v": 5.4,
  "operate_ms": 8.2,
  "release_ms": 5.6,
  "bounce_ms": 1.9,
  "chatter_edges": 2,
  "pickup_ma": 84.0,
  "hold_ma": 62.0,
  "coil_r_ohm": 285.7,
  "travel_mm": 1.46,
  "state": "PASS",
  "action": "log_and_return_to_service"
}
```

## CSV Export Format

```text
timestamp,preset,time_ms,coil_v,current_ma,contact_closed,hall_mm,acoustic
```

### Fields

- `timestamp`: ISO8601 event timestamp
- `preset`: preset name used for interpretation
- `time_ms`: milliseconds from test start
- `coil_v`: applied DUT coil voltage
- `current_ma`: measured coil current in milliamps
- `contact_closed`: `0` or `1`
- `hall_mm`: armature-travel estimate in millimeters
- `acoustic`: normalized envelope amplitude from 0.0 to 1.0

## BLE Characteristics

### Service UUID

`7f2b0001-59d6-48ab-a1f8-4a6cb80d5100`

### Characteristics

- `7f2b0002-59d6-48ab-a1f8-4a6cb80d5100` — latest result JSON, notify/read
- `7f2b0003-59d6-48ab-a1f8-4a6cb80d5100` — start test command, write
- `7f2b0004-59d6-48ab-a1f8-4a6cb80d5100` — preset blob, read/write
- `7f2b0005-59d6-48ab-a1f8-4a6cb80d5100` — waveform stream chunks, notify

## HTTP Endpoints via ESP32-C3

### `GET /api/v1/latest`
Returns the most recent result JSON.

### `GET /api/v1/presets`
Returns stored presets.

### `POST /api/v1/test`
Starts a test.

Request body:

```json
{
  "preset": "automotive_relay_12v",
  "mode": "bounce_capture"
}
```

### `POST /api/v1/presets`
Stores or updates a preset.

## Preset Structure

```json
{
  "name": "automotive_relay_12v",
  "nominal_voltage": 12.0,
  "max_pull_in_v": 9.5,
  "min_drop_out_v": 1.5,
  "max_drop_out_v": 5.0,
  "max_operate_ms": 12.0,
  "max_bounce_ms": 3.0,
  "min_travel_mm": 1.1,
  "min_coil_resistance_ohm": 55.0,
  "release_timeout_ms": 20.0
}
```
