# Panel Warden API Reference

## CSV Log Fields

| Field | Type | Description |
|-------|------|-------------|
| timestamp | ISO8601 | Event or sample time |
| asset_id | string | User-assigned panel identifier |
| fault | enum | NORMAL / OVERLOAD_HEATING / LOOSE_LUG / CORONA_PD / SURFACE_TRACKING / INTERMITTENT_ARC / CONDENSATION_RISK |
| risk_score | float | 0-100 severity score |
| confidence | float | 0-1 classifier confidence |
| hotspot_c | float | Hottest thermal pixel temperature |
| background_c | float | Median panel temperature |
| delta_t_c | float | hotspot minus background |
| hotspot_area_px | float | Hot region pixel count |
| rise_rate_c_per_hr | float | Trend of hot spot |
| ultrasound_db | float | Envelope-equivalent ultrasonic intensity |
| rf_dbm_equiv | float | RF detector equivalent strength |
| burst_rate_hz | float | RF/ultrasonic event repetition rate |
| load_corr_a | float | Relative CT current estimate |
| humidity_pct | float | Relative humidity inside enclosure |
| ambient_c | float | Enclosure air temperature |
| contact_c | float | Cabinet-wall contact temperature |
| dewpoint_c | float | Computed dew point |
| dew_margin_c | float | contact minus dewpoint |
| note | string | Recommended maintenance action |

## BLE GATT Sketch

### Service UUID: `0x7A80`

| Characteristic | UUID | Access | Purpose |
|----------------|------|--------|---------|
| Live summary | `0x7A81` | Notify | Latest fault/risk packet |
| Alarm acknowledge | `0x7A82` | Write | Silence buzzer / mark event seen |
| Threshold profile | `0x7A83` | Read/Write | Site-specific sensitivity tuning |
| Log export | `0x7A84` | Read | Chunked CSV event transfer |
| Install metadata | `0x7A85` | Read/Write | Panel type, voltage class, feeder label |

## Wi-Fi JSON Event

```json
{
  "asset_id": "MCC-A1",
  "fault": "LOOSE_LUG",
  "risk_score": 76.4,
  "confidence": 0.86,
  "hotspot_c": 87.2,
  "delta_t_c": 42.3,
  "ultrasound_db": 18.0,
  "rf_dbm_equiv": -53.0,
  "dew_margin_c": 27.0,
  "note": "localized hotspot exceeds expected load correlation; re-torque and inspect lug oxidation"
}
```

## OTA Strategy

- app partition + OTA_0 / OTA_1 dual slots
- versioned threshold profiles stored in NVS
- event logs remain on microSD and are not erased during firmware update
