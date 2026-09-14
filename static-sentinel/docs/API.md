# Static Sentinel local API reference

Protocol version: `1`. The reference core has no network server. A later ESP-IDF integration may expose these records over serial at 115200 8N1; it must not enable a network listener without an authenticated design review.

## Measurement record

```json
{"version":1,"mat_ohms":1000000,"strap_ohms":1200000,"humidity_pct":41.0,"interlock_closed":true,"status":"OK"}
```

Fields:

| Field | Type / range | Meaning |
|---|---|---|
| `version` | integer `1` | framing version |
| `mat_ohms` | finite >= 0 | calibrated mat path estimate |
| `strap_ohms` | finite >= 0 | calibrated wrist-strap path estimate |
| `humidity_pct` | 0–100 | SHT45 relative humidity context |
| `interlock_closed` | boolean | enclosure/terminal condition; false invalidates a test |
| `status` | see below | policy result |

## Status values

| Status | Meaning | Safe user action |
|---|---|---|
| `OK` | all configured policy checks passed | begin work only under the site’s full procedure |
| `WARN_HUMIDITY` | low humidity context | follow local ESD mitigation policy |
| `FAIL_MAT` | mat estimate outside configured band | inspect/replace mat path; retest |
| `FAIL_STRAP` | strap estimate outside configured band | inspect strap/cord/skin contact; retest |
| `INVALID_SAMPLE` | open interlock, invalid ADC-derived data, or bounds failure | do not infer a passing condition |

The default policy in the host example is mat 0.75 Mohm–1 Gohm, strap 0.75–35 Mohm, and humidity warning below 30% RH. These are configurable engineering defaults, not a universal compliance policy.
