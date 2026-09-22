/* Curb Cue policy core. Designed and documented by jayis1. */
#include <stddef.h>
#include "curb_cue.h"

bool curb_cue_config_valid(const curb_cue_config_t *config) {
    return config != NULL && config->near_mm >= 200U &&
           config->caution_mm > config->near_mm && config->caution_mm <= 4000U;
}

static curb_cue_output_t fault_output(void) {
    return (curb_cue_output_t){CURB_CUE_SENSOR_FAULT, true, true, 80U};
}

curb_cue_output_t curb_cue_evaluate(const curb_cue_config_t *config,
                                    const curb_cue_sample_t *sample) {
    if (!curb_cue_config_valid(config) || sample == NULL ||
        !sample->low_valid || !sample->high_valid ||
        sample->low_mm == 0U || sample->high_mm == 0U) {
        return fault_output();
    }
    bool low_near = sample->low_mm <= config->near_mm;
    bool high_near = sample->high_mm <= config->near_mm;
    bool low_caution = sample->low_mm <= config->caution_mm;
    bool high_caution = sample->high_mm <= config->caution_mm;
    if (low_near && high_near) {
        return (curb_cue_output_t){CURB_CUE_BOTH_NEAR, true, true, 120U};
    }
    if (low_near || high_near) {
        return (curb_cue_output_t){CURB_CUE_NEAR, low_near, high_near, 90U};
    }
    if (low_caution || high_caution) {
        return (curb_cue_output_t){CURB_CUE_CAUTION, low_caution, high_caution, 45U};
    }
    return (curb_cue_output_t){CURB_CUE_CLEAR, false, false, 0U};
}

const char *curb_cue_state_name(curb_cue_state_t state) {
    static const char *const names[] = {"CLEAR", "CAUTION", "NEAR", "BOTH_NEAR", "SENSOR_FAULT"};
    return state <= CURB_CUE_SENSOR_FAULT ? names[state] : "INVALID";
}
