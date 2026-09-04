#ifndef RELAY_FINCH_H
#define RELAY_FINCH_H

#include <stddef.h>
#include <stdbool.h>

#define RF_MAX_SAMPLES 48
#define RF_TRAVEL_ACTUATED_RATIO 0.75f

typedef enum {
    RF_MODE_PULL_IN_SWEEP,
    RF_MODE_HOLD_RELEASE_SWEEP,
    RF_MODE_BOUNCE_CAPTURE,
    RF_MODE_LIFE_SPOT_CHECK
} rf_test_mode_t;

typedef enum {
    RF_STATE_PASS,
    RF_STATE_SLOW_PULL_IN,
    RF_STATE_EXCESSIVE_BOUNCE,
    RF_STATE_LOW_COIL_RESISTANCE,
    RF_STATE_WEAK_RETURN_SPRING,
    RF_STATE_WELDED_CONTACTS,
    RF_STATE_NO_ACTUATION
} rf_state_t;

typedef struct {
    const char *name;
    float nominal_voltage;
    float max_pull_in_v;
    float min_drop_out_v;
    float max_drop_out_v;
    float max_operate_ms;
    float max_bounce_ms;
    float min_travel_mm;
    float min_coil_resistance_ohm;
    float release_timeout_ms;
} rf_preset_t;

typedef struct {
    float time_ms;
    float coil_v;
    float current_ma;
    bool contact_closed;
    float hall_mm;
    float acoustic;
} rf_sample_t;

typedef struct {
    const char *timestamp;
    rf_test_mode_t mode;
    const rf_preset_t *preset;
    size_t sample_count;
    rf_sample_t samples[RF_MAX_SAMPLES];
} rf_event_t;

typedef struct {
    float pull_in_v;
    float drop_out_v;
    float operate_ms;
    float release_ms;
    float bounce_ms;
    int chatter_edges;
    float pickup_current_ma;
    float hold_current_ma;
    float coil_resistance_ohm;
    float max_travel_mm;
    float acoustic_peak;
    bool contact_released;
    bool actuated;
} rf_features_t;

typedef struct {
    rf_state_t state;
    const char *recommended_action;
} rf_decision_t;

const char *rf_state_name(rf_state_t state);
const char *rf_mode_name(rf_test_mode_t mode);

#endif
