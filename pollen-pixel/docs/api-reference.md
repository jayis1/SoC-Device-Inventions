# Pollen Pixel API Reference

## Serial / BLE Command Set

All commands are ASCII lines terminated by `\n`.

### `STATUS`
Returns current operating state, battery level, tape position, and last sample summary.

### `SAMPLE <seconds>`
Starts a sampling interval using the current blower target and then captures/analyses the next tape patch.

Example:

```text
SAMPLE 120
OK sampling=120
```

### `ADVANCE <steps>`
Advances the adhesive tape manually for service or inspection.

### `FLOW?`
Returns measured flow in L/min and the applied calibration coefficients.

### `LIGHT <WHITE|UV> <0-255>`
Sets illumination PWM for service mode.

### `EXPORT LAST`
Returns the most recent sample record as CSV.

Example response:

```text
timestamp,flow_lpm,sample_seconds,particle_count,grains_m3,allergen_index,tree,grass,weed,spore,dust
2026-08-22T08:15:00Z,1.82,120,17,624.5,78.2,5,3,4,2,3
```

## Sample Record Fields

| Field | Meaning |
|-------|---------|
| `timestamp` | UTC timestamp from RTC |
| `flow_lpm` | Mean flow during capture |
| `sample_seconds` | Pumping duration |
| `particle_count` | Segmented objects in analyzed patch |
| `grains_m3` | Estimated concentration |
| `allergen_index` | 0–100 composite risk score |
| `tree/grass/weed/spore/dust` | Per-class counts |

## BLE GATT Sketch

- Service UUID: `7b460000-8cb1-4f89-a4d6-0a2f57f00001`
- Command characteristic: write / write-without-response
- Telemetry characteristic: notify
- Image-preview characteristic: read blob chunks for current ROI

## Suggested Web Dashboard Panels

- live grains/m³ gauge
- stacked bar chart by class
- hourly allergen index trend
- flow stability and blower duty
- camera flat-field / focus preview
