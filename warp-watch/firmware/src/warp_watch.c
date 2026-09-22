/* Warp Watch policy core. Designed and documented by jayis1. */
#include <stddef.h>
#include "warp_watch.h"

bool warp_watch_config_valid(const warp_watch_config_t *config) {
    return config != NULL && config->minimum_cN >= 5U &&
           config->maximum_cN > config->minimum_cN &&
           config->maximum_cN <= 5000U && config->motion_timeout_s >= 5U;
}

static warp_watch_output_t fault_output(void) {
    return (warp_watch_output_t){WARP_WATCH_SENSOR_FAULT, true, true, 120U};
}

warp_watch_output_t warp_watch_evaluate(const warp_watch_config_t *config,
                                        const warp_watch_sample_t *sample) {
    if (!warp_watch_config_valid(config) || sample == NULL ||
        !sample->force_valid || !sample->motion_valid) {
        return fault_output();
    }
    if (sample->tension_cN < config->minimum_cN) {
        return (warp_watch_output_t){WARP_WATCH_SLACK, true, false, 45U};
    }
    if (sample->tension_cN > config->maximum_cN) {
        return (warp_watch_output_t){WARP_WATCH_OVER_TENSION, false, true, 90U};
    }
    if (sample->seconds_without_motion >= config->motion_timeout_s) {
        return (warp_watch_output_t){WARP_WATCH_TAKEUP_STALLED, true, true, 65U};
    }
    return (warp_watch_output_t){WARP_WATCH_STABLE, false, false, 0U};
}

const char *warp_watch_state_name(warp_watch_state_t state) {
    static const char *const names[] = {
        "STABLE", "SLACK", "OVER_TENSION", "TAKEUP_STALLED", "SENSOR_FAULT"
    };
    return state <= WARP_WATCH_SENSOR_FAULT ? names[state] : "INVALID";
}
