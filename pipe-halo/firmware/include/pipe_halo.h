#ifndef PIPE_HALO_H
#define PIPE_HALO_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    PH_STATE_IDLE = 0,
    PH_STATE_NORMAL_FLOW,
    PH_STATE_DRIP_LEAK,
    PH_STATE_WATER_HAMMER,
    PH_STATE_FREEZE_RISK,
    PH_STATE_BURST_RISK,
    PH_STATE_VALVE_TAMPER
} ph_state_t;

typedef struct {
    const char *timestamp;
    float pipe_temp_upstream_c;
    float pipe_temp_downstream_c;
    float ambient_temp_c;
    float humidity_rh;
    float vibration_rms_g;
    float acoustic_rms_mv;
    float spectral_centroid_hz;
    float drip_period_s;
    float pressure_swing_kpa;
    float valve_angle_deg;
    float battery_v;
} ph_sensor_window_t;

typedef struct {
    float flow_index;
    float hammer_index;
    float drip_confidence;
    float freeze_margin_c;
    float burst_risk_score;
} ph_features_t;

typedef struct {
    ph_state_t state;
    const char *recommended_action;
} ph_decision_t;

const char *ph_state_name(ph_state_t state);

#ifdef __cplusplus
}
#endif

#endif
