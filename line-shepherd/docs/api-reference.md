# Line Shepherd API Reference

## CSV Log Fields

| Field | Type | Description |
|-------|------|-------------|
| timestamp | ISO8601 | Capture time |
| asset_id | string | User-assigned cable or drop name |
| pair | string | Selected pair or conductor group |
| cable_type | string | Cable family preset |
| velocity_factor | float | Effective propagation VF used for distance estimate |
| round_trip_ns | float | Time to first major reflection |
| est_distance_m | float | Estimated distance to fault |
| refl_peak_v | float | Positive reflection magnitude |
| refl_trough_v | float | Negative reflection magnitude |
| ringing_ratio | float | Post-event ringing relative to main event |
| loop_res_ohm | float | Measured loop resistance |
| cap_nf | float | Estimated line capacitance |
| pair_balance_pct | float | Conductor or pair balance score |
| health_score | float | 0-100 line health metric |
| classification | enum | Final fault class |

## BLE Service Sketch

### Service UUID: `0x7A40`

| Characteristic | UUID | Access | Purpose |
|----------------|------|--------|---------|
| Live summary | `0x7A41` | Notify | Packed status/result frame |
| Trigger scan | `0x7A42` | Write | Start a measurement on selected pair |
| Cable preset | `0x7A43` | Read/Write | CAT5e, alarm, irrigation, fence, custom |
| Log export | `0x7A44` | Read | Chunked CSV transfer |
| Remote ID map | `0x7A45` | Read | Discovered far-end resistor-coded IDs |

## Wi-Fi JSON Payload

```json
{
  "asset_id": "WAREHOUSE-DROP12",
  "pair": "PAIR_A",
  "cable_type": "CAT5E",
  "velocity_factor": 0.68,
  "est_distance_m": 27.3,
  "classification": "SPLIT_PAIR",
  "loop_res_ohm": 18.4,
  "pair_balance_pct": 71.0,
  "health_score": 63.2
}
```

## Classification Summary

- positive dominant reflection + long distance -> `OPEN_END`
- negative dominant reflection + low resistance -> `SHORT_END`
- moderate reflection + elevated resistance -> `HIGH_RESISTANCE_SPLICE`
- broadened negative event + high capacitance imbalance -> `WATER_INGRESS`
- two similar positive events separated in time -> `BRIDGE_TAP`
- okay continuity + poor balance/crosstalk proxy -> `SPLIT_PAIR`
- weak reflection + good balance -> `HEALTHY_LINE`
- foreign voltage above threshold -> `OVERVOLTAGE_LOCKOUT`
