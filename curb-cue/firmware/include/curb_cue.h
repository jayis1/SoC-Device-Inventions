/* Curb Cue policy core. Designed and documented by jayis1. */
#ifndef CURB_CUE_H
#define CURB_CUE_H

#include <stdbool.h>
#include <stdint.h>

typedef enum {
    CURB_CUE_CLEAR = 0,
    CURB_CUE_CAUTION,
    CURB_CUE_NEAR,
    CURB_CUE_BOTH_NEAR,
    CURB_CUE_SENSOR_FAULT
} curb_cue_state_t;

typedef struct {
    uint16_t low_mm;
    uint16_t high_mm;
    bool low_valid;
    bool high_valid;
} curb_cue_sample_t;

typedef struct {
    uint16_t caution_mm;
    uint16_t near_mm;
} curb_cue_config_t;

typedef struct {
    curb_cue_state_t state;
    bool left_motor;
    bool right_motor;
    uint16_t pulse_ms;
} curb_cue_output_t;

bool curb_cue_config_valid(const curb_cue_config_t *config);
curb_cue_output_t curb_cue_evaluate(const curb_cue_config_t *config,
                                    const curb_cue_sample_t *sample);
const char *curb_cue_state_name(curb_cue_state_t state);
#endif
