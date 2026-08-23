# Stoma Clip API Reference

## Measurement Record

```c
typedef struct {
    float chamber_temp_c[18];
    float chamber_rh_pct[18];
    float ambient_temp_c;
    float ambient_rh_pct;
    float leaf_temp_c;
    float light_lux_proxy;
    float battery_v;
    float blower_current_a;
    float chamber_volume_cm3;
    float leaf_area_cm2;
    float sample_dt_s;
    size_t count;
} sc_capture_t;
```

## Result Record

```c
typedef struct {
    float vpd_kpa;
    float vapor_slope_g_m3_s;
    float transpiration_g_m2_s;
    float stomatal_conductance_mol_m2_s;
    float stress_score;
    float fit_r2;
    float leaf_air_delta_c;
    sc_condition_t condition;
} sc_result_t;
```

## Functions

### `void sc_make_demo_capture(sc_capture_t *capture)`
Creates a deterministic sealed-chamber sample for desktop testing.

### `void sc_compute_result(const sc_capture_t *capture, sc_result_t *result)`
Runs the porometer math pipeline:

1. RH/T -> vapor density
2. linear slope fit over the closed interval
3. transpiration proxy estimation
4. VPD computation
5. stomatal conductance computation
6. stress scoring and bucket assignment

### `void sc_print_report(const sc_capture_t *capture, const sc_result_t *result)`
Prints a console report for the simulator.

## CSV Log Schema

```text
timestamp,leaf_temp_c,ambient_temp_c,ambient_rh_pct,vpd_kpa,g_sw_mol_m2_s,transpiration_g_m2_s,stress_score,condition,fit_r2,light_lux_proxy,battery_v
```

## BLE Characteristic Sketch

| UUID Suffix | Direction | Payload |
|-------------|-----------|---------|
| 0x1001 | notify | live chamber RH/T samples |
| 0x1002 | notify | computed result packet |
| 0x1003 | write | start measurement |
| 0x1004 | read/write | calibration coefficients |
| 0x1005 | read | battery and health |

## REST / Wi-Fi Sketch

- `POST /api/v1/measure/start`
- `GET /api/v1/measure/latest`
- `GET /api/v1/log/export.csv`
- `POST /api/v1/calibration`

## Fault Codes

| Code | Meaning |
|------|---------|
| 1 | leaf missing |
| 2 | chamber failed to seal |
| 3 | blower stall / overcurrent |
| 4 | low battery |
| 5 | poor linear fit |
| 6 | humidity sensor mismatch |
