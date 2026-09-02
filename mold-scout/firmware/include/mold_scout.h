#ifndef MOLD_SCOUT_H
#define MOLD_SCOUT_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    MS_STATE_NORMAL = 0,
    MS_STATE_CONDENSATION_RISK,
    MS_STATE_HIDDEN_MOISTURE,
    MS_STATE_ACTIVE_MOLD_RISK,
    MS_STATE_WET_BUILDING_MATERIAL
} ms_state_t;

typedef struct {
    const char *timestamp;
    float ambient_temp_c;
    float ambient_rh;
    float surface_temp_c;
    float thermal_delta_c;
    float impedance_kohm;
    float phase_deg;
    float voc_index;
    float gas_resistance_kohm;
    float accel_motion_g;
    float battery_v;
} ms_sensor_window_t;

typedef struct {
    float dewpoint_margin_c;
    float moisture_proxy;
    float condensation_risk;
    float thermal_bridge_score;
    float voc_risk;
    float mold_risk_score;
} ms_features_t;

typedef struct {
    ms_state_t state;
    const char *recommended_action;
} ms_decision_t;

const char *ms_state_name(ms_state_t state);

#ifdef __cplusplus
}
#endif

#endif
