/* Warp Watch policy interface. Designed and documented by jayis1. */
#ifndef WARP_WATCH_H
#define WARP_WATCH_H

#include <stdbool.h>
#include <stdint.h>

typedef enum {
    WARP_WATCH_STABLE = 0,
    WARP_WATCH_SLACK,
    WARP_WATCH_OVER_TENSION,
    WARP_WATCH_TAKEUP_STALLED,
    WARP_WATCH_SENSOR_FAULT
} warp_watch_state_t;

typedef struct {
    uint16_t minimum_cN;
    uint16_t maximum_cN;
    uint16_t motion_timeout_s;
} warp_watch_config_t;

typedef struct {
    bool force_valid;
    bool motion_valid;
    uint16_t tension_cN;
    uint16_t seconds_without_motion;
} warp_watch_sample_t;

typedef struct {
    warp_watch_state_t state;
    bool amber_led;
    bool red_led;
    uint16_t beep_ms;
} warp_watch_output_t;

bool warp_watch_config_valid(const warp_watch_config_t *config);
warp_watch_output_t warp_watch_evaluate(const warp_watch_config_t *config,
                                        const warp_watch_sample_t *sample);
const char *warp_watch_state_name(warp_watch_state_t state);

#endif
