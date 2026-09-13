# API reference

## Status endpoint

When joined to Wi-Fi, `GET /api/v1/status` returns device state and the most recent record:

```json
{"state":"complete","profile":"sachet-250ml","setpoint_kpa_g":-35.0,"pressure_pa_s":8.14,"ringdown_ms":24.8,"pressure_z":1.03,"acoustic_z":0.55,"result":"PASS","confidence_pct":89}
```

`result` is `PASS`, `INVESTIGATE`, `ABORTED`, or `INVALID_BLANK`. A valid test must have a closed interlock, stable setpoint, and a blank below the profile limit.

## Commands

| Method/path | JSON body | Meaning |
|---|---|---|
| `POST /api/v1/test` | `{"profile":"sachet-250ml"}` | Starts a run if safe. |
| `POST /api/v1/vent` | `{}` | Opens the vent and disables pump. |
| `POST /api/v1/blank` | `{}` | Runs empty-chamber blank qualification. |
| `GET /api/v1/runs.csv` | none | Streams CSV records. |

CSV columns are `utc,serial,profile,setpoint_kpa_g,pressure_pa_s,ringdown_ms,pressure_z,acoustic_z,result,confidence_pct`. Firmware must fail safe: on watchdog reset, interlock open, out-of-range absolute pressure, or I2C fault, it disables the pump and opens the vent.
