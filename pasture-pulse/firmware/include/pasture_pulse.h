#ifndef PASTURE_PULSE_H
#define PASTURE_PULSE_H

#include <stddef.h>

#define PP_MAX_NOTES 3

typedef enum {
    PP_HEALTHY = 0,
    PP_VEGETATION_LOADING,
    PP_WET_INSULATOR_LEAKAGE,
    PP_OPEN_GATE_OR_BREAK,
    PP_WEAK_ENERGIZER_BATTERY,
    PP_SURGE_DISTURBANCE,
    PP_FENCE_DOWN
} pp_fault_t;

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

typedef struct {
    double energy_index;
    double leakage_ratio;
    double wetness_factor;
    double interval_error_pct;
    double fence_score;
} pp_features_t;

typedef struct {
    pp_fault_t fault;
    double confidence;
    pp_features_t features;
    const char *summary;
    const char *notes[PP_MAX_NOTES];
} pp_result_t;

const char *pp_fault_name(pp_fault_t fault);

#endif
