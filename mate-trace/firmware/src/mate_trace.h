#ifndef MATE_TRACE_H
#define MATE_TRACE_H
#include <stdbool.h>
#include <stdint.h>

typedef struct {
    uint32_t run_id;
    int32_t peak_counts;
    uint32_t sample_count;
    bool active;
} mate_trace_t;

void mate_trace_init(mate_trace_t *state);
bool mate_trace_start(mate_trace_t *state, uint32_t run_id);
bool mate_trace_sample(mate_trace_t *state, int32_t counts);
bool mate_trace_finish(mate_trace_t *state);
#endif
