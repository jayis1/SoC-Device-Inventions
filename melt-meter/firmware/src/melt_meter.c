#include "melt_meter.h"
#include <stddef.h>
void mm_init(mm_state_t *state)
{
    if (state != NULL) { state->last_ms = 0U; state->start_ms = 0U; state->sequence = 0U; state->started = false; }
}
bool mm_decode_frame(uint32_t frame, int16_t *quarter_c, uint8_t *fault)
{
    if (quarter_c == NULL || fault == NULL) return false;
    *fault = (uint8_t)(frame & 0x07U);
    if ((frame & (1UL << 16)) != 0U) return false;
    int16_t raw = (int16_t)((frame >> 18) & 0x3FFFU);
    if ((raw & 0x2000) != 0) raw = (int16_t)(raw | (int16_t)0xC000);
    *quarter_c = raw;
    return true;
}
bool mm_sample(mm_state_t *state, uint32_t frame, uint32_t now_ms, mm_record_t *record)
{
    int16_t q = 0; uint8_t fault = 0;
    if (state == NULL || record == NULL) return false;
    if (state->started && (uint32_t)(now_ms - state->last_ms) < MM_PERIOD_MS) return false;
    if (!state->started) state->start_ms = now_ms;
    record->sequence = ++state->sequence; record->elapsed_ms = (uint32_t)(now_ms - state->start_ms);
    record->valid = mm_decode_frame(frame, &q, &fault); record->fault = fault;
    record->quarter_c = record->valid ? q : 0;
    state->last_ms = now_ms; state->started = true;
    return true;
}
