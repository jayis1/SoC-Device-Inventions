# Pasture Pulse API Reference

## Frame Structure

The host simulation operates on this conceptual frame:

```c
typedef struct {
    const char *scenario_name;
    double pulse_peak_kv;
    double rise_time_us;
    double decay_time_us;
    double pulse_interval_s;
    double current_peak_a;
    double ambient_temp_c;
    double humidity_rh;
    double dewpoint_margin_c;
    double battery_v;
    int gate_open;
    int surge_flag;
    int pulse_missing;
} pp_frame_t;
```

## Derived Features

`pp_extract_features()` computes:

- `energy_index` — relative pulse-delivery metric
- `leakage_ratio` — current vs voltage weighted loading metric
- `wetness_factor` — humidity/dew driven leakage bias
- `interval_error_pct` — cadence deviation from nominal 1 s pulse train
- `fence_score` — 0-100 heuristic health score

## Fault Classes

- `PP_HEALTHY`
- `PP_VEGETATION_LOADING`
- `PP_WET_INSULATOR_LEAKAGE`
- `PP_OPEN_GATE_OR_BREAK`
- `PP_WEAK_ENERGIZER_BATTERY`
- `PP_SURGE_DISTURBANCE`
- `PP_FENCE_DOWN`

## Host Build

```bash
cd firmware
cmake -S . -B build
cmake --build build
./build/pasture_pulse_fw_sim
```

## Suggested Future Embedded Tasks

- replace demo frames with DMA waveform capture from ADS131M02
- add FRAM ring buffer persistence
- add LoRaWAN uplink serializer for periodic summaries
- add remote threshold update command parser
- add dual-node correlation for directional fault location
