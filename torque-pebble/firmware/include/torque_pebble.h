#ifndef TORQUE_PEBBLE_H
#define TORQUE_PEBBLE_H

#include <stddef.h>

typedef enum {
    TP_STATE_PASS = 0,
    TP_STATE_UNDER_TORQUE,
    TP_STATE_OVER_TORQUE,
    TP_STATE_SOFT_JOINT,
    TP_STATE_CROSS_THREAD_RISK,
    TP_STATE_REWORK_DETECTED
} tp_state_t;

typedef struct {
    const char *name;
    float target_torque_nm;
    float torque_low_nm;
    float torque_high_nm;
    float target_angle_deg;
    float angle_window_deg;
    int enforce_angle;
} tp_preset_t;

typedef struct {
    const char *timestamp;
    const tp_preset_t *preset;
    size_t sample_count;
    float torque_nm[32];
    float angle_deg[32];
    float accel_g[32];
    float temperature_c;
} tp_event_t;

typedef struct {
    float peak_torque_nm;
    float snug_torque_nm;
    float angle_after_snug_deg;
    float prevailing_torque_nm;
    float stiffness_nm_per_deg;
    float backoff_deg;
    float yield_risk;
    float cross_thread_score;
} tp_features_t;

typedef struct {
    tp_state_t state;
    const char *recommended_action;
} tp_decision_t;

const char *tp_state_name(tp_state_t state);
const tp_preset_t *tp_default_presets(size_t *count);

#endif
