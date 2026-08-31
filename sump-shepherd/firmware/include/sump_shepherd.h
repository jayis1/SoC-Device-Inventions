#ifndef SUMP_SHEPHERD_H
#define SUMP_SHEPHERD_H

#include <stdbool.h>
#include <stddef.h>

#define SS_MAX_LABEL_LEN 32

typedef struct {
    char label[SS_MAX_LABEL_LEN];
    float level_mm;
    float rise_rate_mm_min;
    float drawdown_rate_mm_min;
    float pump_current_a;
    float vibration_g_rms;
    float humidity_rh;
    float battery_v;
    float rebound_mm_60s;
    bool float_active;
    bool leak_active;
    bool power_fail;
} ss_frame_t;

typedef enum {
    SS_STATE_HEALTHY = 0,
    SS_STATE_NO_START,
    SS_STATE_BACKFLOW,
    SS_STATE_SHORT_CYCLING,
    SS_STATE_DRY_RUN,
    SS_STATE_SLOW_PUMP,
    SS_STATE_LEAK_CONFIRMED,
    SS_STATE_HUMIDITY_ALERT
} ss_state_t;

typedef struct {
    ss_state_t state;
    float fault_score;
    float flood_risk_score;
    float cycle_health_score;
} ss_result_t;

const char *ss_state_name(ss_state_t state);

#endif
