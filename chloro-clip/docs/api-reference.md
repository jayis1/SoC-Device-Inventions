# Chloro Clip API Reference

## Telemetry Record

Each summarized measurement can be serialized as JSON or CSV.

### JSON Shape

```json
{
  "timestamp": "2026-08-26T12:00:00Z",
  "leaf_id": "oak-healthy-01",
  "mode": "dark_adapted",
  "leaf_temp_c": 28.4,
  "ambient_temp_c": 27.1,
  "ambient_par_umol": 780,
  "fo": 118.0,
  "fm": 781.0,
  "fv_fm": 0.849,
  "fs": 428.0,
  "fm_prime": 671.0,
  "fo_prime": 156.0,
  "phi_psii": 0.362,
  "npq": 0.164,
  "qp": 0.472,
  "stress_index": 13.8,
  "diagnosis": "HEALTHY"
}
```

## BLE Characteristics

### Service UUID
`7b1b3000-4f6d-4b50-8f29-8d70fd77a100`

### Characteristics

- `...a101` live summarized measurement (notify)
- `...a102` raw curve frame (notify)
- `...a103` configuration / calibration (read-write)
- `...a104` device status (read-notify)

## Wi-Fi Endpoints

### `GET /api/v1/status`
Returns battery, storage, optics temperature, and mode.

### `GET /api/v1/last`
Returns the most recent processed measurement.

### `POST /api/v1/measure`
Starts a new measurement in `dark_adapted`, `light_adapted`, or `rapid` mode.

### `POST /api/v1/calibration/dark-zero`
Captures offset with clip closed and no leaf.

## CSV Columns

```text
timestamp,leaf_id,mode,leaf_temp_c,ambient_temp_c,ambient_par_umol,fo,fm,fv_fm,fs,fm_prime,fo_prime,phi_psii,npq,qp,stress_index,diagnosis
```

## Diagnosis Enumeration

- `HEALTHY`
- `MILD_DROUGHT_STRESS`
- `HEAT_STRESS`
- `PHOTOINHIBITION`
- `NITROGEN_DEFICIENCY`
- `CLAMP_ERROR`

## Configuration Parameters

```text
measure_led_dac
flash_pulse_ms
flash_current_limit_ma
far_red_pulse_ms
dark_offset_counts
tia_gain_profile
species_profile
light_adapt_wait_ms
```

## Error Conditions

- `ERR_CLIP_OPEN`
- `ERR_ADC_SATURATION`
- `ERR_FLASH_UNDERVOLTAGE`
- `ERR_OPTICAL_LEAK`
- `ERR_SD_WRITE`
- `ERR_LOW_BATTERY`
