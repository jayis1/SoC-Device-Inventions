/* Host demonstration for Curb Cue. Designed and documented by jayis1. */
#include <stdio.h>
#include "curb_cue.h"

int main(void) {
    const curb_cue_config_t config = {.caution_mm = 1500U, .near_mm = 750U};
    const curb_cue_sample_t sample = {.low_mm = 620U, .high_mm = 1800U, .low_valid = true, .high_valid = true};
    const curb_cue_output_t output = curb_cue_evaluate(&config, &sample);
    printf("state=%s left=%d right=%d pulse_ms=%u\n", curb_cue_state_name(output.state),
           output.left_motor, output.right_motor, output.pulse_ms);
    return output.state == CURB_CUE_NEAR ? 0 : 1;
}
