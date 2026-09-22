/* Host tests for Curb Cue. Designed and documented by jayis1. */
#include <assert.h>
#include "curb_cue.h"

int main(void) {
    const curb_cue_config_t c = {.caution_mm = 1500U, .near_mm = 750U};
    assert(curb_cue_config_valid(&c));
    assert(!curb_cue_config_valid(&(curb_cue_config_t){.caution_mm = 700U, .near_mm = 750U}));
    curb_cue_output_t clear = curb_cue_evaluate(&c, &(curb_cue_sample_t){2000U, 2000U, true, true});
    assert(clear.state == CURB_CUE_CLEAR && !clear.left_motor && !clear.right_motor);
    curb_cue_output_t near = curb_cue_evaluate(&c, &(curb_cue_sample_t){700U, 1700U, true, true});
    assert(near.state == CURB_CUE_NEAR && near.left_motor && !near.right_motor);
    curb_cue_output_t both = curb_cue_evaluate(&c, &(curb_cue_sample_t){700U, 700U, true, true});
    assert(both.state == CURB_CUE_BOTH_NEAR && both.left_motor && both.right_motor);
    curb_cue_output_t fault = curb_cue_evaluate(&c, &(curb_cue_sample_t){0U, 1000U, false, true});
    assert(fault.state == CURB_CUE_SENSOR_FAULT && fault.left_motor && fault.right_motor);
    return 0;
}
