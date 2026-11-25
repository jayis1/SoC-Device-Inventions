#ifndef MELT_METER_H
#define MELT_METER_H
#include <stdbool.h>
#include <stdint.h>
#define MM_PERIOD_MS 250U
typedef struct { uint32_t last_ms; uint32_t start_ms; uint32_t sequence; bool started; } mm_state_t;
typedef struct { uint32_t sequence; uint32_t elapsed_ms; int16_t quarter_c; uint8_t fault; bool valid; } mm_record_t;
void mm_init(mm_state_t *state);
bool mm_decode_frame(uint32_t frame, int16_t *quarter_c, uint8_t *fault);
bool mm_sample(mm_state_t *state, uint32_t frame, uint32_t now_ms, mm_record_t *record);
#endif
