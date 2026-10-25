#include <stdbool.h>
#include <stdint.h>

#define SS_SAMPLE_HZ 20u
#define SS_JAM_FEED_MM 5.0f
#define SS_WHEEL_CIRCUMFERENCE_MM 20.0f

typedef struct {
    uint16_t angle_raw; /* AS5600 12-bit angle, 0..4095 */
    uint32_t spool_edges;
    bool angle_valid;
    bool spool_sensor_valid;
} ss_sample_t;

typedef struct {
    uint16_t previous_angle;
    uint32_t previous_spool_edges;
    bool initialized;
} ss_state_t;

typedef struct {
    float feed_mm;
    uint32_t spool_edges;
    bool sensor_fault;
    bool motion_disagreement;
} ss_result_t;

/* Portable calculation core. Caller bounds sampling to SS_SAMPLE_HZ and supplies
 * validated, coherent readings; this function never drives printer hardware. */
ss_result_t ss_update(ss_state_t *state, const ss_sample_t *sample)
{
    ss_result_t result = {0.0f, 0u, false, false};
    if (state == 0 || sample == 0 || sample->angle_raw > 4095u) {
        result.sensor_fault = true;
        return result;
    }
    if (!sample->angle_valid || !sample->spool_sensor_valid) {
        result.sensor_fault = true;
        return result;
    }
    if (!state->initialized) {
        state->previous_angle = sample->angle_raw;
        state->previous_spool_edges = sample->spool_edges;
        state->initialized = true;
        return result;
    }
    int32_t delta = (int32_t)sample->angle_raw - (int32_t)state->previous_angle;
    if (delta > 2048) delta -= 4096;
    if (delta < -2048) delta += 4096;
    result.feed_mm = ((float)delta / 4096.0f) * SS_WHEEL_CIRCUMFERENCE_MM;
    result.spool_edges = sample->spool_edges - state->previous_spool_edges;
    result.motion_disagreement = (result.feed_mm >= SS_JAM_FEED_MM && result.spool_edges == 0u);
    state->previous_angle = sample->angle_raw;
    state->previous_spool_edges = sample->spool_edges;
    return result;
}
