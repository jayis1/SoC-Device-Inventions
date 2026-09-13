# API reference

## `GET /api/v1/status`

Returns the last completed sample. It intentionally returns `null` before a valid sample.

```json
{"profile":"wheat","moisture_wb_pct":13.4,"bulk_density_kg_m3":782.1,"test_weight_kg_hl":78.2,"grain_temp_c":24.1,"ambient_rh_pct":52.0,"insect_impulses_min":2,"storage_class":"AERATE","safe_days":35,"confidence_pct":91}
```

`storage_class` is one of `DRY`, `CONDITION`, `AERATE`, `URGENT DRY`, or `INVALID`.

## `POST /api/v1/measure`

Starts one acquisition. The cup must already be filled and leveled. Response: `202 {"state":"measuring"}`. A completed result is emitted through the BLE notification characteristic and appended to `/kernel-scout.csv`.

## `POST /api/v1/profile`

Accepts a validated local profile. Coefficients must be produced from known oven-reference samples; the device refuses a profile whose mass limits are outside 0–500 g.

```json
{"name":"wheat","a0":8.3,"a1":42.0,"a2":-4.2,"a3":-0.015,"baseline_mag":1000,"min_mass_g":80,"max_mass_g":108}
```

The reference implementation does not expose the web server itself; its model and payload field names are defined in `firmware/`.
